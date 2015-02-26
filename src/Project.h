//
//  Project.h
//  Blackpath
//
//  Created by Johan Bichel Lindegaard on 18/02/15.
//
//

#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "Parameters.h"
#include "Texture.h"
#include "Model.h" 
#include "ofxThreadedImageLoader.h"
#include "defines.h"

class Scene {
public:
    
    Scene() {
        params = new Parameters();
    };
    
    string name;
    ofxXmlSettings settings;
    
    void remove();
    void save();
    
    void exit() {
        delete params;
    };
    
    Texture * selectedLandscapeTexture;
    Texture * selectedSecondaryTexture;
    Texture * selectedEffectTexture;
    Texture * selectedSkyTexture;
    
    Model * selectedLandscapeModel;
    Model * selectedEffectModel;
    
    Parameters * params;
    
private:
    
};


class Project {
    friend Scene;
public:
    
    Project() {
    };
    
    void load(string filename);
    void save();
    void save(string filename);
    
    void cloneScene(Scene cloneFromScene);
    void addScene(string name);
    void addScene();
    
    void selectScene(string name);
    
    vector<SyphonTexture> syphonTextures;
    ofxThreadedImageLoader threadImgLoader;
    ofImage tmpLoadingImage;
    
    void update();
    
    Scene * getActiveScene() {
        return activeScene;
    };
    
    Scene * getPreviewScene() {
        return activeScene;
    };
    
    ofxXmlSettings getSettings() {
        return settings;
    };
    
    vector<Scene*> getScenes() {
        return scenes;
    };
    
    void exit() {
      // save project and scenes
      // delete scene pointers
        
        threadImgLoader.stopThread();

    };
    
    void addTexture(Texture * _t) {
    }
    
    void removeTexture(Texture * _t) {
    }
    
    void addModel(Model * _m) {
    }
    
    void removeModel(Model * _m) {
    }
    
    vector<ofFile> textureQueue;
    vector<ofFile> skyQueue;
    vector<ofFile> modelQueue;
    
    Scene * activeScene;
    Scene * previewScene;
    vector<Scene*>   scenes;
    
    vector<Texture*> textures;
    vector<Model*>   models;
    
    
    
private:
    string projectPath;
    ofxXmlSettings settings;
    

};
