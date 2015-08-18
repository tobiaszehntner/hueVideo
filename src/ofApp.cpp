#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("hueVideo");

    ofBackground(255);
    
    video.loadMovie("video.mov");
    video.setVolume(0);
    video.play();
    
    screen.x = 0;
    screen.y = 100;
    screen.width = ofGetWindowWidth();
    screen.height = ofGetWindowWidth()/video.getWidth()*video.getHeight();
    
    ratio = video.getWidth()/screen.width; // 2.4
    
    sampleNum = 9;
    sampleSize = 50; // pixels
    sampleGlobal.width = sampleSize;
    sampleGlobal.height = sampleSize;
    
    samplingAreaCenter.x = screen.x+screen.width/2;
    samplingAreaCenter.y = screen.y+screen.height/2;
    samplingArea.width = screen.width;
    samplingArea.height = sampleGlobal.height;
    samplingArea.x = screen.x;
    samplingArea.y = screen.y+screen.height/2-sampleGlobal.height/2;
    
    smoothing = 0.8; // 0-1, 0 = no smoothing
}

//--------------------------------------------------------------
void ofApp::update(){

    sampleGlobal.width = sampleSize;
    sampleGlobal.height = sampleSize;
    
    samples.clear();

    for (int i = 0; i < sampleNum; i++) {
        
        ofRectangle tempSample;
        
        if(sampleNum > 1) {
            tempSample.setFromCenter(
                                     ((samplingArea.x+sampleGlobal.width/2) + ((samplingArea.width-sampleGlobal.width) / (sampleNum-1)) * i),
                                     ((samplingArea.y+sampleGlobal.height/2) + ((samplingArea.height-sampleGlobal.height) / (sampleNum-1)) * i),
                                     sampleGlobal.width,
                                     sampleGlobal.height);
        } else {
            tempSample.setFromCenter(samplingArea.getCenter(), sampleGlobal.width, sampleGlobal.height);
        }
        
        samples.push_back(tempSample);
        
    }
    
    video.update();
    
    if(video.isFrameNew()) {

        sampleColor.clear();

        for (int i = 0; i < sampleNum; i++) {
            ofColor color = getAverageColor(samples[i], video.getPixelsRef());
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
    video.draw(screen);
    
    for (int i = 0; i < sampleNum; i++) {
        ofSetColor(ofColor::green);
        ofNoFill();
        ofRect(samples[i]);
        ofDrawBitmapString(ofToString(i+1), samples[i].x+5, samples[i].y+15);
    }
    
    for (int i = 0; i < sampleNum; i++) {
        ofSetColor(averageColor[i]);
        ofFill();    
        ofRect(10 + (i*60), 10, 50, 50);
        ofSetColor(255);
        ofDrawBitmapString(ofToString(i+1), 15 + (i*60), 25);
    }
    
    ofSetColor(ofColor::red);
    ofLine(samplingArea.x, samplingArea.y, samplingArea.x+samplingArea.width, samplingArea.y+samplingArea.height);
    
    
    ofSetColor(255);
    ofDrawBitmapString("[c-v/n-m] X/Y Distr   = " + ofToString(samplingArea.width-sampleGlobal.width) + "/" + ofToString(samplingArea.height-sampleGlobal.height) + "\n"
                       "[arrows]  Center      = " + ofToString(samplingArea.getCenter(), 1) + "\n"
                       "[k-l]     Samples     = " + ofToString(sampleNum) + "\n" +
                       "[a-s]     Sample size = " + ofToString(sampleSize) + "\n"
                       "[q-w]     Smoothing   = " + ofToString(smoothing, 2) + "\n" +
                       "[a-s]     Sample size = " + ofToString(sampleSize)
                       , 10, 120);
    
}

//--------------------------------------------------------------
ofColor ofApp::getAverageColor(ofRectangle sample, ofPixels frame) {
    
    sampleGlobal.x = (sampleGlobal.x-screen.x)*ratio;
    sampleGlobal.y = (sampleGlobal.y-screen.y)*ratio;
    sampleGlobal.width = sampleGlobal.width*ratio;
    sampleGlobal.height = sampleGlobal.height*ratio;
    
    ofColor averageColor;
    
    int rSum = 0;
    int gSum = 0;
    int bSum = 0;
    
    for(int i = sampleGlobal.x; i < (sampleGlobal.x+sampleGlobal.width); i++) {
        
        for(int j = sampleGlobal.y; j < (sampleGlobal.y+sampleGlobal.height-1); j++) {
            
            ofColor pixelColor = frame.getColor(i, j);
            rSum += pixelColor.r;
            gSum += pixelColor.g;
            bSum += pixelColor.b;
        }
    }
    
    averageColor.r = rSum / sampleGlobal.getArea();
    averageColor.g = gSum / sampleGlobal.getArea();
    averageColor.b = bSum / sampleGlobal.getArea();
    
    return averageColor;

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == OF_KEY_DOWN){
        if(samplingArea.y+sampleGlobal.height < screen.getBottom() && samplingArea.y+samplingArea.height <screen.getBottom()) {
                samplingArea.y += 5;
        }
    }
    if (key == OF_KEY_UP){
        if(samplingArea.y > screen.getTop() && samplingArea.y+samplingArea.height-sampleGlobal.height > screen.getTop()) {
            samplingArea.y -= 5;;
        }
    }
    if (key == OF_KEY_RIGHT){
        if(samplingArea.x+sampleGlobal.width < screen.getRight() && samplingArea.x+samplingArea.width < screen.getRight()) {
            samplingArea.x += 5;;
        }
    }
    if (key == OF_KEY_LEFT){
        if(samplingArea.x > screen.getLeft() && samplingArea.x+samplingArea.width-sampleGlobal.width > screen.getLeft()) {
            samplingArea.x -= 5;;
        }
    }
    
    
    if (key == 'n'){
        if(samplingArea.x+samplingArea.width-sampleGlobal.width < screen.getRight() &&
           samplingArea.x+samplingArea.width-sampleGlobal.width > screen.getLeft()) {
            samplingArea.width -= 5;;
        }
    }
    if (key == 'm'){
        if(samplingArea.getRight() < screen.getRight() &&
           samplingArea.getLeft()+sampleGlobal.width > screen.getLeft()) {
            samplingArea.width += 5;;
        }
    }
    
    if (key == 'c'){
        if(samplingArea.getTop() > screen.getTop()+sampleGlobal.height) {
            samplingArea.height -= 5;;
        }
    }
    if (key == 'v'){
        if(samplingArea.getBottom() < screen.getBottom()) {
            samplingArea.height += 5;;
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
        //if(screen.inside(<#float px#>, <#float py#>))
        if(sampleNum < 500) {
            sampleSize += 10;
        }
//        bool isInside = true;
//        for(int i = 0; i < sampleNum; i++) {
//            if(!screen.inside(samplePos[i].x, samplePos[i].y) || !screen.inside(samplePos[i].x+sampleGlobal.width, samplePos[i].y+sampleGlobal.height)) {
//                isInside = false;
//            }
//        }
//        cout << isInside << endl;
//        if(!isInside) {
//            sampleSize -= 10;
//        }
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
