#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("hueVideo");
    ofBackground(255);
    
    x = 0;
    y = 0;
    w = 50;
    h = 50;
    
    video.loadMovie("video.mov");
    video.setVolume(0);
    video.play();
    
    videoPosX = 0;
    videoPosY = 100;
    videoPosW = ofGetWindowWidth();
    videoPosH = ofGetWindowWidth()/video.getWidth()*video.getHeight();
    
    ratio = video.getWidth()/videoPosW; // 1.6
    
    numSamples = 10;
    
    areaCenter.x = videoPosX+videoPosW/2;
    areaCenter.y = videoPosY+videoPosH/2;
    areaW = videoPosW;
    areaH = h;
    
    isFlippedX = false;
    isFlippedY = false;

}

//--------------------------------------------------------------
void ofApp::update(){

    
    samplePos.clear();
    int sampleX;
    int sampleY;
    
    for (int i = 0; i < numSamples; i++) {
        
        if(numSamples > 1) {
            if(isFlippedX) {
                sampleX = areaCenter.x + (areaW/2) - ( ((areaW-w) / (numSamples-1)) * i) -w;
                cout << sampleX << endl;
            } else {
                sampleX = areaCenter.x - (areaW/2) + ( ((areaW-w) / (numSamples-1)) * i);
            }
        } else {
            sampleX = areaCenter.x - w/2;
        }
        
        if(numSamples > 1) {
            if(isFlippedY) {
                sampleY = areaCenter.y + (areaH/2) - ( ((areaH-h) / (numSamples-1)) * i) -h;
            } else {
                sampleY = areaCenter.y - (areaH/2) + ( ((areaH-h) / (numSamples-1)) * i);
            }
        } else {
            sampleY = areaCenter.y - h/2;
        }

        ofVec2f loc(sampleX, sampleY);

        samplePos.push_back(loc);
        
    }
    
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
    
    for (int i = 0; i < numSamples; i++) {
        ofSetColor(ofColor::green);
        ofNoFill();
        ofRect(samplePos[i].x,samplePos[i].y,w,h);
        ofDrawBitmapString(ofToString(i+1), samplePos[i].x+5, samplePos[i].y+15);
    }
    
    ofSetColor(ofColor::red);
    ofNoFill();
    ofRect(x,y,w,h);
    
    ofDrawBitmapString(ofToString("1"), x+5, y+15);
    
    ofSetColor(sampleColor);
    ofFill();    
    ofRect(10, 10, 50, 50);
    
    ofSetColor(0);
    ofDrawBitmapString("X/Y Distr [c-v/n-m] = " + ofToString(areaW-w) + "/" + ofToString(areaH-h) + "\n"
                       "Center [arrowKeys]  = " + ofToString(areaCenter.x) + "/" + ofToString(areaCenter.y)
                       , 10, 80);
    ofDrawBitmapString("Flip X/Y [f-g] = " + ofToString(isFlippedX) + "/" + ofToString(isFlippedY) + "\n"
                       "Samples [k-l]  = " + ofToString(numSamples)
                       , 400, 80);
    
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
    
    if (key == OF_KEY_DOWN){
        if(areaCenter.y < (videoPosY + videoPosH - (h/2))) {
            areaCenter.y += 5;
        }
    }
    if (key == OF_KEY_UP){
        if(areaCenter.y > (videoPosY + (h/2))) {
            areaCenter.y -= 5;;
        }
    }
    if (key == OF_KEY_RIGHT){
        if(areaCenter.x < (videoPosX + videoPosW - (w/2))) {
            areaCenter.x += 5;;
        }
    }
    if (key == OF_KEY_LEFT){
        if(areaCenter.x > (videoPosX + (w/2))) {
            areaCenter.x -= 5;;
        }
    }
    
    if (key == 'n'){
        if(areaW > w) {
            areaW -= 5;;
        }
    }
    if (key == 'm'){
        if(areaW < videoPosW) {
            areaW += 5;;
        }
    }
    if (key == 'c'){
        if(areaH > h) {
            areaH -= 5;;
        }
    }
    if (key == 'v'){
        if(areaH < videoPosH) {
            areaH += 5;;
        }
    }
    
    if (key == 'f'){
        if(isFlippedX) {
            isFlippedX = false;
        } else {
            isFlippedX = true;
        }
    }
    if (key == 'g'){
        if(isFlippedY) {
            isFlippedY = false;
        } else {
            isFlippedY = true;
        }
    }
    
    if (key == 'k'){
        if(numSamples > 1) {
            numSamples--;
        }
    }
    if (key == 'l'){
        if(numSamples < 10) {
            numSamples++;
        }
    }
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
