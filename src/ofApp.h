#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxHistoryPlot.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    //Input
    int rawInputValue = 0;
    int rawOutputValue = 0;
    int mapInputValue = 0;
    int filterInputValue[2] = {0};
    int filterOutputValue[2] = {0};
    float a = 0.9;
    
    //output
    bool bDeform = false;
    int delta = 0;
    int absDelta = 0;
    int oldDelta = 0;
    int _deltaDelta = 0;
    bool bPolarity = false;
    float startTime = 0;
    float milliSeconds = 0;
    
    void actuate();
    void stopActuate();
    void coolDown();
    void checktime();
    int checkDelta(int x, int y);
    int absoluteDelta(int x);
    int deltaDelta(int x, int y);
    void sendDigitalArduinoDeflation();
    void sendDigitalArduinoInflation();
    void sendDigitalArduinoMaintain();
    void manipulateElastOn();
    void manipulateElastOff();
    
    static const int ANALOG_NUM = 2;
    static const int MIDDLE_VALUE = 500;
    int currentVolume[ANALOG_NUM] = {0};
    int propotionVolume[ANALOG_NUM] = {0};
    int minValue[ANALOG_NUM] = {MIDDLE_VALUE};
    int maxValue[ANALOG_NUM] = {MIDDLE_VALUE};
    int manipulateInput, manipulateOutput;
    
    static const int DEFORM_RESOLUSION = 60;
    
    //ofxgui
    ofxPanel gui;
    ofxFloatSlider operateMinValueA0;
    ofxFloatSlider operateMaxValueA0;
    ofxFloatSlider operateMinValueA1;
    ofxFloatSlider operateMaxValueA1;
    
    //plot
    ofxHistoryPlot * plot;
    ofxHistoryPlot * plot2;
    ofxHistoryPlot * plot3;
    void setupHistoryPlot();
    float currentFrameRate;
    bool bDrawPlot = false;

    //graphic
    float width = ofGetWidth()/2;
    float height = ofGetHeight()/2;
    ofTrueTypeFont      font;
    ofTrueTypeFont      smallFont;
    float valueRow[3] = {20, 20 + width * 2/3, width * 4/3 - 50};
    float valueCol[3] = {30, 60 * 2 + 30, 60 * 3 + 30};
    void drawLog();
    
    //test
    bool pompTest13 = false;
    bool pompTest12 = false;
    bool valveTest3 = false;
    bool valveTest4 = false;
    bool valveTest5 = false;
    bool valveTest6 = false;
    void ledTest();
    bool bLed = false;
    
    //record
    static const int FRAMERATE_NUM = 30; //1sec
    static const int RECORD_NUM = FRAMERATE_NUM * 6;
    int recordAnalog[RECORD_NUM] = {0};
    int count = 0;
    int loopCount = 0;
    bool bRecord = false;
    void record();
    void countClear();
    
    //play
    int playCount = 0;
    bool bPlay = false;
    void play();
    
    
    ofFile myTextFile;
    ofFile myReadFile;
    bool bWrite = false;
    
private:
    ofArduino ard; //arduino
    bool bSetupArduino;
    
    void initArduino();
    void setupArduino(const int & version);
    void digitalPinChanged(const int & pinNum);
    void analogPinChanged(const int & pinNum);
    void updateArduino();
    
    string buttonState;
    string potValue;
    
    double ceil2(double dIn, int nLen);
    
    int ledPin = 2;
    int pumpPin01 = 10;
    int pumpPin02 = 11;
    int elastPin01 = 4;
    int elastPin02 = 5;
    int valvePin[8] = {2,3,4,5,6,7,8,9};
};
