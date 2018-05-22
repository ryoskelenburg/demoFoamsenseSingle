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
    
    void startPush(int level);
    void stopPush();
    void push();
    void checktime();
    
    ofTrueTypeFont      font;
    ofTrueTypeFont      smallFont;
    
    void drawLog();
    
    float width = ofGetWidth()/2;
    float height = ofGetHeight()/2;
    
    int rawInputValue = 0;
    int rawOutputValue = 0;
    int mapInputValue = 0;
    int filterInputValue[2] = {0};
    int filterOutputValue[2] = {0};
    float a = 0.9;
    
    //0:Input, 1:Output
    int currentVolume[2] = {0};
    int propotionVolume[2] = {0};
    int minValue[2] = {0};
    int maxValue[2] = {0};
    int manipulateInput, manipulateOutput;
    
    float milliSeconds = 0;
    
    float currentFrameRate;
    double ceil2(double dIn, int nLen);
    
    void drawPressSponge(int _value);
    void defineSponge(int _analog, int _define);
    
    ofArduino ard;
    bool bSetupArduino;
    
    ofxPanel gui;
    ofxFloatSlider operateMinValue;
    ofxFloatSlider neutral;
    ofxFloatSlider defineDelta;
    
    ofxHistoryPlot * plot;
    ofxHistoryPlot * plot2;
    void setupHistoryPlot();
    
    int valueRow1 = 20;
    int valueRow3 = width * 4/3 + 20;
    int inputValueY = 80;
    int outputValueY = 200;
    
    //control---------------------------------
    int delta;
    int absDelta;
    //int defineDelta = 3;//***変化量の閾値***
    
    //int neutral = 750;//***ニュートラルの値***
    int defineNeutral = 20;//***ニュートラルの閾値***
    int identNeutral;
    
    int condition;
    
    //------------------------------------------

private:
    void initArduino();
    void setupArduino(const int & version);
    void digitalPinChanged(const int & pinNum);
    void analogPinChanged(const int & pinNum);
    void updateArduino();
    //void sendDigitalArduino01();
    void sendDigitalArduino02();
    void sendDigitalArduino03();
    void sendDigitalArduino04();
    void controlPomp(int x, int y);
    
    string buttonState;
    string potValue;
    
    bool bPushing = false;
    int pushLevel;
    int startTime;
};
