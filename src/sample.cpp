

#include "sample.h"
#include "ofMain.h"
#include "ofApp.h"

Sample::Sample(){
}

void Sample::setup(int sampleX, int sampleY, int sampleW, int sampleH){
    x = sampleX;
    y = sampleY;
    w = sampleW;
    h = sampleH;
    
    number = 1;
    
    color_wireframe.set(0);
}


void Sample::update(){
    
}

void Sample::draw(){
    
    ofNoFill();
    ofRect(x, y, w, h);
    ofDrawBitmapString(ofToString(number), x+5, y+15);
    
}

void Sample::sampling(ofPixels frame) {
    
    int rSum = 0;
    int gSum = 0;
    int bSum = 0;
    
    
    for(int i = x; i < (x+w); i++) {
        
        for(int j = y; j < (y+h); j++) {
            
            ofColor pixelColor = frame.getColor(i, j);
            rSum += pixelColor.r;
            gSum += pixelColor.g;
            bSum += pixelColor.b;
        }
    }
    
    int samples = w * h;
    
    averageColor.r = rSum / samples;
    averageColor.g = gSum / samples;
    averageColor.b = bSum / samples;
    
    
}
