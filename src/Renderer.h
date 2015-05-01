//
//  Renderer.h
//  Blackpath
//
//  Created by Johan Bichel Lindegaard on 2/26/15.
//
//

#pragma once

#include "ofMain.h"
#include "Project.h"
#include "ofxCubeMap.h"
#include "TextureFader.h"
#include "ModelFader.h"
#include "ofxBiquadFilter.h"
#include "ofxTween.h"
#include "Model.h"
#include "Texture.h"

/*
 Renders scenes, and handles transitions between scenes 
 A Renderer can be a main output or a preview
 */

class Renderer {
public:
    
    Renderer(string _name, Project * _project) {
        name = _name;
        project = _project;
        
        syphonOut.setName(name);
    }
    
    Project * project;
    
    void setup();
    void exit();
    
    void publishSyphon() {
        syphonOut.publishTexture(&fbo.getTexture());
    }
    
    void setOutput();
    void setupFilters();
    void update();             
    
    // Texturefaders
    TextureFader * landscapeTextureFader;
    TextureFader * secondaryTextureFader;
    TextureFader * effectTextureFader;
    TextureFader * skyTextureFader;
    
    ModelFader * landscapeFader;
    ModelFader * effectModelFader;
    
    // next current wait logic for scenes
    ofFbo fbo;
    ofCamera cam;
    ofCamera skyBoxCam;
    ofxCubeMap cubeMap;
    
    
    
    // disable enable
    // low priority for preview - preset
       // lower framerate
       // low quality
       // lower resolution
       // drop frames if framerate is getting low
    
    ofxTween effectTween;
    
    void render();
    void renderSky();
    void renderLandscape();
    void renderLandscapeVboMeshes(Model * m, float fade, bool prim);
    void renderEffectModel(Model * model, float fade);
    
    ofxBiquadFilter3f* camOrientationFilter;
    ofVec3f camOrientation;
    ofxBiquadFilter3f*    effectOrientationFilter;
    ofVec3f               effectOrientation;
    ofxBiquadFilter3f*    effectOffsetFilter;
    ofVec3f               effectOffset;
    ofxBiquadFilter3f*    camOffsetFilter;
    ofVec3f               camOffset;
    
    ofVec3f travelPos;
    ofVec3f basePos;
    ofVec3f modelOffset;
    ofVec3f modelEndOffset;
    ofVec3f landScapeRefPos;
    
    ofVec3f camRefPos;
    
    float zTravel; // Move z-travel to scene?
    
    void enable()  { enabled = true;  }
    void disable() { enabled = false; }
    
    void setTextureFromAsset(TextureFader * textureFader, Asset asset);
    void setModelFromAsset(ModelFader * modelFader, Asset asset);
    //Parameters * params;
    Scene * scene;
    
    string name;
    ofxSyphonServer syphonOut;
    
private:
    bool enabled;
};