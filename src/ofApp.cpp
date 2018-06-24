#include "ofApp.h"
void ofApp::setup(){
    ofBackground(20, 20, 20);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    font.load("franklinGothic.otf", 16);
    smallFont.load("franklinGothic.otf", 10);
    initArduino();
    
    gui.setup(); //ofxGui
    gui.add(operateMinValueA0.setup("minValue: A0",10, 0, 1023));
    gui.add(operateMaxValueA0.setup("MaxValue: A0",200, 0, 1023));
    gui.add(operateMinValueA1.setup("minValue: A1",300, 0, 1023));
    gui.add(operateMaxValueA1.setup("MaxValue: A1",550, 0, 1023));
    
    setupHistoryPlot(); //ofxHistoryPlot

}

void ofApp::update(){
    currentFrameRate = ofGetFrameRate();
    updateArduino();
    
    rawInputValue = ard.getAnalog(elastPin01); //input from Arduino
    rawOutputValue = ard.getAnalog(1);
    filterInputValue[1] = a * filterInputValue[0] + (1-a) * rawInputValue; //filtering
    filterOutputValue[1] = a * filterOutputValue[0] + (1-a) * rawOutputValue;
    
    propotionVolume[0] = ofMap(filterInputValue[1], minValue[0], maxValue[0], 0, 20);
    propotionVolume[1] = ofMap(filterOutputValue[1], minValue[1], maxValue[1], 0, 20);
    
    currentVolume[0] = ofMap(propotionVolume[0], 0, 20, -30, 60);
    currentVolume[1] = ofMap(propotionVolume[1], 0, 20, -30, 60);
    
    if(filterInputValue[1] > maxValue[0]){ //setting maxvalue & minvalue
        maxValue[0] = filterInputValue[1];
    }
    if(filterInputValue[1] < minValue[0]){
        minValue[0] = filterInputValue[1];
    }
    if(filterOutputValue[1] > maxValue[1]){
        maxValue[1] = filterOutputValue[1];
    }
    if(filterOutputValue[1] < minValue[1]){
        minValue[1] = filterOutputValue[1];
    }
    
    plot->update(currentVolume[0]);
    plot2->update(currentVolume[1]);
    
    milliSeconds = ofGetElapsedTimeMillis();
    
    //"change false"
    //if true, change false after elapased time * delta.
    if(bDeform == true) {
        if(ofGetElapsedTimeMillis() - startTime < 70 * delta) {
            bDeform = true;
        } else {
            bDeform = false;
        }
    }
    
    if (bRecord == true) {
        if(count > RECORD_NUM){
            bRecord = false;
            countClear();
        } else {
            recordAnalog[count - 1] = filterInputValue[0];
        }
    }
    count++;
    
    if (bPlay == true) {
        if (playCount > RECORD_NUM) {
            bPlay = false;
            countClear();
        } else {
            if(recordAnalog[playCount]> 200){
                ard.sendDigital(ledPin, ARD_HIGH);
            } else {
                ard.sendDigital(ledPin, ARD_LOW);
            }
        }
        playCount++;
    }
}

//void ofApp::checktime(){
//
//}

void ofApp::draw(){
    
    //"change true"
    //compare input and output value, if delta > value, change true and timer starting.
    controlPomp(propotionVolume[0], propotionVolume[1]);
    
    //***--maser method--***
    if(bDeform == true) {
        //ard.sendDigital(pumpPin, ARD_HIGH);
        if(bPolarity == true){
            sendDigitalArduinoInflation();
        }else{
            sendDigitalArduinoDeflation();
        }
    } else {
        //ard.sendDigital(pumpPin, ARD_LOW);
        sendDigitalArduinoMaintain();
    }
    //***----***
    
    drawLog();
    gui.draw();
    
    plot->draw(0, 300, ofGetWidth(), 300);
    plot2->draw(0, 300, ofGetWidth(), 300);
    filterInputValue[0] = filterInputValue[1];
    filterOutputValue[0] = filterOutputValue[1];
}

void ofApp::controlPomp(int input, int output){
    if ((input - output) > 0) {
        bPolarity = true;
    } else if ((input - output) < 0){
        bPolarity = false;
    }
    
    delta = abs(input - output);
    
    if(delta > 1) {
        //startDeform(delta);
        bDeform = true;
        startTime = ofGetElapsedTimeMillis();
    }
}

void ofApp::startDeform(int level){
    //    bDeform = true;
    //    startTime = ofGetElapsedTimeMillis();
}

void ofApp::actuate(){
    //ard.sendDigital(pumpPin, ARD_HIGH);
}

void ofApp::stopActuate(){
    //ard.sendDigital(pumpPin, ARD_LOW);
}

void ofApp::sendDigitalArduinoDeflation(){
    //    ard.sendDigital(3, ARD_LOW);
    //    ard.sendDigital(4, ARD_HIGH);
    //    ard.sendDigital(5, ARD_HIGH);
    //    ard.sendDigital(6, ARD_LOW);
}

void ofApp::sendDigitalArduinoInflation(){
    //    ard.sendDigital(3, ARD_HIGH);
    //    ard.sendDigital(4, ARD_LOW);
    //    ard.sendDigital(5, ARD_LOW);
    //    ard.sendDigital(6, ARD_HIGH);
}

void ofApp::sendDigitalArduinoMaintain(){
    //    ard.sendDigital(3, ARD_LOW);
    //    ard.sendDigital(4, ARD_LOW);
    //    ard.sendDigital(5, ARD_HIGH);
    //    ard.sendDigital(6, ARD_HIGH);
}

void ofApp::drawLog(){
    ofSetColor(255);
    
    if (!bSetupArduino){
        font.drawString("Connect ready...\n", valueRow[0], valueCol[1]);
    } else {
        font.drawString("Connect succeed!\n", valueRow[0], valueCol[1]);
    }
    
    font.drawString("Current propotion : " + ofToString(propotionVolume[0]) + ", Current Volume : " + ofToString(currentVolume[0]) + "ml", valueRow[2], valueCol[0]);
    smallFont.drawString("rawInputValue  :  " + ofToString(rawInputValue), valueRow[2], valueCol[0] + 20);
    smallFont.drawString("InputValue     :  " + ofToString(filterInputValue[1]), valueRow[2], valueCol[0] + 40);
    smallFont.drawString("minValue  :  " + ofToString(minValue[0]), valueRow[2], valueCol[0] + 60);
    smallFont.drawString("maxValue     :  " + ofToString(maxValue[0]), valueRow[2], valueCol[0] + 80);
    font.drawString("Current propotion : " + ofToString(propotionVolume[1]) + ", Current Volume : " + ofToString(currentVolume[1]) + "ml", valueRow[2], valueCol[1]);
    smallFont.drawString("rawOutputValue :  " + ofToString(rawOutputValue), valueRow[2], valueCol[1] + 20);
    smallFont.drawString("OutputValue    :  " + ofToString(filterOutputValue[1]), valueRow[2], valueCol[1] + 40);
    smallFont.drawString("minValue  :  " + ofToString(minValue[1]), valueRow[2], valueCol[1] + 60);
    smallFont.drawString("maxValue     :  " + ofToString(maxValue[1]), valueRow[2], valueCol[1] + 80);
    smallFont.drawString("count : " + ofToString(count) , valueRow[2], valueCol[1] + 150);
    smallFont.drawString("playCount : " + ofToString(playCount) , valueRow[2], valueCol[1] + 165);
    smallFont.drawString("recordAnalog[0] : " + ofToString(recordAnalog[0]) , valueRow[2], valueCol[1] + 180);
    smallFont.drawString("recordAnalog[1] : " + ofToString(recordAnalog[1]) , valueRow[2], valueCol[1] + 190);
    smallFont.drawString("recordAnalog[LAST] : " + ofToString(recordAnalog[RECORD_NUM - 1]) , valueRow[2], valueCol[1] + 200);
    
    smallFont.drawString("bool bRecord: " + ofToString(bRecord) , valueRow[2], valueCol[1] + 220);
    smallFont.drawString("bool bPlay: " + ofToString(bPlay) , valueRow[2], valueCol[1] + 240);
    
    smallFont.drawString("40resolution: 2.25ml = 36ms", valueRow[0], valueCol[1] + 30);
    smallFont.drawString("millis" + ofToString(milliSeconds), valueRow[0], valueCol[1] + 50);
    smallFont.drawString("--------- INPUT", valueRow[0], valueCol[1] + 80);
    ofSetColor(255, 0, 0);
    smallFont.drawString("--------- OUTPUT", valueRow[0], valueCol[1] + 100);
    
    //    std::cout << "raw: " << rawInputValue << ", oldValue: " << filterInputValue[0] << ", newValue: " << filterInputValue[1] << endl;
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
            break;
        default:
            break;
    }
}

void ofApp::record(){
    recordAnalog[count] = filterInputValue[0];
}

void ofApp::play(){
    
}

void ofApp::countClear(){
    count = 0;
    playCount = 0;
}

void ofApp::keyReleased(int key){
    switch (key) {
        case 'p':
            pompTest13 = false;
            break;
        case 'l':
            pompTest12 = false;
            break;
        case 'g':
            valveTest3 = false;
            break;
        case 'h':
            valveTest4 = false;
            break;
        case 'j':
            valveTest5 = false;
            break;
        case 'k':
            valveTest6 = false;
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
    ard.sendDigitalPinMode(2, ARD_INPUT);
    ard.sendDigitalPinMode(19, ARD_INPUT);  // pin 21 if using StandardFirmata from Arduino 0022 or older
    ard.sendAnalogPinReporting(0, ARD_ANALOG);
    ard.sendAnalogPinReporting(1, ARD_ANALOG);
    
    ard.sendAnalogPinReporting(elastPin01, ARD_ANALOG);
    ard.sendDigitalPinMode(3, ARD_OUTPUT);
    ard.sendDigitalPinMode(5, ARD_OUTPUT);
    ard.sendDigitalPinMode(6, ARD_OUTPUT);
    ard.sendDigitalPinMode(12, ARD_OUTPUT);
    ard.sendDigitalPinMode(pumpPin, ARD_OUTPUT);
    
    ard.sendDigitalPinMode(ledPin, ARD_OUTPUT);
    //ard.sendDigitalPinMode(ledPin, ARD_PWM);
    
    // attach a servo to pin D9
    // servo motors can only be attached to pin D3, D5, D6, D9, D10, or D11
    ard.sendServoAttach(9);
    
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
    plot->setRange(-100, 100);//definable range of plot
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
}

double ofApp::ceil2(double dIn, int nLen){
    double dOut;
    dOut = dIn * pow(10.0, nLen);
    dOut = (double)(int)(dOut + 0.9);
    return dOut * pow(10.0, -nLen);
}
// = (float)ceil2((double)neutral,1);
