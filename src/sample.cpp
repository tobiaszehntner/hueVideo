

#include "sample.h"
#include "ofMain.h"

Sample::Sample(){
}

void Sample::setup(){
    x = ofGetWindowWidth()/2;
    y = ofGetWindowHeight()/2;
    w = 50;
    h = 50;
    
    number = 1;
    
    color.set(0);
}


void Sample::update(){
    
    
}

void Sample::draw(){
    
    ofNoFill();
    ofRect(x, y, w, h);
    ofDrawBitmapString(ofToString(number), x+5, y+15);
    
}