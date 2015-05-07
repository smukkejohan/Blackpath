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
    
    allParameters.setSerializable(true);
    
    // default values, labels and ranges
    allParameters.add(
            camSpeed.set("speed", 0, -1, 1));
    
    // field of view
    allParameters.add(
            camFov.set("fov", 50, CAM_MIN_FOV, CAM_MAX_FOV));
    
    allParameters.add(
            camFarClip.set("far-clip", 30000.0));
    
    allParameters.add(
            camNearClip.set("near-clip", 8.0));
    
    // camera offset x,y - pan
    allParameters.add(
            camOffset.set("cam-offset", ofVec2f(0,-0.18)));
    
    // camera orientation
    allParameters.add(
            camOrientation.set("cam-orientation-reference", ofVec3f(0,0,0), CAM_MIN_ORIENTATION, CAM_MAX_ORIENTATION));
    
    // direction and speed of automatic camera rotation
    allParameters.add(
            bAutoCameraRotation.set("auto-cam-orientation-enabled", false));
    
    allParameters.add(
            autoCamSpeed.set("cam-orientation-speed", ofVec3f(0,0,0), CAM_MIN_SPEED_AUTO_ORIENTATION, CAM_MAX_SPEED_AUTO_ORIENTATION));
    
    allParameters.add(
            effectOffset.set("effect-offset", EFFECT_DEFAULT_OFFSET, EFFECT_MIN_OFFSET, EFFECT_MAX_OFFSET));
    
    allParameters.add(
            effectScale.set("Scale", 1, 0, 1));
    
    allParameters.add(
            bAutoEffectRotation.set("effect-uuto-rotation-enabled", false));
   
    allParameters.add(
            effectOrientationRef.set("effect-rotation-reference", ofVec3f(0,0,0), ofVec3f(-120,-120,-120), ofVec3f(120,120,120)));
    
    allParameters.add(
            autoEffectRotSpeed.set("effect-rotation-speed", ofVec3f(0,0,0), ofVec3f(-1,-1,-1), ofVec3f(1,1,1)));
    
    allParameters.add(zTravel.set("ztravel",0));
    
    allParameters.add(effectOrientation.set("effect-rotation",ofVec3f(0,0,0)));
    allParameters.add(camOrientation.set("cam-orientation",ofVec3f(0,0,0)));
    
    allParameters.add(replicate.set("effect-replicate", ofVec3f(1,1,1), ofVec3f(1,1,1), ofVec3f(10,10,10))  );
    
    allParameters.add(replicateSpacing.set("effect-replicate-spacing", ofVec3f(1,1,1), ofVec3f(1,1,1), ofVec3f(400,400,400)));
    
    
    //allParamPanel.setup(allParameters);
    
    //directSyphon.set("Syphon direct out", 0, 0, 1);
    
    //camOffsetParam.set(ofVec2f(0,0));
    //camOrientationParam.set( ofVec3f(-uiCamOrientationXY.y, uiCamOrientationXY.x, uiCamOrientationZ) );
    
}