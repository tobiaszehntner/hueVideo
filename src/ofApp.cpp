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
    screen.y = 150;
    screen.width = ofGetWindowWidth();
    screen.height = ofGetWindowWidth()/video.getWidth()*video.getHeight();
    
    ratio = video.getWidth()/screen.width; // 2.4
    
    videoSpeed = 1;
    
    // Sampling
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
    
    smoothing = 0; // 0-1, 0 = no smoothing
    
    // Hue
    isHueOn = false;
    hueBridgeIP = "192.168.100.101";
    hueUser = "tobiaszehntner";
    hueUpdateDecisecond = sampleNum;
    hueUpdateTimer = 0;
    hueUpdateLast = 0;
    
    if(isHueOn) {
        hueGet(""); // Print Hue setup to output (can be "", "lights", "groups", "config")
        hueSetup(2); // Set general settings of Hue bulbs for groupNum
    }
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
    video.setSpeed(videoSpeed);
    
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
    hueUpdateTimer =  ofGetElapsedTimeMillis()/100;
    
    if(isHueOn) {
        
        if(hueUpdateTimer >= hueUpdateLast + hueUpdateDecisecond) {
            
            hueColor.clear();
            
            for (int i = 0; i < sampleNum; i++) {
                hueSetColor(i, averageColor[i], hueUpdateDecisecond);
                hueColor.push_back(averageColor[i]);
            }
            hueUpdateLast = hueUpdateTimer;
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
        ofDrawBitmapString(ofToString(i), samples[i].x+5, samples[i].y+15);
    }
    
    ofSetColor(ofColor::red);
    ofLine(samplingArea.x, samplingArea.y, samplingArea.x+samplingArea.width, samplingArea.y+samplingArea.height);
    
    ofSetColor(0);
    ofDrawBitmapString("Video", 10, 25);
    for (int i = 0; i < sampleNum; i++) {
        ofSetColor(averageColor[i], 255);
        ofFill();
        ofRect(60 + (i*60), 10, 50, 50);
        ofSetColor(128);
        ofDrawBitmapString(ofToString(i), 65 + (i*60), 25);
    }
    
    ofSetColor(0);
    ofDrawBitmapString("Hue", 10, 85);
    for (int i = 0; i < sampleNum; i++) {
        if(isHueOn) {
            ofSetColor(hueColor[i], 255);
            ofFill();
        } else {
            ofSetColor(0);
            ofNoFill();
        }
        ofRect(60 + (i*60), 70, 50, 50);
        ofSetColor(128);
        ofDrawBitmapString(ofToString(i), 65 + (i*60), 85);
    }
    
    ofSetColor(0, 0, 0, 80);
    ofFill();
    ofRect(screen.x, screen.y, screen.width, 90);
    
    ofSetColor(255);
    ofDrawBitmapString("[c-v/n-m] X/Y Distr   = " + ofToString(samplingArea.width-sampleGlobal.width) + "/" + ofToString(samplingArea.height-sampleGlobal.height)
              + "\n" + "[arrows]  Center      = " + ofToString(samplingArea.getCenter(), 1)
              + "\n" + "[k-l]     Samples     = " + ofToString(sampleNum)
              + "\n" + "[a-s]     Sample size = " + ofToString(sampleSize)
              + "\n" + "[q-w]     Smoothing   = " + ofToString(smoothing, 2)
              + "\n" + "[r-t]     Video speed = " + ofToString(video.getSpeed()) + "x"
              , 10, 170);
    if(isHueOn) {
        ofDrawBitmapString("[o]   Hue       = On", ofGetWindowWidth()-200, 170);
    } else {
        ofDrawBitmapString("[o]   Hue       = Off", ofGetWindowWidth()-200, 170);
    }
    ofDrawBitmapString(    "[u-i] HueUpdate = " + ofToString(hueUpdateDecisecond/10, 1) + "s", ofGetWindowWidth()-200, 185);
    
}

//--------------------------------------------------------------
ofColor ofApp::getAverageColor(ofRectangle tempSample, ofPixels frame) {
    
    tempSample.x = (tempSample.x-screen.x)*ratio;
    tempSample.y = (tempSample.y-screen.y)*ratio;
    tempSample.width = tempSample.width*ratio;
    tempSample.height = tempSample.height*ratio;
    
    ofColor averageColor;
    
    int rSum = 0;
    int gSum = 0;
    int bSum = 0;
    for(int i = tempSample.x; i < (tempSample.x+tempSample.width); i++) {
        
        for(int j = tempSample.y; j < (tempSample.y+tempSample.height-1); j++) {
            
            ofColor pixelColor = frame.getColor(i, j);
            rSum += pixelColor.r;
            gSum += pixelColor.g;
            bSum += pixelColor.b;
        }
    }
    
    averageColor.r = rSum / tempSample.getArea();
    averageColor.g = gSum / tempSample.getArea();
    averageColor.b = bSum / tempSample.getArea();
    
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
    messageBody["transitiontime"] = hueUpdateDecisecond;       // 10 = 1sec;
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
void ofApp::hueOffAll(int hueGroupNum) {
    
    std::string groupNumString = ofToString(hueGroupNum);
    std::string putUrl = "http://" + hueBridgeIP + "/api/" + hueUser + "/groups/" + groupNumString + "/action";
    // Hue values
    Json::Value messageBody;
    
    messageBody["on"]             = false;
    
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
    
    std::string lightNumString = ofToString(lightNum+1); // Hue lights start at 1, array starts at 0, therefor +1
    std::string putUrl = "http://" + hueBridgeIP + "/api/" + hueUser + "/lights/" + lightNumString + "/state";
    
    // Make sure color data is in int
    int hue = ofMap(color.getHue(), 0, 255, 0, 65535);
    int saturation = color.getSaturation();
    int brightness = color.getBrightness();
    
    Json::Value messageBody;
    messageBody["hue"]            = hue;       // 0-65535 (red to red)
    messageBody["sat"]            = saturation;     // 0-254 (0 = white)
    messageBody["bri"]            = brightness;     // 1-254 (254 brightest)
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
    
    switch (key) {
            
        case OF_KEY_DOWN:
            if(samplingArea.y+sampleGlobal.height < screen.getBottom() && samplingArea.y+samplingArea.height <screen.getBottom()) {
                samplingArea.y += 5;
            }
            break;
            
        case OF_KEY_UP:
            if(samplingArea.y > screen.getTop() && samplingArea.y+samplingArea.height-sampleGlobal.height > screen.getTop()) {
                samplingArea.y -= 5;;
            }
            break;
            
        case OF_KEY_RIGHT:
            if(samplingArea.x+sampleGlobal.width < screen.getRight() && samplingArea.x+samplingArea.width < screen.getRight()) {
                samplingArea.x += 5;;
            }
            break;
        
        case OF_KEY_LEFT:
            if(samplingArea.x > screen.getLeft() && samplingArea.x+samplingArea.width-sampleGlobal.width > screen.getLeft()) {
                samplingArea.x -= 5;;
            }
            break;
            
        case 'n':
            if(samplingArea.x+samplingArea.width-sampleGlobal.width < screen.getRight() &&
               samplingArea.x+samplingArea.width-sampleGlobal.width > screen.getLeft()) {
                samplingArea.width -= 5;
                samplingArea.x += 2.5;
            }
            break;
        
        case 'm':
            if(samplingArea.getRight() < screen.getRight() &&
               samplingArea.getLeft()+sampleGlobal.width > screen.getLeft()) {
                samplingArea.width += 5;
                samplingArea.x -= 2.5;
            }
            break;
            
        case 'c':
            if(samplingArea.getTop() > screen.getTop()+sampleGlobal.height) {
                samplingArea.height -= 5;
                samplingArea.y += 2.5;
            }
            break;

            
        case 'v':
            if(samplingArea.getBottom() < screen.getBottom()) {
                samplingArea.height += 5;
                samplingArea.y -= 2.5;
            }
            break;
            
        case 'k':
            if(sampleNum == 2) {
                samplingAreaPrevious = samplingArea;
                samplingArea.setFromCenter(samplingArea.getCenter(), sampleGlobal.width, sampleGlobal.height);
            }
            if(sampleNum > 1) {
                sampleNum--;
            }
            break;
            
        case 'l':
            if(sampleNum == 1) {
                samplingArea = samplingAreaPrevious;
            }
            if(sampleNum < 10) {
                sampleNum++;
                if(hueUpdateDecisecond < sampleNum) {
                    hueUpdateDecisecond = sampleNum;
                }
            }
            break;
            
        case 'a':
            if(sampleSize > 20) {
                sampleSize -= 5;
            }
            break;
            
        case 's':
            if(samplingArea.y+sampleSize < screen.getBottom()
               && samplingArea.y+samplingArea.height-sampleSize > screen.getTop()
               && samplingArea.x+sampleSize < screen.getRight()
               && samplingArea.x+samplingArea.width-sampleSize > screen.getLeft()
               ) {
                sampleSize += 5;
            }
            break;
            
        case 'q':
            if(smoothing < 0.98) {
                smoothing += 0.01;
            }
            break;
            
        case 'w':
            if(smoothing > 0.02) {
                smoothing -= 0.01;
            } else {
                smoothing = 0;
            }
            break;
            
        case 'o':
            if(!isHueOn) {
                hueSetup(2);
                isHueOn = true;
            } else {
                hueOffAll(2);
                isHueOn = false;
            }
            break;
            
        case 'u':
            if(hueUpdateDecisecond > sampleNum) {
                hueUpdateDecisecond -= 1;
            }
            break;
            
        case 'i':
            if(hueUpdateDecisecond < 60) {
                hueUpdateDecisecond += 1;
            }
            break;
            
        case 't':
            if(video.getSpeed() < 0) {
                videoSpeed /= 2;
            } else if(video.getSpeed() == 0) {
                videoSpeed = 1;
            } else if(video.getSpeed() < 10 && video.getSpeed() > 0) {
                videoSpeed *= 2;
            }
            break;
            
        case 'r':
            if(video.getSpeed() > 1) {
                videoSpeed /= 2;
            } else if(video.getSpeed() == 1) {
                videoSpeed = 0;
            }else if(video.getSpeed() == 0) {
                videoSpeed = -1;
            } else if (video.getSpeed() < 0 && video.getSpeed() > -10) {
                videoSpeed *= 2;
            }
            break;
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
