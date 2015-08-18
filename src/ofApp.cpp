#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    // Window
    ofSetWindowTitle("hueVideo");
    ofBackground(255);
    
    // Video
    video.loadMovie("video.mov");
    video.setVolume(0);
    video.play();
    
    screen.x = 0;
    screen.y = 100;
    screen.width = ofGetWindowWidth();
    screen.height = ofGetWindowWidth()/video.getWidth()*video.getHeight();
    
    ratio = video.getWidth()/screen.width; // 2.4
    
    // Sampling
    sampleNum = 9;
    sampleSize = 50; // pixels
    sample.width = sampleSize;
    sample.height = sampleSize;
    
    samplingAreaCenter.x = screen.x+screen.width/2;
    samplingAreaCenter.y = screen.y+screen.height/2;
    samplingArea.width = screen.width;
    samplingArea.height = sample.height;
    samplingArea.x = screen.x;
    samplingArea.y = screen.y+screen.height/2-sample.height/2;
    
    smoothing = 0.8; // 0-1, 0 = no smoothing
    
    // Hue
    isHueOn = false;
    hueBridgeIP = "192.168.100.100";
    hueUser = "tobiaszehntner";
    
    if(isHueOn) {
        hueGet(""); // Print Hue setup to output (can be "", "lights", "groups", "config")
        hueSetup(2); // Set general settings of Hue bulbs for groupNum
    }
}

//--------------------------------------------------------------
void ofApp::update(){

    sample.width = sampleSize;
    sample.height = sampleSize;
    
    samples.clear();

    for (int i = 0; i < sampleNum; i++) {
        
        ofRectangle tempSample;
        
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
    
    if(isHueOn) {
        for (int i = 0; i < sampleNum; i++) {
            hueSetColor(i, averageColor[i], 0);
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
    ofDrawBitmapString("[c-v/n-m] X/Y Distr   = " + ofToString(samplingArea.width-sample.width) + "/" + ofToString(samplingArea.height-sample.height) + "\n"
                       "[arrows]  Center      = " + ofToString(samplingArea.getCenter()) + "\n"
                       "[k-l]     Samples     = " + ofToString(sampleNum) + "\n" +
                       "[a-s]     Sample size = " + ofToString(sampleSize) + "\n"
                       "[q-w]     Smoothing   = " + ofToString(smoothing, 2) + "\n" +
                       "[a-s]     Sample size = " + ofToString(sampleSize)
                       , 10, 120);
    if(isHueOn) {
        ofDrawBitmapString("[o] Hue = On"
                           , ofGetWindowWidth()-120, 120);
    } else {
        ofDrawBitmapString("[o] Hue = Off"
                           , ofGetWindowWidth()-120, 120);
    }
    
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
void ofApp::hueGet(string hueGetObject) { // can be "", "lights", "groups", "config"
    
    std::string getUrl = "http://" + hueBridgeIP + "/api/" + hueUser + "/" + hueGetObject;
    // GET request
    ofx::HTTP::GetRequest getRequest(getUrl);
    
    // Print GET request
    try
    {
        // Execute the request and get the response stream.
        std::istream& responseStream = client.execute(getRequest, response, context);
        // Request and response headers can be examined here.
        std::cout << "============" << endl;
        // Copy the output to the terminal.
        Poco::StreamCopier::copyStream(responseStream, std::cout);
        
        // Flush the input stream.
        std::cout << std::endl;
        std::cout << "============" << endl;
    }
    catch(const Poco::Exception& exc)
    {
        ofLogError("ofApp::setup") << "Got Exception " << exc.displayText() << " " << exc.code();
    }
    catch(...)
    {
        ofLogError("ofApp::setup") << "Got unknown exception.";
    }
    
}

//--------------------------------------------------------------
void ofApp::hueSetup(int hueGroupNum) {
    
    std::string groupNumString = ofToString(hueGroupNum);
    std::string putUrl = "http://" + hueBridgeIP + "/api/" + hueUser + "/groups/" + groupNumString + "/action";
    // Hue values
    Json::Value messageBody;
    
    messageBody["on"]             = true;
    messageBody["bri"]            = 254;     // 1-254 (254 brightest)
    messageBody["hue"]            = 0;       // 0-65535 (red to red)
    messageBody["sat"]            = 0;       // 0-254 (0 = white)
    messageBody["alert"]          = "none";  // none, select, lselect
    messageBody["effect"]         = "none";  // colorloop or none
    messageBody["transitiontime"] = 0;       // 10 = 1sec;
    messageBody["ct"]             = 153;     // 153 (6500K/daylight) - 500 (2000K/candlelight)
    
    bodyBuffer = messageBody.toStyledString();
    
    cout << bodyBuffer.getText() << endl;
    
    ofx::HTTP::PutRequest putRequest(putUrl);
    putRequest.setPutBuffer(bodyBuffer);
    
    try
    {
        // Execute the request and get the response stream.
        std::istream& responseStream = client.execute(putRequest, response, context);
        
        // Request and response headers can be examined here.
        std::cout << "============" << endl;
        // Copy the output to the terminal.
        Poco::StreamCopier::copyStream(responseStream, std::cout);
        
        // Flush the input stream.
        std::cout << std::endl;
        std::cout << "============" << endl;
    }
    catch(const Poco::Exception& exc)
    {
        ofLogError("ofApp::setup") << "Got Exception " << exc.displayText() << " " << exc.code();
    }
    catch(...)
    {
        ofLogError("ofApp::setup") << "Got unknown exception.";
    }
}

//--------------------------------------------------------------
void ofApp::hueSetColor(int lightNum, ofColor color, int transitionTime) {
    
    std::string lightNumString = ofToString(lightNum);
    std::string putUrl = "http://" + hueBridgeIP + "/api/" + hueUser + "/lights/" + lightNumString + "/state";
    // Hue values
    Json::Value messageBody;
    messageBody["hue"]            = color.getHue();       // 0-65535 (red to red)
    messageBody["sat"]            = color.getSaturation();     // 0-254 (0 = white)
    messageBody["bri"]            = color.getBrightness();     // 1-254 (254 brightest)
    messageBody["transitiontime"] = transitionTime;      // 10 = 1sec;
    
    bodyBuffer = messageBody.toStyledString();
    
    cout << bodyBuffer.getText() << endl;
    
    ofx::HTTP::PutRequest putRequest(putUrl);
    putRequest.setPutBuffer(bodyBuffer);
    
    try
    {
        // Execute the request and get the response stream.
        std::istream& responseStream = client.execute(putRequest, response, context);
        
        // Request and response headers can be examined here.
        std::cout << "============" << endl;
        // Copy the output to the terminal.
        Poco::StreamCopier::copyStream(responseStream, std::cout);
        
        // Flush the input stream.
        std::cout << std::endl;
        std::cout << "============" << endl;
    }
    catch(const Poco::Exception& exc)
    {
        ofLogError("ofApp::setup") << "Got Exception " << exc.displayText() << " " << exc.code();
    }
    catch(...)
    {
        ofLogError("ofApp::setup") << "Got unknown exception.";
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == OF_KEY_DOWN){
        if(samplingArea.y+sample.height < screen.getBottom() && samplingArea.y+samplingArea.height <screen.getBottom()) {
                samplingArea.y += 5;
        }
    }
    if (key == OF_KEY_UP){
        if(samplingArea.y > screen.getTop() && samplingArea.y+samplingArea.height-sample.height > screen.getTop()) {
            samplingArea.y -= 5;;
        }
    }
    if (key == OF_KEY_RIGHT){
        if(samplingArea.x+sample.width < screen.getRight() && samplingArea.x+samplingArea.width < screen.getRight()) {
            samplingArea.x += 5;;
        }
    }
    if (key == OF_KEY_LEFT){
        if(samplingArea.x > screen.getLeft() && samplingArea.x+samplingArea.width-sample.width > screen.getLeft()) {
            samplingArea.x -= 5;;
        }
    }
    
    
    if (key == 'n'){
        if(samplingArea.x+samplingArea.width-sample.width < screen.getRight() &&
           samplingArea.x+samplingArea.width-sample.width > screen.getLeft()) {
            samplingArea.width -= 5;;
        }
    }
    if (key == 'm'){
        if(samplingArea.getRight() < screen.getRight() &&
           samplingArea.getLeft()+sample.width > screen.getLeft()) {
            samplingArea.width += 5;;
        }
    }
    
    if (key == 'c'){
        if(samplingArea.getTop() > screen.getTop()+sample.height) {
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
    
    if (key == 'o'){
        if(!isHueOn) {
            isHueOn = true;
        } else {
            isHueOn = false;
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
