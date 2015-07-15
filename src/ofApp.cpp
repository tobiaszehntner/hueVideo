#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("hueVideo");
    
    ofBackground(255);
    
    video.loadMovie("video.mov");
    video.setVolume(0);
    video.play();
    videoFrame.allocate(
                        video.getWidth(),
                        video.getHeight(),
                        OF_IMAGE_COLOR);
    
    sample1.setup();
    
    

}

//--------------------------------------------------------------
void ofApp::update(){
    video.update();
    
    
    if(video.isFrameNew()) {
        
        
        videoFrame.setFromPixels(
                                 video.getPixels(),
                                 video.getWidth(),
                                 video.getHeight(),
                                 OF_IMAGE_COLOR);
        
        sample1.sampling(videoFrame);
        
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255);
    video.draw(0, 100, ofGetWindowWidth(), (ofGetWindowWidth()/16*9));
    
    sample1.draw();
    
    ofSetColor(sample1.color_sample);
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
