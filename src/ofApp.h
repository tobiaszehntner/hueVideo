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
    int videoPosX, videoPosY, videoPosW, videoPosH;
    ofPixels pixels;
    
    float ratio;
    int frameCount;
    
    int sampleNum;
    int sampleSize;
    int sampleRate;
    unsigned int sampleW, sampleH;
    
    vector<ofVec3f> buffer;
    vector<ofVec2f> samplePos;
    vector<ofColor> sampleColor;
    vector<ofColor> averageColor;
    
    ofVec2f areaCenter;
    float areaW, areaH;
		
};
