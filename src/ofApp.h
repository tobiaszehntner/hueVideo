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

    ofVideoPlayer video;
    float ratio;
    ofRectangle screen;
    
    int sampleNum;
    int sampleSize;
    unsigned int sampleW, sampleH;
    float smoothing;
    vector<ofVec2f> samplePos;
    vector<ofColor> sampleColor;
    vector<ofColor> averageColor;
    
    ofVec2f areaCenter;
    float areaW, areaH;
    
    ofColor sample(int x, int y, int w, int h, ofPixels frame);
		
};
