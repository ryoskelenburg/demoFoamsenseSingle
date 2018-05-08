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
    
    ofTrueTypeFont      font;
    ofTrueTypeFont      smallFont;
    
    float width = ofGetWidth()/2;
    float height = ofGetHeight()/2;
    
    int rawInputValue = 0;
    int rawOutputValue = 0;
    int filterInputValue[2] = {0};
    int filterOutputValue[2] = {0};
    float a = 0.9;
    
    float currentFrameRate;
    double ceil2(double dIn, int nLen);
    
    void drawPressSponge(int _value);
    void defineSponge(int _analog, int _define);
    
    ofArduino ard;
    bool bSetupArduino;
    
    ofxPanel gui;
    ofxFloatSlider minValue;
    ofxFloatSlider neutral;
    ofxFloatSlider defineDelta;
    
    ofxHistoryPlot * plot;
    ofxHistoryPlot * plot2;
    void setupHistoryPlot();
    
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
    
    string buttonState;
    string potValue;
};
