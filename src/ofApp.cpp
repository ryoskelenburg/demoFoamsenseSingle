#include "ofApp.h"

//--------------------------------------------------------------

void ofApp::setup(){
    ofBackground(20, 20, 20);
    
    ofSetVerticalSync(true);
    ofSetFrameRate(20);
    font.load("franklinGothic.otf", 20);
    smallFont.load("franklinGothic.otf", 14);
    
    initArduino();
    
    gui.setup();
    gui.add(minValue.setup("minValue",1, 0, 255));
    gui.add(neutral.setup("neutral",500,0,1023));
    gui.add(defineDelta.setup("delta",3,0,10));
    
    setupHistoryPlot();
}

void ofApp::update(){
    currentFrameRate = ofGetFrameRate();
    
    updateArduino();
    rawInputValue = ard.getAnalog(0);
    rawOutputValue = ard.getAnalog(1);
    filterInputValue[1] = a * filterInputValue[0] + (1-a) * rawInputValue;
    filterOutputValue[1] = a * filterOutputValue[0] + (1-a) * rawOutputValue;
    
    plot->update(filterInputValue[1]);
    
    //ofxguiのfloatをintへ
    neutral = (float)ceil2((double)neutral,1);

}


void ofApp::draw(){
    
    ofSetColor(255);
    if (!bSetupArduino){
        font.drawString("arduino not ready...\n", 600, 40);
    } else {
        font.drawString(potValue, 600,40);
        
        std::cout << "raw: " << rawInputValue << ", oldValue: " << filterInputValue[0] << ", newValue: " << filterInputValue[1] << endl;
    }
    
    font.drawString("rawInputValue          :=  " + ofToString(rawInputValue), 600, 60);
    font.drawString("filterInputValue[0]          :=  " + ofToString(filterInputValue[0]), 600, 80);
    font.drawString("filterInputValue[1]          :=  " + ofToString(filterInputValue[1]), 600, 100);
    
    
    
    gui.draw();
    
    plot->draw(0, 300, ofGetWidth(), 300);
    plot2->draw(0, 300, ofGetWidth(), 300);
    filterInputValue[0] = filterInputValue[1];//値の更新
}

//--------------------------------------------------------------

double ofApp::ceil2(double dIn, int nLen){
    double dOut;
    dOut = dIn * pow(10.0, nLen);
    dOut = (double)(int)(dOut + 0.9);
    return dOut * pow(10.0, -nLen);
}

void ofApp::defineSponge(int _analogV,int _define){
    if (_define < _analogV + 1) {
        _define = _analogV;
    }
}

//--------------------------------------------------------------

void ofApp::initArduino(){
    buttonState = "digital pin:";
    potValue = "analog pin:";
    ard.connect("/dev/cu.usbmodem1411", 57600);
    
    ofAddListener(ard.EInitialized, this, &ofApp::setupArduino);
    bSetupArduino    = false;
    
}

void ofApp::setupArduino(const int & version) {
    
    // remove listener because we don't need it anymore
    ofRemoveListener(ard.EInitialized, this, &ofApp::setupArduino);
    
    // it is now safe to send commands to the Arduino
    bSetupArduino = true;
    
    // print firmware name and version to the console
    ofLogNotice() << ard.getFirmwareName();
    ofLogNotice() << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion();
    
    // Note: pins filterInputValue - A5 can be used as digital input and output.
    // Refer to them as pins 14 - 19 if using StandardFirmata from Arduino 1.0.
    // If using Arduino 0022 or older, then use 16 - 21.
    // Firmata pin numbering changed in version 2.3 (which is included in Arduino 1.0)
    
    // set pins D2 and A5 to digital input
    ard.sendDigitalPinMode(2, ARD_INPUT);
    ard.sendDigitalPinMode(19, ARD_INPUT);  // pin 21 if using StandardFirmata from Arduino 0022 or older
    
    // set pin filterInputValue to analog input
    ard.sendAnalogPinReporting(0, ARD_ANALOG);
    ard.sendAnalogPinReporting(1, ARD_ANALOG);
    
    
    // set pin from D3 to D6 to digital output
    ard.sendDigitalPinMode(3, ARD_OUTPUT);
    ard.sendDigitalPinMode(4, ARD_OUTPUT);
    ard.sendDigitalPinMode(5, ARD_OUTPUT);
    ard.sendDigitalPinMode(6, ARD_OUTPUT);
    
    // set pin D13 as digital output
    ard.sendDigitalPinMode(13, ARD_OUTPUT);
    
    // set pin A4 as digital output
    ard.sendDigitalPinMode(18, ARD_OUTPUT);  // pin 20 if using StandardFirmata from Arduino 0022 or older
    
    // set pin D11 as PWM (analog output)
    ard.sendDigitalPinMode(11, ARD_PWM);
    
    // attach a servo to pin D9
    // servo motors can only be attached to pin D3, D5, D6, D9, D10, or D11
    ard.sendServoAttach(9);
    
    // Listen for changes on the digital and analog pins
    ofAddListener(ard.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
    ofAddListener(ard.EAnalogPinChanged, this, &ofApp::analogPinChanged);
}

void ofApp::updateArduino(){
    ard.update();
    if (bSetupArduino) {
        ard.sendPwm(11, (int)(128 + 128 * sin(ofGetElapsedTimef())));   // pwm...
    }
}

void ofApp::digitalPinChanged(const int & pinNum) {
    // do something with the digital input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    buttonState = "digital pin: " + ofToString(pinNum) + " = " + ofToString(ard.getDigital(pinNum));
}

// analog pin event handler, called whenever an analog pin value has changed
void ofApp::analogPinChanged(const int & pinNum) {
    // do something with the analog input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    potValue = "analog pin: " + ofToString(pinNum) + " = " + ofToString(ard.getAnalog(pinNum));
    
    
    //settingValue = "minValue: " + ofToString(minValue) + "\n" + "maxValue: " + ofToString(maxValue);
}


void ofApp::setupHistoryPlot(){
    plot = new ofxHistoryPlot(&currentFrameRate, "timeline", ofGetWidth(), false);
    plot->setBackgroundColor(ofColor(0,0,0,0));
    //plot->setShowNumericalInfo(true);
    plot->setRange(0, 1023);//definable range of plot
    plot->setRespectBorders(true);
    plot->setLineWidth(1);
    plot->setCropToRect(true);
    plot->setDrawGrid(true);
    plot->setGridUnit(16);
    plot->setGridColor(ofColor(100));
    plot->setShowSmoothedCurve(false);
    plot->setSmoothFilter(0.1); //smooth filter strength
    
    plot2 = new ofxHistoryPlot(&currentFrameRate, "hogehogehoge", ofGetWidth(), true);
    plot2->setBackgroundColor(ofColor(0,0,0,0));
    plot2->setColor( ofColor(255,0,255) );
    plot2->setRange(0, 1023);//definable range of plot
    plot2->setRespectBorders(true);
    plot2->setLineWidth(1);
    plot2->setCropToRect(true);
    plot2->setDrawGrid(true);
    plot2->setGridUnit(16);
    plot2->setShowSmoothedCurve(false);
    plot2->setSmoothFilter(0.1); //smooth filter strength
}

void ofApp::keyPressed(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
//        case 'o':
//            ard.sendDigital(3, ARD_HIGH);
//            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
//    switch (key) {
//        case 'o':
//            ard.sendDigital(3, ARD_LOW);
//            break;
//    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    ard.sendDigital(13, ARD_HIGH);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    ard.sendDigital(13, ARD_LOW);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}

