#pragma once

#include "ofMain.h"
#include "ofxHTTP.h"
#include "ofxJSON.h"

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

    
    // Video
    ofVideoPlayer video;
    int videoPosX, videoPosY, videoPosW, videoPosH;
    float ratio;
    
    // Sampling
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
    
    // Hue
    std::string hueBridgeIP;
    std::string hueUser;
    
    ofx::HTTP::DefaultClient client;
    ofx::HTTP::Context context;
    ofx::HTTP::BaseResponse response;
    
    ofBuffer bodyBuffer;
    
    void hueGet(string hueGetObject);
    void hueSetup(int hueGroupNum);
    void huePutColor(int bulbNum, ofColor color, int transitionTime);
};
