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
//#include "ofxMultiSelector.h"

using namespace PARAM;


class textureSelector {
public:
    
    
    // hidden or shown
    // what id it corresponds to
    // update method to set what is selected
    
    
    
    
    
    
    
};


class Interface {
    
public:
    
    Interface(Project * _project) {
        project = _project;
    }
    
    void setup();
    void update();
    void draw();
    void exit();
    
    ofxUIToggle * getToggleBtn(string _title, int _id) {
        

        ofxUIToggle * btn = new ofxUIToggle(_title +"_"+ ofToString(_id), false, 20, 20);
        btn->setLabelVisible(false);
        
                //cout<<"adding tex button with id "<< _id<<endl;
        
        return btn;
    }
    
    
    // pointers to things we interact with
    Project * project;
    
    void resetTextureSelector();
    
    void guiEvent(ofxUIEventArgs &e);
    
    ofxUICanvas * projectTopMenu;
    ofxUICanvas * sceneSelect;
    ofxUICanvas * projectSettings;
    
    ofxUICanvas * sceneSettings; //todo rename cam settings
    ofxUICanvas * effectSettings;
    
    
    //ofxUICanvas
    
    
    ofxUIScrollableCanvas * textureSelect;
    ofxUIScrollableCanvas * modelSelect;
    
    
    vector<ofxUILabelButton*> sceneTabs;
    void addSceneTab(string name);
    
    Scene * selectedScene;
    
    void selectScene(string name);
    //MultiSelector * textureSelector;
    //MultiSelector * modelSelector;
    
    //void thumbEventListener(MultiSelectorEventData& args);
    
    ofVec3f camRotSpeed;
    
    ofVec3f camOffset;
    ofVec3f effectRotation;
    
    
    ofVec3f camOrientationXY;
    float camOrientationZ;
    
    bool bAutoCameraRotation;
    
    //ofVec3f effectOrientationXY;
    //float effectOrientationZ;
    
    ofVec3f autoEffectRotSpeed;
    
    bool bAutoEffectRotation;
    
    float effectScale;
    
    ofxUI2DPad * effectXYPad;
    ofVec3f effectPositionXY;
    float effectPositionZ;
    
    ofxUI2DPad * camXYPad;
    
    ofxUI2DPad * camOffsetPad;
    
    float camFov;
    float camSpeed;
    ofxUISlider * fovSlider;
    
    ofxUIRangeSlider * clippingSlider;
    
    
    
};