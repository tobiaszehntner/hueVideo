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
    
    ratio = video.getWidth()/screen.width; // 1.6
    
    sampleNum = 9;
    sampleSize = 50; // pixels
    sample.width = sampleSize;
    sample.height = sampleSize;
    
    samplingAreaCenter.x = screen.x+screen.width/2;
    samplingAreaCenter.y = screen.y+screen.height/2;
    samplingArea.width = screen.width;
    samplingArea.height = sample.height;
    
    smoothing = 0.8; // 0-1, 0 = no smoothing
}

//--------------------------------------------------------------
void ofApp::update(){

    sample.width = sampleSize;
    sample.height = sampleSize;

    samples.clear();

    for (int i = 0; i < sampleNum; i++) {
        
        ofRectangle tempSample;
        tempSample.width = sample.width;
        tempSample.height = sample.height;
        
        if(sampleNum > 1) {
            tempSample.setFromCenter(
                                     ((samplingArea.x+sample.width/2) + ((samplingArea.width-sample.width) / (sampleNum-1)) * i),
                                     ((samplingArea.y+sample.height/2) + ((samplingArea.height-sample.height) / (sampleNum-1)) * i),
                                     sample.width,
                                     sample.height);
        } else {
            tempSample.setFromCenter(samplingArea.getCenter(), sample.width, sample.height);
        }
        
        samples.push_back(tempSample);
        
    }
    
//    for (int i = 0; i < sampleNum; i++) {
//    
//        int sampleX;
//        int sampleY;
//        
//        if(sampleNum > 1) {
//            sampleX = samplingAreaCenter.x - (samplingArea.width/2) + ( ((samplingArea.width-sample.width) / (sampleNum-1)) * i);
//        } else {
//            sampleX = samplingAreaCenter.x - sample.width/2;
//        }
//        
//        if(sampleNum > 1) {
//            sampleY = samplingAreaCenter.y - (samplingArea.height/2) + ( ((samplingArea.height-sample.height) / (sampleNum-1)) * i);
//        } else {
//            sampleY = samplingAreaCenter.y - sample.height/2;
//        }
//
//        ofVec2f loc(sampleX, sampleY);
//
//        samplePos.push_back(loc);
//        
//    }
    
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
    
    
    ofSetColor(0);
    ofDrawBitmapString("X/Y Distr [c-v/n-m] = " + ofToString(samplingArea.width-sample.width) + "/" + ofToString(samplingArea.height-sample.height) + "\n"
                       "Center [arrowKeys]  = " + ofToString(samplingAreaCenter.x) + "/" + ofToString(samplingAreaCenter.y)
                       , 10, 80);
    ofDrawBitmapString("Samples [k-l]     = " + ofToString(sampleNum) + "\n" +
                       "Sample size [a-s] = " + ofToString(sampleSize)
                       , 300, 80);
    ofDrawBitmapString("Smoothing [q-w] = " + ofToString(smoothing, 2) + "\n" +
                       "Sample size [a-s] = " + ofToString(sampleSize)
                       , 500, 80);
    
}

//--------------------------------------------------------------
ofColor ofApp::getAverageColor(ofRectangle sample, ofPixels frame) {
    
    sample.scale(ratio);
    
    ofColor averageColor;
    
    int rSum = 0;
    int gSum = 0;
    int bSum = 0;
    
    for(int i = sample.x; i < (sample.x+sample.width); i++) {
        
        for(int j = sample.y; j < (sample.y+sample.height); j++) {
            
            ofColor pixelColor = frame.getColor(i, j);
            rSum += pixelColor.r;
            gSum += pixelColor.g;
            bSum += pixelColor.b;
        }
    }
    
    averageColor.r = rSum / sample.getArea();
    averageColor.g = gSum / sample.getArea();
    averageColor.b = bSum / sample.getArea();
    
    return averageColor;

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == OF_KEY_DOWN){
        if(samplingAreaCenter.y < (screen.y + screen.height - (sample.height/2))) {
            samplingAreaCenter.y += 5;
        }
    }
    if (key == OF_KEY_UP){
        if(samplingAreaCenter.y > (screen.y + (sample.height/2))) {
            samplingAreaCenter.y -= 5;;
        }
    }
    if (key == OF_KEY_RIGHT){
        if(samplingAreaCenter.x < (screen.x + screen.width - (sample.width/2))) {
            samplingAreaCenter.x += 5;;
        }
    }
    if (key == OF_KEY_LEFT){
        if(samplingAreaCenter.x > (screen.x + (sample.width/2))) {
            samplingAreaCenter.x -= 5;;
        }
    }
    
    if (key == 'n'){
        int num = (screen.width*-1)+(2*sample.width);
        if(samplingArea.width > num) {
            samplingArea.width -= 5;;
        }
    }
    if (key == 'm'){
        if(samplingArea.width < screen.width) {
            samplingArea.width += 5;;
        }
    }
    
    if (key == 'c'){
        int num = (screen.height*-1)+(2*sample.height);
        if(samplingArea.height > num) {
            samplingArea.height -= 5;;
        }
    }
    if (key == 'v'){
        if(samplingArea.height < screen.height) {
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
        if(sampleNum < 500) {
            sampleSize += 10;
        }
//        bool isInside = true;
//        for(int i = 0; i < sampleNum; i++) {
//            if(!screen.inside(samplePos[i].x, samplePos[i].y) || !screen.inside(samplePos[i].x+sample.width, samplePos[i].y+sample.height)) {
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
