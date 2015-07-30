#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("hueVideo");
    
    ofBackground(255);
    
    video.loadMovie("video.mov");
    video.setVolume(0);
    video.play();
    
    
    sample1.setup(0,0,50,50);
    
    videoPosX = 0;
    videoPosY = 100;
    videoPosW = ofGetWindowWidth();
    videoPosH = ofGetWindowWidth()/video.getWidth()*video.getHeight();
    
    ratio = video.getWidth()/videoPosW; // 1.6

}

//--------------------------------------------------------------
void ofApp::update(){
    video.update();
    
    
    if(video.isFrameNew()) {
        
        sample1.sampling(video.getPixelsRef());
        
    }
    
    if(mouseX >= videoPosX && mouseX <= (videoPosX+videoPosW-sample1.w/ratio)) {
        sample1.x = (mouseX-videoPosX)*ratio;
    }
    if(mouseY >= videoPosY && mouseY <= videoPosY+videoPosH-sample1.h/ratio) {
        sample1.y = (mouseY-videoPosY)*ratio;
    }    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255);
    video.draw(videoPosX, videoPosY, videoPosW, videoPosH);
    
    cout << "mouseX = " << mouseX << " sample1.x = " << sample1.x << " mouseY = " << mouseY << " sample1.y " << sample1.y << endl;
    
    ofSetColor(ofColor::red);
    ofNoFill();
    
    ofRect(
           ((sample1.x/ratio)+videoPosX),
           ((sample1.y/ratio)+videoPosY),
           (sample1.w/ratio),
           (sample1.h/ratio)
           );
    
    ofDrawBitmapString(ofToString(sample1.number), ((sample1.x/ratio)+videoPosX)+5, ((sample1.y/ratio)+videoPosY)+15);
    
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
