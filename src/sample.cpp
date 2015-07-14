

#include "sample.h"

Sample::Sample(){
}

void Sample::setup(){
    x = 0;
    y = 0;
    
    w = 20;           // and random speed and direction
    h = 20;
    
    color.set(255, 255, 255);
}


void Sample::update(){
    
    
}

void Sample::draw(){
    
    ofNoFill();
    ofRect(x, y, w, h);
}