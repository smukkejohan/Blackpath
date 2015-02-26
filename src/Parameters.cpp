//
//  params.cpp
//  Blackpath
//
//  Created by Johan Bichel Lindegaard on 18/02/15.
//
//

#include "Parameters.h"


void Parameters::reset() {
    
    // default values, labels and ranges
    
    /*camSpeed.set("Speed", 0, -1, 1);
    camFov.set("Fov", 50, 4, 160);
    camFarClip.set("FarClip", 4000.0, 1, 20000);
    camNearClip.set("NearClip", 8.0, 0.0, 200);
    camOffset.set("Offset", ofVec2f(0,-80), ofVec2f(-500,-500), ofVec2f(500,500));
    
    camOrientation.set("Orientation Ref", ofVec3f(0,0,0), ofVec3f(120,120,120), ofVec3f(-120,-120,-120));
    
    bAutoCameraRotation.set("Auto Camera Orientation", false);
    autoCamSpeed.set("Auto orientation speed", ofVec3f(0,0,0), ofVec3f(-1,-1,-1), ofVec3f(1,1,1));
    
    effectOffsetParam.set("Offset", ofVec3f(0,0,-400), ofVec3f(-5000,-5000,-5000), ofVec3f(5000,5000,100));
    effectScale.set("Scale", 1, 0, 1);
    
    bAutoEffectRotation.set("Auto rotation", false);
    effectOrientationRef.set("Rotation ref", ofVec3f(0,0,0), ofVec3f(-120,-120,-120), ofVec3f(120,120,120));
    
    autoEffectRotSpeed.set("Auto rot speed", ofVec3f(0,0,0), ofVec3f(-1,-1,-1), ofVec3f(1,1,1));*/
    
    
    /*if(resetAll) {
     
     camSpeed.set(0);
     camFov.set(50);
     camFarClip.set(4000);
     camNearClip.set(8);
     camOffsetParam.set(ofVec2f(0,0));
     camOrientationParam.set(ofVec3f(0,0,0));
     autoCameraRotation.set(false);
     autoEffectRotSpeed.set(ofVec3f(0,0,0));
     bAutoEffectRotation.set(false);
     autoEffectRotSpeed.set(ofVec3f(0,0,0));
     directSyphon.set(0);
     effectScale.set(1);
     effectOffsetParam.set(ofVec3f(0,0,-400));
     autoCamSpeed.set(ofVec3f(0,0,0));
     effectOrientationRef.set(ofVec3f(0,0,0));
     
     resetAll = false;
     }*/
    
}


void Parameters::init() {
    
    // default values, labels and ranges
    camSpeed.set("Speed", 0, -1, 1);
    camFov.set("Fov", 50, 4, 160);
    camFarClip.set("FarClip", 4000.0, 1, 20000);
    camNearClip.set("NearClip", 8.0, 0.0, 200);
    camOffset.set("Offset", ofVec2f(0,-80), ofVec2f(-500,-500), ofVec2f(500,500));
    camOrientation.set("Orientation Ref", ofVec3f(0,0,0), ofVec3f(120,120,120), ofVec3f(-120,-120,-120));
    bAutoCameraRotation.set("Auto Camera Orientation", false);
    autoCamSpeed.set("Auto orientation speed", ofVec3f(0,0,0), ofVec3f(-1,-1,-1), ofVec3f(1,1,1));
    effectOffset.set("Offset", ofVec3f(0,0,-400), ofVec3f(-5000,-5000,-5000), ofVec3f(5000,5000,100));
    effectScale.set("Scale", 1, 0, 1);
    bAutoEffectRotation.set("Auto rotation", false);
    effectOrientationRef.set("Rotation ref", ofVec3f(0,0,0), ofVec3f(-120,-120,-120), ofVec3f(120,120,120));
    autoEffectRotSpeed.set("Auto rot speed", ofVec3f(0,0,0), ofVec3f(-1,-1,-1), ofVec3f(1,1,1));
    //directSyphon.set("Syphon direct out", 0, 0, 1);
    
    //camOffsetParam.set(ofVec2f(0,0));
    //camOrientationParam.set( ofVec3f(-uiCamOrientationXY.y, uiCamOrientationXY.x, uiCamOrientationZ) );
    
}