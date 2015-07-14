#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("hueVideo");
    
    ofBackground(255);
    
    video.loadMovie("video.mov");
    //video.play();
    
    sample1.setup();
    

}

//--------------------------------------------------------------
void ofApp::update(){
    video.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    video.draw(0, 100, ofGetWindowWidth(), (ofGetWindowWidth()/16*9));
    sample1.draw();
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
