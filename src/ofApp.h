#pragma once

#include "ofMain.h"

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    ofColor sample(int x, int y, int w, int h, ofPixels frame);
    
    ofVideoPlayer video;
    ofPixels pixels;
    ofColor sampleColor;
    
    int numSamples;
    unsigned int x, y, w, h;
    int videoPosX, videoPosY, videoPosW, videoPosH;
    float ratio;
    
    ofVec2f areaCenter;
    int areaW, areaH;
    vector<ofVec2f> samplePos;
    bool isFlippedX;
    bool isFlippedY;
		
};
