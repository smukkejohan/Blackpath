//
//  params.h
//  Blackpath
//
//  Created by Johan Bichel Lindegaard on 18/02/15.
//
//

#pragma once
#include "ofMain.h"

// Parameters for a scene
class Parameters {
public:
    
    void init();
    
    void reset();
    
    // all the params
    // + the filters
    
    // Camera parameters
    ofParameter<float>   camFarClip;
    ofParameter<float>   camNearClip;
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