#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("hueVideo");
    
    ofBackground(255);
    
    video.loadMovie("video.mov");
    video.setVolume(0);
    video.play();
    
    
    sample1.setup(0,0,50,50);
    
    

}

//--------------------------------------------------------------
void ofApp::update(){
    video.update();
    
    
    if(video.isFrameNew()) {
        
        sample1.sampling(video.getPixelsRef());
        
    }
    
    sample1.x = mouseX;
    sample1.y = mouseY;
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255);
    video.draw(0, 100, ofGetWindowWidth(), (ofGetWindowWidth()/16*9));
    
    sample1.draw();
    
    ofSetColor(sample1.averageColor);
    ofFill();    
    ofRect(10, 10, 50, 50);
    
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
