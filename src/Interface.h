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


/*class Vec3RotarySlider {
public:
    ofxUIRotarySlider * sX;
    ofxUIRotarySlider * sY;
    ofxUIRotarySlider * sZ;
    
    ofVec3f * value;
    
    void setup() {
    }
    
    void addWidgetsDown() {
        
    }
    
};
*/

// int slider
class Vec3IntSlider {
public:
    ofxUISlider_<int> * sX;
    ofxUISlider_<int> * sY;
    ofxUISlider_<int> * sZ;
    
    ofVec3f val;
    int x,y,z;
    ofParameter<ofVec3f> param;
    string name;
    int width, height;
    
    Vec3IntSlider() {
    }
    
    void setup(ofParameter<ofVec3f> & _param) {
        
        //param = _param;
        name = _param.getName();
        width = 280;
        height = 18;
        val = _param.getMin();
        
        sX = new ofxUISlider_<int>(name + "-x", _param.getMin().x, _param.getMax().x, &x, width, height);
        
        sY = new ofxUISlider_<int>(name + "-y", _param.getMin().y, _param.getMax().y, &y, width, height);
        
        sZ = new ofxUISlider_<int>(name + "-z", _param.getMin().z, _param.getMax().z, &z, width, height);
        
    }
 
    void addWidgetsDown(ofxUICanvas * canvas) {
        canvas->addWidgetDown(sX);
        canvas->addWidgetDown(sY);
        canvas->addWidgetDown(sZ);
    }
    
    void updateParams(ofParameter<ofVec3f> & p, ofxUIWidget * w) {
        val = ofVec3f(x,y,z);
        
        if(w == sX || w == sY || w == sZ) {
            p.set(val);
        }
    }
};

class Vec3FloatSlider {
public:
    ofxUISlider_<float> * sX;
    ofxUISlider_<float> * sY;
    ofxUISlider_<float> * sZ;
    
    ofVec3f val;
    ofParameter<ofVec3f> param;
    string name;
    int width, height;
    
    Vec3FloatSlider() {
    }
    
    void setup(ofParameter<ofVec3f> & _param) {
        
        //param = _param;
        name = _param.getName();
        width = 280;
        height = 18;
        val = _param.getMin();
        
        sX = new ofxUISlider_<float>(name + "-x", _param.getMin().x, _param.getMax().x, &val.x, width, height);
        
        sY = new ofxUISlider_<float>(name + "-y", _param.getMin().y, _param.getMax().y, &val.y, width, height);
        
        sZ = new ofxUISlider_<float>(name + "-z", _param.getMin().z, _param.getMax().z, &val.z, width, height);
        
    }
    
    void addWidgetsDown(ofxUICanvas * canvas) {
        canvas->addWidgetDown(sX);
        canvas->addWidgetDown(sY);
        canvas->addWidgetDown(sZ);
    }
    
    void updateParams(ofParameter<ofVec3f> & p, ofxUIWidget * w) {
        if(w == sX || w == sY || w == sZ) {
            p.set(val);
        }
    }
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
    void resetSceneSelector();
    void guiEvent(ofxUIEventArgs &e);
    
    ofxUICanvas * projectTopMenu;
    ofxUICanvas * sceneSelect;
    ofxUICanvas * projectSettings;
    ofxUICanvas * sceneSettings;
    ofxUICanvas * effectSettings;
    ofxUIScrollableCanvas * textureSelect;
    ofxUIScrollableCanvas * modelSelect;
    
    vector<ofxUILabelButton*> sceneTabs;
    void addSceneTab(string name, int i);
    
    Scene * selectedScene;
    void selectScene(Scene * _scene);
    
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
    ofxUIButton       * resetEffectOffsetButton;
    ofxUIButton       * resetOffsetButton;
    ofxUIToggle       * camAutoOrientationToggle;
    ofxUIToggle       * effectAutoRotationToggle;
    
    
    Vec3FloatSlider autoCamRotSlider;
    /*ofxUIRotarySlider * autoCamRotXSlider;
    ofxUIRotarySlider * autoCamRotYSlider;
    ofxUIRotarySlider * autoCamRotZSlider;*/
    
    Vec3FloatSlider effectRotSlider;
    /*ofxUIRotarySlider * effectRotXSlider;
    ofxUIRotarySlider * effectRotYSlider;
    ofxUIRotarySlider * effectRotZSlider;*/
    
    Vec3FloatSlider autoEffectRotSlider;
    /*ofxUIRotarySlider * autoEffectRotXSlider;
    ofxUIRotarySlider * autoEffectRotYSlider;
    ofxUIRotarySlider * autoEffectRotZSlider;*/
    
    Vec3IntSlider effectReplicate;
    Vec3IntSlider effectSpacing;
    
    ofxUILabelButton * newSceneBtn;
    ofxUILabelButton * removeSceneBtn;
    ofxUILabelButton * cloneSceneBtn;
    ofxUILabelButton * saveSceneBtn;
    ofxUILabelButton * saveProjectBtn;
    
    ofxUITextInput * widthInput;
    ofxUITextInput * heightInput;
    ofxUIButton * updateOutputButton;
    
    
    ofxUITextInput * sceneNameInput;
    ofxUITextInput * projectNameInput;
    
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