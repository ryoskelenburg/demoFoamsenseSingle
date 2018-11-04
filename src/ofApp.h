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
    
    /*-----input-----*/
    static const int ANALOG_NUM = 3;
    static const int UNIT_NUM = 2;
    static const int TOTAL_ANALOG_NUM = ANALOG_NUM * UNIT_NUM;

    int analogPinNum[TOTAL_ANALOG_NUM] = {0};
    int outputGPIO = ANALOG_NUM * 2;
    int analogNumStart[UNIT_NUM] = {0,3};
    
    /*-----output-----*/
    //valve 14~19
    int valveNumStart = 14;
    int supplyValve[ANALOG_NUM] = {0};
    int vacuumValve[ANALOG_NUM] = {0};
    
    //pump 3,5,6,9,10,11
    int pumpNumStart = 14;
    int supplyPump[ANALOG_NUM] = {3, 6, 10};
    int vacuumPump[ANALOG_NUM] = {5, 9, 11};
    
    int pwm = 150;
    
    /*-----setup-----*/
    static const int DEFORM_RESOLUSION = 100;
    static const int FRAMERATE_NUM = 20; //1sec
    static const int RECORD_NUM = FRAMERATE_NUM * 10; //nSec
    
    static const int MIDDLE_VALUE = 600;
    int THRESHOLD_VALUE = 100;
    int minValue[TOTAL_ANALOG_NUM] = {MIDDLE_VALUE};
    int maxValue[TOTAL_ANALOG_NUM] = {MIDDLE_VALUE};
    int manipulateInput, manipulateOutput;
    
/*-----------------------------------------------------------------*/
    
private:
    void workRecord();
    void workPlay();
    void workRealtime();
    
    void ffJudge(int number);
    void ffOutput (int number, int input);
    
    bool bRecord = false;
    bool bPlay = false;
    bool bReal = false;
    
    void checkWrite();
    void useImportData();
    void captureScreen();
    
    //output
    bool bDeform[TOTAL_ANALOG_NUM] = {false};
    bool bPolarity[TOTAL_ANALOG_NUM] = {false};
    int forClosedLoopDelta[ANALOG_NUM] = {0};
    int oldDelta = 0;
    float startTime = 0;
    float milliSeconds = 0;
    
    void actuate();
    void stopActuate(int number);
    void coolDown();
    void checktime();
    int deltaDelta(int x, int y);
    
    void sendDigitalArduinoSupply(int number);
    void sendDigitalArduinoVacuum(int number);
    void sendDigitalArduinoClose(int number);
    void sendDigitalArduinoExhaust(int number);
    
    void checkDigital(int number);
    void clearDigital();
    void checkPWM(int number, int PWM);
    
    int count = 0;
    void countClear();
    void actuate(int number, int deltaThreshold);
    
    //------feedforward---
    
    int delta[TOTAL_ANALOG_NUM] = {0};
    int absDelta[TOTAL_ANALOG_NUM] = {0};
    
    int playCount = 0;
    
    ofArduino ard; //arduino
    bool bSetupArduino;
    void initArduino();
    void setupArduino(const int & version);
    void digitalPinChanged(const int & pinNum);
    void analogPinChanged(const int & pinNum);
    void updateArduino();
    
    string buttonState;
    string potValue;
    
    int filteredValue[TOTAL_ANALOG_NUM][2] = {0};
    int propVol[TOTAL_ANALOG_NUM] = {0};
    int recordPropVol[TOTAL_ANALOG_NUM][RECORD_NUM] = {0};
    
    void adjustAnalog(int pin, int order);
    void updateVal(int order);
    
    double ceil2(double dIn, int nLen);
    float a = 0.9;
    
    bool bRelay = false;
    
    //graphics
    float width = ofGetWidth()/2;
    float height = ofGetHeight()/2;
    ofTrueTypeFont      font;
    ofTrueTypeFont      smallFont;
    float valueRow[3] = {20, width * 2 - 200, width * 2 - 350};
    float valueCol[3] = {250, 400, 550};
    
    float contentWidth = 200;
    float row[2][3] = {{20, 20 + contentWidth, 20 + contentWidth * 2}, {width, width + contentWidth, width + contentWidth * 2}};
    int column = 700;
    void drawLog();
    void drawLogContents(int number, int row, int order);
    void drawArrayData(int number);
    
    ofFile recordFile;
    ofFile feedbackFile;
    bool bRecordWrite = false;
    bool bPlatWrite = false;

    //------feedback-------
    
    
    ofImage screen;
    
    //ofxgui
    ofxPanel gui;
    ofxFloatSlider operateMax[TOTAL_ANALOG_NUM];
    ofxFloatSlider operateMin[TOTAL_ANALOG_NUM];
    
    //plot
    ofxHistoryPlot * plot[TOTAL_ANALOG_NUM];
    ofxHistoryPlot * recordPlot[TOTAL_ANALOG_NUM];
    void setupHistoryPlot(int number);

    float currentFrameRate;
    bool bDrawPlot = false;
    
    bool bActive[3] = {false};
};
