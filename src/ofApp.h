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
    int delta;
    bool bPolarity = false;
    int startTime;
    float milliSeconds = 0;
    
    void startDeform(int level);
    void actuate();
    void stopActuate();
    void checktime();
    void controlPomp(int x, int y);
    void sendDigitalArduinoDeflation();
    void sendDigitalArduinoInflation();
    void sendDigitalArduinoMaintain();
    
    //0:Input, 1:Output
    int currentVolume[2] = {0};
    int propotionVolume[2] = {0};
    int minValue[2] = {0};
    int maxValue[2] = {0};
    int manipulateInput, manipulateOutput;
    
    //ofxgui
    ofxPanel gui;
    ofxFloatSlider operateMinValue;
    ofxFloatSlider neutral;
    ofxFloatSlider defineDelta;
    
    //plot
    ofxHistoryPlot * plot;
    ofxHistoryPlot * plot2;
    void setupHistoryPlot();
    float currentFrameRate;

    //graphic
    float width = ofGetWidth()/2;
    float height = ofGetHeight()/2;
    ofTrueTypeFont      font;
    ofTrueTypeFont      smallFont;
    float valueRow[3] = {20, 20 + width * 2/3, 20 + width * 4/3};
    float valueCol[3] = {100, 100 * 2, 100 * 3};
    void drawLog();
    
private:
    //arduino
    ofArduino ard;
    bool bSetupArduino;
    
    void initArduino();
    void setupArduino(const int & version);
    void digitalPinChanged(const int & pinNum);
    void analogPinChanged(const int & pinNum);
    void updateArduino();
    
    string buttonState;
    string potValue;
    
    double ceil2(double dIn, int nLen);
};
