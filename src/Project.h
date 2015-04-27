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


struct Asset {
    
    Asset(){};
    Asset(string _type, int _nid) {
        type = _type;
        nid = _nid;
        isSet = true;
    };
    
    string type="";
    int nid=0;
    bool isSet = false;
};

class Scene {
public:
    
    Scene() {
        params = new Parameters();
    };
    
    string name;
    void remove();
    void load(ofxXmlSettings& settings) {
        
        name = settings.getValue("name", "");
        
        // todo load asssets
        
        // todo load parameters
        
    }
    
    void save(ofxXmlSettings& settings) {
        
        settings.addValue("name", name);
        
        // todo save asssets
        
        
        // todo save parameters
        
        
    }
    
    void exit() {
        delete params;
    };
    
    Asset landscapeTexture,
          secondaryTexture,
          effectTexture,
          skyTexture,
          landscapeModel,
          effectModel;
    
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
    
    ofTexture * getTextureAsset(Asset _asset) {
        ofTexture * ret;
        
        if(_asset.type == "syphon") {
            
            SyphonTexture * s = &syphonTextures[_asset.nid];
            
            if(s->client &&
               s->client->isSetup() &&
               s->armed &&
               s->client->getTextureRef().isAllocated()) {
                
                ret = &s->client->getTextureRef();
            }
            
        } else /* asset type texture*/ {
                ret = &textures[_asset.nid].getTexture();
        } // todo color / gen patterns
        
        return ret;
    }
    
    Model * getModelAsset(Asset _asset) {
        return &models[_asset.nid];
    }
    
    Scene * getScene(string name);
    
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
    
    void addTexture(Texture _t) {
    }
    
    void removeTexture(Texture _t) {
    }
    
    void addModel(Model _m) {
    }
    
    void removeModel(Model _m) {
    }
    
    vector<ofFile> textureQueue;
    vector<ofFile> skyQueue;
    vector<ofFile> modelQueue;
    
    Scene * activeScene; // main output
    Scene * previewScene; // interface and preview
    
    vector<Scene*>   scenes;
    
    vector<Texture> textures;
    vector<Model>   models;
    vector<SyphonTexture> syphonTextures;
    
    
private:
    string projectPath;
    ofxXmlSettings settings;
    

};
