

#ifndef hueVideo_sample_h
#define hueVideo_sample_h

#include "ofMain.h"

class Sample {
    
public: // place public functions or variables declarations here
    
    // methods, equivalent to specific functions of your class objects
    void setup();	//setup method, use this to setup your object's initial state
    void update();  // update method, used to refresh your objects properties
    void draw();    // draw method, this where you'll do the object's drawing
    
    void sampling(ofPixels& frame);
    
    // variables
    int x, y, w, h;
    int r, g, b;
    
    ofColor color_wireframe; // color using ofColor type
    ofColor color_sample;
    int number;
    int samplePix[3];
    
    Sample(); // constructor - used to initialize an object, if no properties are passed the program sets them to the default value
    
private: // place private functions or variables declarations here
};

#endif
