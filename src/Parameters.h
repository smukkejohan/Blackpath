//
//  params.h
//  Blackpath
//
//  Created by Johan Bichel Lindegaard on 18/02/15.
//
//

#pragma once
#include "ofMain.h"
#include "ofxGui.h"

namespace PARAM { // Parmeter constants / defaults
    
    static const float CAM_MIN_SPEED = -1;
    static const float CAM_MAX_SPEED = 1;
    
    static const float CAM_MIN_FOV = 4;
    static const float CAM_MAX_FOV = 160;
    
    static const float CAM_MIN_FAR_CLIP = 1;
    static const float CAM_MAX_FAR_CLIP = 20000;
    
    static const float CAM_MIN_NEAR_CLIP = 0.0;
    static const float CAM_MAX_NEAR_CLIP = 200.0;
    
    
    static const ofVec2f CAM_MIN_OFFSET = ofVec2f(-500,-500);
    static const ofVec2f CAM_MAX_OFFSET = ofVec2f(500,500);
    
    static const ofVec3f CAM_MIN_ORIENTATION = ofVec3f(-120,-120,-120);
    static const ofVec3f CAM_MAX_ORIENTATION = ofVec3f(120,120,120);
    
    static const ofVec3f CAM_MIN_SPEED_AUTO_ORIENTATION = ofVec3f(-1,-1,-1);
    static const ofVec3f CAM_MAX_SPEED_AUTO_ORIENTATION = ofVec3f(1,1,1);
    
    static const ofVec3f EFFECT_DEFAULT_OFFSET = ofVec3f(0,0,-400);
    static const ofVec3f EFFECT_MIN_OFFSET = ofVec3f(-5000,-5000,-5000);
    static const ofVec3f EFFECT_MAX_OFFSET = ofVec3f(5000,5000,100);
    
    static const ofVec3f EFFECT_DEFAULT_SCALE = ofVec3f(1,1,1);
    
    // effect auto rotation
    
    // effect orientation
    
    static const ofVec3f EFFECT_MIN_ORIENTATION = ofVec3f(-120,-120,-120);
    static const ofVec3f EFFECT_MAX_ORIENTATION = ofVec3f(120,120,120);
    
    
};

using namespace PARAM;

// Parameters for a scene
class Parameters {
public:
    
    void init();
    
    void reset();
    
    // all the params
    // + the filters
    
    // Camera parameters
    ofParameter<float>   camFarClip;
    ofParameter<float>   camNearClip; // maybe a range vec2f
    ofParameter<ofVec3f> camOrientation;
    ofParameter<ofVec2f> camOffset;
    ofParameter<float>   camSpeed;
    ofParameter<float>   camFov;
    
    // Auto camera movement parameters
    ofParameter<bool>    bAutoCameraRotation;
    ofParameter<ofVec3f> autoCamSpeed;

    // Effect model parameters
    ofParameter<ofVec3f> effectOffset;
    ofParameter<float>   effectScale;
    
    ofParameter<bool>  bAutoEffectRotation;
    ofParameter<ofVec3f> effectOrientationRef;
    ofParameter<ofVec3f> autoEffectRotSpeed;
    
    ofParameterGroup allParameters;
    //ofxPanel allParamPanel;
    //Put these in UI its an action
    //ofParameter<bool>    resetAll;
    //ofParameter<bool>    resetEffectRot;
    //
    
    // these should be project level
    //ofParameter<float>   directSyphon;
    //ofParameter<float>   blackout;
    
    
    // IDEAS
    // debug gui with ofxGui
    
};