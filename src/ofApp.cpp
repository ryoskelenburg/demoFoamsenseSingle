#include "ofApp.h"
void ofApp::setup(){
    ofBackground(20, 20, 20);
    ofSetVerticalSync(true);
    ofSetFrameRate(FRAMERATE_NUM);
    font.load("franklinGothic.otf", 16);
    smallFont.load("franklinGothic.otf", 10);
    initArduino();
    
    gui.setup(); //ofxGui
    gui.add(operateMinValueA0.setup("minValue: A0",200, 0, 1023));
    gui.add(operateMaxValueA0.setup("MaxValue: A0",933, 0, 1023));
    //gui.add(operateMinValueA1.setup("minValue: A1",300, 0, 1023));
    //gui.add(operateMaxValueA1.setup("MaxValue: A1",550, 0, 1023));
    
    setupHistoryPlot(); //ofxHistoryPlot
    

//------import file--------
//    myReadFile.open("text.txt",ofFile::ReadOnly);
//    cout << myReadFile.readToBuffer().getText();
//    auto input = ofSplitString(myReadFile.readToBuffer().getText(), "\n");
//        for(int i= 0; i < RECORD_NUM;i++)
//        {
//            recordAnalog[i] = stoi(input[i]);
//        }
//-------------------------
    
    recordFile.open("recordData" + ofGetTimestampString() + ".txt",ofFile::WriteOnly);
    feedbackFile.open("feedBackData" + ofGetTimestampString() + ".txt",ofFile::WriteOnly);
}

void ofApp::update(){
    currentFrameRate = ofGetFrameRate();
    updateArduino();
    
    rawInputValue = ard.getAnalog(elastPin01);
    filterInputValue[1] = a * filterInputValue[0] + (1-a) * rawInputValue;
    
    propotionVolume[0] = ofMap(filterInputValue[1], minValue[0], maxValue[0], 0, DEFORM_RESOLUSION);
    
    if(filterInputValue[1] > maxValue[0]){
        maxValue[0] = filterInputValue[1];
    }
    if(filterInputValue[1] < minValue[0]){
        minValue[0] = filterInputValue[1];
    }
    
    plot->update(propotionVolume[0]);
    //plot2->update(currentVolume[1]);
    plot3->update(recordAnalog[count]);
    
    milliSeconds = ofGetElapsedTimeMillis();
    
    //-------------------
    
    if (bRecord == true) {
        if(count >= RECORD_NUM){
            //manipulateElastOff();
            bRecord = false;
            countClear();
        } else {
            //manipulateElastOn();
            record();
            recordFile << recordAnalog[count] << endl;
        }
        
    }
    
    //--------------------
    
    if (bPlay == true) {
        
        if (playCount >= RECORD_NUM) {
            bPlay = false;
            countClear();
            sendDigitalArduinoMaintain();
        } else {
            delta = checkDelta(recordAnalog[count], recordAnalog[count-1]);
            absDelta = absoluteDelta(delta);
            play();
            feedbackFile << propotionVolume[0]  << endl;
        }
        
        playCount++;
    }
    
    
    //-------------------
    
//    if (bRecordWrite == true) {
//
//        myTextFile.open("text.txt",ofFile::WriteOnly);
//        for(int i = 0;i < RECORD_NUM;i++)
//        {
//            myTextFile << recordAnalog[i] << endl;
//        }
//        bRecordWrite = false;
//    }
    
    count++;
}

void ofApp::draw(){
    drawLog();
    gui.draw();
    
    plot->draw(0, 400, ofGetWidth(), 400);
    //plot2->draw(0, 400, ofGetWidth(), 400);
    if (bDrawPlot == true) {
        if (playCount >= RECORD_NUM) {
            bDrawPlot = false;
        } else {
            plot3->draw(0,400, ofGetWidth(), 400);
        }
    }
    
    filterInputValue[0] = filterInputValue[1];
}

void ofApp::record(){
    recordAnalog[count] = propotionVolume[0];
}

void ofApp::play(){
    
    std::cout << "count :" << count << " ,recordAnalog :"<< recordAnalog[count] << " ,delta :"<<  delta << endl;
    
    startTime = ofGetElapsedTimeMillis();
    if(delta > 0){
        //inflation
        //startTime = ofGetElapsedTimeMillis();
        bDeform = true;
        while(bDeform) {
            sendDigitalArduinoInflation();
            stopActuate();
        }
    }else if(delta < 0){
        //defltation
        //startTime = ofGetElapsedTimeMillis();
        bDeform = true;
        while(bDeform) {
            sendDigitalArduinoDeflation();
            stopActuate();
        }
    } else if(delta == 0){
        sendDigitalArduinoMaintain();
    }
    
}

void ofApp::countClear(){
    count = 0;
    playCount = 0;
    startTime = 0;
}

int ofApp::checkDelta(int value, int oldValue){
    return value - oldValue;
}

int ofApp::absoluteDelta(int _delta){
    return abs(_delta);
}

int ofApp::deltaDelta(int _x, int _y){
    return abs(_x - _y);
}

void ofApp::stopActuate(){
    //RFP32B03B: 3.8L/min = 63ml/sec
    //elastV1.0: 90ml, resolution:8, 11.25ml
    //resolution : 0.179sec = 179milliseconds
    //the elapsed time in milliseconds (1000 milliseconds = 1 second).
    if(ofGetElapsedTimeMillis() - startTime < 1000 * (1/ 30)) {
        bDeform = true;
    } else {
        bDeform = false;
    }
}

void ofApp::coolDown(){
    
}

void ofApp::sendDigitalArduinoInflation(){
    ard.sendDigital(valvePin[6], ARD_HIGH);
    ard.sendDigital(valvePin[7], ARD_HIGH);
    ard.sendDigital(pumpPin01, ARD_LOW);
    ard.sendDigital(pumpPin02, ARD_HIGH);
}

void ofApp::sendDigitalArduinoDeflation(){
    ard.sendDigital(valvePin[6], ARD_HIGH);
    ard.sendDigital(valvePin[7], ARD_HIGH);
    ard.sendDigital(pumpPin01, ARD_HIGH);
    ard.sendDigital(pumpPin02, ARD_LOW);
}

void ofApp::sendDigitalArduinoMaintain(){
    ard.sendDigital(valvePin[6], ARD_LOW);
    ard.sendDigital(valvePin[7], ARD_LOW);
    ard.sendDigital(pumpPin01, ARD_LOW);
    ard.sendDigital(pumpPin02, ARD_LOW);
}

void ofApp::manipulateElastOn(){
    ard.sendDigital(valvePin[6], ARD_HIGH);
    ard.sendDigital(valvePin[7], ARD_HIGH);
}

void ofApp::manipulateElastOff(){
    ard.sendDigital(valvePin[6], ARD_LOW);
    ard.sendDigital(valvePin[7], ARD_LOW);
}

void ofApp::drawLog(){
    ofSetColor(255);
    
    if (!bSetupArduino){
        font.drawString("Connect ready...\n", valueRow[0], valueCol[1]);
    } else {
        if (bRecord == true) {
            font.drawString("Recording...\n", valueRow[0], valueCol[1]);
        } else if (bPlay == true) {
            font.drawString("Playing...\n", valueRow[0], valueCol[1]);
        } else {
            font.drawString("Connect succeed!\n", valueRow[0], valueCol[1]);
        }
    }
    
    font.drawString("Propotion : " + ofToString(propotionVolume[0]), valueRow[2], valueCol[0]);
    smallFont.drawString("rawInputValue  :  " + ofToString(rawInputValue), valueRow[2], valueCol[0] + 20);
    smallFont.drawString("InputValue     :  " + ofToString(filterInputValue[1]), valueRow[2], valueCol[0] + 40);
    smallFont.drawString("minValue  :  " + ofToString(minValue[0]), valueRow[2], valueCol[0] + 60);
    smallFont.drawString("maxValue     :  " + ofToString(maxValue[0]), valueRow[2], valueCol[0] + 80);
//    font.drawString("Current propotion : " + ofToString(propotionVolume[1]) + ", Current Volume : " + ofToString(currentVolume[1]) + "ml", valueRow[2], valueCol[1]);
//    smallFont.drawString("rawOutputValue :  " + ofToString(rawOutputValue), valueRow[2], valueCol[1] + 20);
//    smallFont.drawString("OutputValue    :  " + ofToString(filterOutputValue[1]), valueRow[2], valueCol[1] + 40);
//    smallFont.drawString("minValue  :  " + ofToString(minValue[1]), valueRow[2], valueCol[1] + 60);
//    smallFont.drawString("maxValue     :  " + ofToString(maxValue[1]), valueRow[2],  valueCol[1] + 80);
    
    smallFont.drawString("count : " + ofToString(count) , valueRow[1], valueCol[0]+10);
    smallFont.drawString("playCount : " + ofToString(playCount), valueRow[1], valueCol[0]+20);
    smallFont.drawString("recordAnalog[0] : " + ofToString(recordAnalog[0]) , valueRow[1], valueCol[0]+30);
    smallFont.drawString("recordAnalog[1] : " + ofToString(recordAnalog[1]) , valueRow[1], valueCol[0]+40);
    smallFont.drawString("recordAnalog[LAST] : " + ofToString(recordAnalog[RECORD_NUM - 1]) , valueRow[1], valueCol[0]+50);
    
//    smallFont.drawString("bRecord: " + ofToString(bRecord) , valueRow[1], valueCol[0] + 120);
//    smallFont.drawString("bPlay: " + ofToString(bPlay) , valueRow[1], valueCol[0] + 140);
//    smallFont.drawString("bDeform: " + ofToString(bDeform) , valueRow[1], valueCol[0] + 160);
//    smallFont.drawString("delta: " + ofToString(delta) , valueRow[1], valueCol[0] + 200);
//    smallFont.drawString("absDelta: " + ofToString(absDelta) , valueRow[1], valueCol[0] + 220);
//    smallFont.drawString("bWrite: " + ofToString(bWrite) , valueRow[1], valueCol[0] + 240);
    
    smallFont.drawString("millis: " + ofToString(milliSeconds), valueRow[0], valueCol[1] + 50);
    smallFont.drawString("startTime: " + ofToString(startTime), valueRow[0], valueCol[1] + 70);
    smallFont.drawString("--------- INPUT", valueRow[0], valueCol[1] + 100);
    ofSetColor(255, 0, 0);
    smallFont.drawString("--------- OUTPUT", valueRow[0], valueCol[1] + 120);
    
    //    std::cout << "current[" << count << "]: " << recordAnalog[count] << endl;
    //    std::cout << "recordAnalog[" << count-1 << "]: " << recordAnalog[count-1] << endl;
}

void ofApp::keyPressed(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case 'c':
            minValue[0] = operateMinValueA0;
            maxValue[0] = operateMaxValueA0;
            minValue[1] = operateMinValueA1;
            maxValue[1] = operateMaxValueA1;
            break;
        case 'v':
            milliSeconds = 0;
            break;
        case 's':
            bRecord = true;
            countClear();
            break;
        case 'r':
            bPlay = true;
            countClear();
            plot->reset();
            plot3->reset();
            bDrawPlot = true;
            break;
        case 'l':
            bLed = true;
            break;
        case 'm':
            bDeform = true;
            break;
        case 'q':
            //bPlay = false;
            //bDeform = false;
            sendDigitalArduinoMaintain();
            break;
        case 'w':
            //bRecordWrite = true;
            break;
        default:
            break;
    }
}

void ofApp::keyReleased(int key){
    switch (key) {
        case 'l':
            bLed = false;
            break;
        case 'm':
            bDeform = false;
            break;
        default:
            break;
    }
}
void ofApp::mouseMoved(int x, int y ){
}
void ofApp::mouseDragged(int x, int y, int button){
}
void ofApp::mousePressed(int x, int y, int button){
}
void ofApp::mouseReleased(int x, int y, int button){
}
void ofApp::mouseEntered(int x, int y){
}
void ofApp::mouseExited(int x, int y){
}
void ofApp::windowResized(int w, int h){
}
void ofApp::gotMessage(ofMessage msg){
}
void ofApp::dragEvent(ofDragInfo dragInfo){
}

void ofApp::initArduino(){
    ard.connect("/dev/cu.usbmodem1411", 57600);
    
    ofAddListener(ard.EInitialized, this, &ofApp::setupArduino);
    bSetupArduino    = false;
}

void ofApp::setupArduino(const int & version) {
    ofRemoveListener(ard.EInitialized, this, &ofApp::setupArduino);
    bSetupArduino = true;
    ofLogNotice() << ard.getFirmwareName();
    ofLogNotice() << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion();
    
    // Note: pins filterInputValue - A5 can be used as digital input and output.
    // Refer to them as pins 14 - 19 if using StandardFirmata from Arduino 1.0.
    // If using Arduino 0022 or older, then use 16 - 21.
    // Firmata pin numbering changed in version 2.3 (which is included in Arduino 1.0)
    //ard.sendDigitalPinMode(2, ARD_INPUT);
    ard.sendDigitalPinMode(19, ARD_INPUT);  // pin 21 if using StandardFirmata from Arduino 0022 or older
    ard.sendAnalogPinReporting(0, ARD_ANALOG); //A0
    ard.sendAnalogPinReporting(1, ARD_ANALOG); //A1
    
    ard.sendAnalogPinReporting(elastPin01, ARD_ANALOG); //A4
    //ard.sendAnalogPinReporting(elastPin02, ARD_ANALOG); //A5
    
    ard.sendDigitalPinMode(valvePin[0], ARD_OUTPUT); //D2
    ard.sendDigitalPinMode(valvePin[1], ARD_OUTPUT); //D3
    ard.sendDigitalPinMode(valvePin[2], ARD_OUTPUT); //D4
    ard.sendDigitalPinMode(valvePin[3], ARD_OUTPUT); //D5
    ard.sendDigitalPinMode(valvePin[4], ARD_OUTPUT); //D6
    ard.sendDigitalPinMode(valvePin[5], ARD_OUTPUT); //D7
    ard.sendDigitalPinMode(valvePin[6], ARD_OUTPUT); //D8
    ard.sendDigitalPinMode(ledPin, ARD_OUTPUT); //D53
    
    ard.sendDigitalPinMode(pumpPin01, ARD_OUTPUT); //D11
    ard.sendDigitalPinMode(pumpPin02, ARD_OUTPUT); //D12
    
    //ard.sendServoAttach(9);
    //ard.sendDigitalPinMode(ledPin, ARD_OUTPUT);
    //ard.sendDigitalPinMode(ledPin, ARD_PWM);
    
    ofAddListener(ard.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
    ofAddListener(ard.EAnalogPinChanged, this, &ofApp::analogPinChanged);
}

void ofApp::updateArduino(){
    ard.update();
    //    if (bSetupArduino) {
    //        ard.sendPwm(11, (int)(128 + 128 * sin(ofGetElapsedTimef())));   // pwm...
    //    }
}

void ofApp::digitalPinChanged(const int & pinNum) {
    // do something with the digital input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    buttonState = "digital pin: " + ofToString(pinNum) + " = " + ofToString(ard.getDigital(pinNum));
}

void ofApp::analogPinChanged(const int & pinNum) {
    // do something with the analog input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    potValue = "analog pin: " + ofToString(pinNum) + " = " + ofToString(ard.getAnalog(pinNum));
}

void ofApp::setupHistoryPlot(){
    plot = new ofxHistoryPlot(&currentFrameRate, "timeline", ofGetWidth(), false);
    plot->setBackgroundColor(ofColor(0,0,0,0));
    plot->setColor( ofColor(255,255,255) );
    //plot->setShowNumericalInfo(true);
    plot->setRange(-DEFORM_RESOLUSION - 10, DEFORM_RESOLUSION + 10);//definable range of plot
    plot->setRespectBorders(false);
    plot->setLineWidth(1);
    plot->setCropToRect(false);
    plot->setDrawGrid(false);
    plot->setGridUnit(16);
    plot->setGridColor(ofColor(100));
    plot->setShowSmoothedCurve(false);
    plot->setSmoothFilter(0.1); //smooth filter strength
    
    plot2 = new ofxHistoryPlot(&currentFrameRate, "hogehogehoge", ofGetWidth(), false);
    plot2->setBackgroundColor(ofColor(0,0,0,0));
    plot2->setColor( ofColor(255,0,0) );
    plot2->setRange(-100, 100);//definable range of plot
    plot2->setRespectBorders(false);
    plot2->setLineWidth(1);
    plot2->setCropToRect(false);
    plot2->setDrawGrid(false);
    plot2->setGridUnit(16);
    plot2->setShowSmoothedCurve(false);
    plot2->setSmoothFilter(0.1); //smooth filter strength
    
    plot3 = new ofxHistoryPlot(&currentFrameRate, "recordValue", ofGetWidth(), false);
    plot3->setBackgroundColor(ofColor(0,0,0,0));
    plot3->setColor( ofColor(255,0,0) );
    plot3->setRange(-DEFORM_RESOLUSION - 10, DEFORM_RESOLUSION + 10);//definable range of plot
    plot3->setRespectBorders(false);
    plot3->setLineWidth(1);
    plot3->setCropToRect(false);
    plot3->setDrawGrid(false);
    plot3->setGridUnit(16);
    plot3->setShowSmoothedCurve(false);
    plot3->setSmoothFilter(0.1); //smooth filter strength
}

void ofApp::ledTest(){
    if (bLed) {
        ard.sendDigital(ledPin, ARD_HIGH);
    } else {
        ard.sendDigital(ledPin, ARD_LOW);
    }
}

double ofApp::ceil2(double dIn, int nLen){
    double dOut;
    dOut = dIn * pow(10.0, nLen);
    dOut = (double)(int)(dOut + 0.9);
    return dOut * pow(10.0, -nLen);
}
