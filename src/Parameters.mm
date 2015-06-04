//
//  params.cpp
//  Blackpath
//
//  Created by Johan Bichel Lindegaard on 18/02/15.
//
//

#include "Parameters.h"


void Parameters::reset() {
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
            camOrientationRef.set("cam-orientation-reference", ofVec3f(0,0,0), CAM_MIN_ORIENTATION, CAM_MAX_ORIENTATION));
    
    // direction and speed of automatic camera rotation
    allParameters.add(
            bAutoCameraRotation.set("auto-cam-orientation-enabled", false));
    
    allParameters.add(
            autoCamSpeed.set("cam-orientation-speed", ofVec3f(0,0,0), CAM_MIN_SPEED_AUTO_ORIENTATION, CAM_MAX_SPEED_AUTO_ORIENTATION));
    
    allParameters.add(
            effectOffset.set("effect-offset", EFFECT_DEFAULT_OFFSET, EFFECT_MIN_OFFSET, EFFECT_MAX_OFFSET));
    
    allParameters.add(
            effectScale.set("Scale", 1, 0, 10));
    
    allParameters.add(
            bAutoEffectRotation.set("effect-auto-rotation-enabled", false));
   
    allParameters.add(
            effectOrientationRef.set("effect-rotation-reference", ofVec3f(0,0,0), ofVec3f(-180,-180,-180), ofVec3f(180,180,180)));
    
    allParameters.add(
            autoEffectRotSpeed.set("effect-rotation-speed", ofVec3f(0,0,0), ofVec3f(-100,-100,-100), ofVec3f(100,100,100)));
    
    allParameters.add(zTravel.set("ztravel",0));
    
    allParameters.add(effectOrientation.set("effect-rotation",ofVec3f(0,0,0)));
    allParameters.add(camOrientation.set("cam-orientation", ofVec3f(0,0,0), ofVec3f(-90,-90,-90), ofVec3f(90,90,90)));
    
    allParameters.add(replicate.set("effect-replicate", ofVec3f(1,1,1), ofVec3f(1,1,1), ofVec3f(10,10,10))  );
    
    allParameters.add(replicateSpacing.set("effect-replicate-spacing", ofVec3f(1,1,1), ofVec3f(1,1,1), ofVec3f(600,600,600)));
    
    
    //allParamPanel.setup(allParameters);
    
    //directSyphon.set("Syphon direct out", 0, 0, 1);
    
    //camOffsetParam.set(ofVec2f(0,0));
    //camOrientationParam.set( ofVec3f(-uiCamOrientationXY.y, uiCamOrientationXY.x, uiCamOrientationZ) );
    
}