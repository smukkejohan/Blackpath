#pragma once
#include "defines.h"
#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxUI.h"
#include "ofxXmlSettings.h"
#include "Model.h"
#include "Texture.h"
#include "TextureFader.h"
#include "ModelFader.h"
#include "Project.h"
#include "Interface.h"
#include "Renderer.h"

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
    
    Renderer * liveRenderer;
    Renderer * previewRenderer;
    
    ofxSyphonServerDirectory directory;
    
    int dirIdx = 0;
    void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
    void serverUpdated(ofxSyphonServerDirectoryEventArgs &args);
    void serverRetired(ofxSyphonServerDirectoryEventArgs &arg);
    
    ofxXmlSettings settings;

};
