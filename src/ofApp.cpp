#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("hueVideo");
    
    ofBackground(255);
    
    video.loadMovie("video.mov");
    video.setVolume(0);
    video.play();
    
    videoPosX = 0;
    videoPosY = 100;
    videoPosW = ofGetWindowWidth();
    videoPosH = ofGetWindowWidth()/video.getWidth()*video.getHeight();
    
    ratio = video.getWidth()/videoPosW; // 1.6
    
    x = 0;
    y = 0;
    w = 50;
    h = 50;

}

//--------------------------------------------------------------
void ofApp::update(){
    video.update();
    
        if(mouseX >= videoPosX && mouseX <= videoPosX+videoPosW-w) {
            x = mouseX;
        }
        if(mouseY >= videoPosY && mouseY <= videoPosY+videoPosH-h) {
            y = mouseY;
        }
    
    if(video.isFrameNew()) {
        
        sampleColor = sample(x, y, w, h, video.getPixelsRef());
        
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255);
    video.draw(videoPosX, videoPosY, videoPosW, videoPosH);
    
    ofSetColor(ofColor::red);
    ofNoFill();
    
    ofRect(x,y,w,h);
    
    ofDrawBitmapString(ofToString("1"), x+5, y+15);
    
    ofSetColor(sampleColor);
    ofFill();    
    ofRect(10, 10, 50, 50);
    
}

ofColor ofApp::sample(int x, int y, int w, int h, ofPixels frame) {
    
    x = (x-videoPosX)*ratio;
    y = (y-videoPosY)*ratio;
    w = w*ratio;
    h = h*ratio;
    
    ofColor averageColor;
    
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
    
    
    return averageColor;
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
