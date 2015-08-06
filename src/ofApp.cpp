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
    
    sampleNum = 9;
    sampleSize = 50; // pixels
    sampleW = sampleSize;
    sampleH = sampleSize;
    
    areaCenter.x = videoPosX+videoPosW/2;
    areaCenter.y = videoPosY+videoPosH/2;
    areaW = videoPosW;
    areaH = sampleH;
    
    smoothing = 0.8; // 0-1, 0 = no smoothing
}

//--------------------------------------------------------------
void ofApp::update(){

    sampleW = sampleSize;
    sampleH = sampleSize;

    samplePos.clear();
    
    for (int i = 0; i < sampleNum; i++) {
    
        int sampleX;
        int sampleY;
        
        if(sampleNum > 1) {
            sampleX = areaCenter.x - (areaW/2) + ( ((areaW-sampleW) / (sampleNum-1)) * i);
        } else {
            sampleX = areaCenter.x - sampleW/2;
        }
        
        if(sampleNum > 1) {
            sampleY = areaCenter.y - (areaH/2) + ( ((areaH-sampleH) / (sampleNum-1)) * i);
        } else {
            sampleY = areaCenter.y - sampleH/2;
        }

        ofVec2f loc(sampleX, sampleY);

        samplePos.push_back(loc);
        
    }
    
    video.update();
    
    if(video.isFrameNew()) {

        sampleColor.clear();

        for (int i = 0; i < sampleNum; i++) {
            ofColor color = sample(samplePos[i].x,samplePos[i].y,sampleW,sampleH, video.getPixelsRef());
            sampleColor.push_back(color);
        }

        
        if(averageColor.size() == 0) {
            for (int i = 0; i < sampleNum; i++) {
                averageColor.push_back(sampleColor[i]);
            }
        } else {
            for (int i = 0; i < sampleNum; i++) {
                averageColor[i].r = smoothing * averageColor[i].r + (1-smoothing) * sampleColor[i].r;
                averageColor[i].g = smoothing * averageColor[i].g + (1-smoothing) * sampleColor[i].g;
                averageColor[i].b = smoothing * averageColor[i].b + (1-smoothing) * sampleColor[i].b;
            }
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255);
    video.draw(videoPosX, videoPosY, videoPosW, videoPosH);

    for (int i = 0; i < sampleNum; i++) {
        ofSetColor(ofColor::green);
        ofNoFill();
        ofRect(samplePos[i].x,samplePos[i].y,sampleW,sampleH);
        ofDrawBitmapString(ofToString(i+1), samplePos[i].x+5, samplePos[i].y+15);
    }
    
    for (int i = 0; i < sampleNum; i++) {
        ofSetColor(averageColor[i]);
        ofFill();    
        ofRect(10 + (i*60), 10, 50, 50);
        ofSetColor(255);
        ofDrawBitmapString(ofToString(i+1), 15 + (i*60), 25);
    }
    
    
    ofSetColor(0);
    ofDrawBitmapString("X/Y Distr [c-v/n-m] = " + ofToString(areaW-sampleW) + "/" + ofToString(areaH-sampleH) + "\n"
                       "Center [arrowKeys]  = " + ofToString(areaCenter.x) + "/" + ofToString(areaCenter.y)
                       , 10, 80);
    ofDrawBitmapString("Samples [k-l]     = " + ofToString(sampleNum) + "\n" +
                       "Sample size [a-s] = " + ofToString(sampleSize)
                       , 300, 80);
    ofDrawBitmapString("Smoothing [q-w] = " + ofToString(smoothing, 2) + "\n" +
                       "Sample size [a-s] = " + ofToString(sampleSize)
                       , 500, 80);
    
}

//--------------------------------------------------------------
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
        if(areaCenter.y < (videoPosY + videoPosH - (sampleH/2))) {
            areaCenter.y += 5;
        }
    }
    if (key == OF_KEY_UP){
        if(areaCenter.y > (videoPosY + (sampleH/2))) {
            areaCenter.y -= 5;;
        }
    }
    if (key == OF_KEY_RIGHT){
        if(areaCenter.x < (videoPosX + videoPosW - (sampleW/2))) {
            areaCenter.x += 5;;
        }
    }
    if (key == OF_KEY_LEFT){
        if(areaCenter.x > (videoPosX + (sampleW/2))) {
            areaCenter.x -= 5;;
        }
    }
    
    if (key == 'n'){
        int num = (videoPosW*-1)+(2*sampleW);
        if(areaW > num) {
            areaW -= 5;;
        }
    }
    if (key == 'm'){
        if(areaW < videoPosW) {
            areaW += 5;;
        }
    }
    
    if (key == 'c'){
        int num = (videoPosH*-1)+(2*sampleH);
        if(areaH > num) {
            areaH -= 5;;
        }
    }
    if (key == 'v'){
        if(areaH < videoPosH) {
            areaH += 5;;
        }
    }
    
    if (key == 'k'){
        if(sampleNum > 1) {
            sampleNum--;
        }
    }
    if (key == 'l'){
        if(sampleNum < 10) {
            sampleNum++;
        }
    }

    if (key == 'a'){
        if(sampleSize > 20) {
            sampleSize -= 10;
        }
    }
    if (key == 's'){
        if(sampleNum < 500) {
            sampleSize += 10;
        }
    }
    
    if (key == 'q'){
        if(smoothing < 0.98) {
            smoothing += 0.01;
        }
    }
    if (key == 'w'){
        if(smoothing > 0.02) {
            smoothing -= 0.01;
        } else {
            smoothing = 0;
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
