//
//  ui.h
//  Blackpath
//
//  Created by Johan Bichel Lindegaard on 18/02/15.
//
//

#pragma once
#include "ofMain.h"
#include "ofxUI.h"
#include "Project.h"
#include "ofxMultiSelector.h"

class Interface {
    
public:
    
    Interface(Project * _project) {
        project = _project;
    }
    
    void setup();
    void update();
    void draw();
    void exit();
    
    // pointers to things we interact with
    Project * project;
    
    void guiEvent(ofxUIEventArgs &e);
    
    ofxUICanvas * projectTopMenu;
    ofxUICanvas * sceneSelect;
    ofxUICanvas * projectSettings;
    ofxUICanvas * sceneSettings;
    
    vector<ofxUILabelButton*> sceneTabs;
    void addSceneTab(string name);
    
    Scene * selectedScene;
    
    MultiSelector * textureSelector;
    MultiSelector * modelSelector;
    
    void thumbEventListener(MultiSelectorEventData& args);
};