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
#include <algorithm>    // std::min

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
    
    void windowResized(int w, int h);
    void layoutUIInWindow(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    
    // pointers to things we interact with
    Project * project;
    
    void resetTextureSelector();
    void updateTextureSelector();
    void resetModelSelector();
    void updateModelSelector();
    
    void guiEvent(ofxUIEventArgs &e);
    
    ofxUICanvas * projectTopMenu;
    ofxUICanvas * sceneSelect;
    ofxUICanvas * projectSettings;
    ofxUICanvas * sceneSettings;
    ofxUICanvas * effectSettings;
    ofxUIScrollableCanvas * textureSelect;
    ofxUIScrollableCanvas * modelSelect;
    
    vector<ofxUILabelButton*> sceneTabs;
    void addSceneTab(string name);
    
    Scene * selectedScene;
    void selectScene(string name);
    
    // internal ui values for binding
    ofVec3f camRotSpeed;
    ofVec3f camOffset;
    float   camFov;
    float   camSpeed;
    ofVec3f camOrientationXY;
    float   camOrientationZ;
    bool    bAutoCamOrientation;
    ofVec3f effectRotSpeed;
    bool    bAutoEffectRotation;
    float   effectScale;
    ofVec3f effectPositionXY;
    float   effectPositionZ;
    ofVec3f effectRotation;
    
    // ui widgets
    ofxUI2DPad        * camOrientationXYPad;
    ofxUI2DPad        * effectXYPad;
    ofxUI2DPad        * camOffsetPad;
    ofxUISlider       * fovSlider;
    ofxUISlider       * camSpeedSlider;
    ofxUIRangeSlider  * clippingPlaneSlider;
    ofxUISlider       * camOrientationZSlider;
    ofxUISlider       * effectDistanceSlider;
    ofxUISlider       * effectScaleSlider;
    ofxUIButton       * resetOrientationButton;
    ofxUIToggle       * camAutoOrientationToggle;
    ofxUIToggle       * effectAutoRotationToggle;
    
    ofxUIRotarySlider * autoCamRotXSlider;
    ofxUIRotarySlider * autoCamRotYSlider;
    ofxUIRotarySlider * autoCamRotZSlider;

    ofxUIRotarySlider * effectRotXSlider;
    ofxUIRotarySlider * effectRotYSlider;
    ofxUIRotarySlider * effectRotZSlider;
    
    ofxUIRotarySlider * autoEffectRotXSlider;
    ofxUIRotarySlider * autoEffectRotYSlider;
    ofxUIRotarySlider * autoEffectRotZSlider;
    
    ofxUILabelButton * newSceneBtn;
    ofxUILabelButton * removeSceneBtn;
    ofxUILabelButton * cloneSceneBtn;
    ofxUILabelButton * saveSceneBtn;
    ofxUILabelButton * saveProjectBtn;
    
    // actions
    ofxUIButton * cueLiveButton;
    ofxUIToggle * previewToggle;
    
    ofxUISlider * blackoutSlider;
    ofxUISlider * directSyphonSlider;
    
    //void commonUICanvasSetup();
    
    // return toggle button for texture and model selector
    ofxUIToggle * getToggleBtn(string _title, int _id) {
        
        ofxUIToggle * btn = new ofxUIToggle(_title +"_"+ ofToString(_id), false, 20, 20);
        btn->setLabelVisible(false);
        
        return btn;
    }
    
    ofxUILabel * getLabel(string _name) {
        return new ofxUILabel(_name, OFX_UI_FONT_MEDIUM);
    };
    
    
    int numTextures = 0;
    int numModels = 0;
    int colWidth;
    int topMenuHeight;
    int colHeight;
    int halfColHeight;
    
    
};