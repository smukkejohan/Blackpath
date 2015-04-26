#pragma once
#include "defines.h"

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxUI.h" // probably decide on one of theese - Use parameters and parameterGroups
#include "ofxXmlSettings.h"
#include "ofxMultiSelector.h"

#include "Model.h"
#include "Texture.h"
#include "TextureFader.h"
#include "ModelFader.h"

#include "Project.h"

#include "Interface.h"
#include "Renderer.h"

// 1. load images an create thumbnails
// 2. load models and create thumbnails
// 3. put texture on models
// 4. create camera animation
// 5. create nice gui selector

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    Project * project;
    Interface * ui;
    
    ofxSyphonServer syphonOut;
    
    /*ofVec3f uiCamOrientationXY;
    float   uiCamOrientationZ;
    bool    resetCamRot;
    
    ofVec3f uiEffectRotationXY;
    float   uiEffectRotationZ;*/
    
    Renderer * liveRenderer;
    Renderer * previewRenderer;
    
    ofxSyphonServerDirectory dir;
    
    int dirIdx;
    void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
    void serverUpdated(ofxSyphonServerDirectoryEventArgs &args);
    void serverRetired(ofxSyphonServerDirectoryEventArgs &arg);
    
    ofFbo syphonInFbo;
    
    ofxXmlSettings settings;

    float _width;
    float _height;
    bool bUpdateOutput;
    
    
};
