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


// Asset is an intermediate object containing metadata
// to link scene assets to the correct project data containers.
// A unique asset is defined by an id (nid) and a type.
// I have done it this way to decaouple things as much as possible and to avoid problems with memory addresses.
// At this time the nid is also the index in vectors for the given type. TODO: Don't depend on that.

struct Asset {
    
    Asset(){};
    Asset(string _type, int _nid) {
        type = _type;
        nid = _nid;
        //isSet = true;
    };
    
    Asset(string _name, ofxXmlSettings& s) {
        s.pushTag(_name);
        type = s.getValue("type", "none");
        nid = s.getValue("nid", -1);
        s.popTag();
        //isSet = true;
    };
    
    string type="none";
    int nid=-1;
    //bool isSet = false;
    
    void saveAsset(string title, ofxXmlSettings& s) {
        s.addTag(title);
        s.pushTag(title);
        s.addValue("type", type);
        s.addValue("nid", nid);
        s.popTag();
    }
    
};

class Scene {
public:
    
    Scene() {
        params = new Parameters();
        params->init();
    };
    
    string name;
    void remove();
    
    
    void load(ofxXmlSettings& s) {
        
        name = s.getValue("name", "");
        // settings.getValue
        // todo load asssets
        // todo load parameters
        landscapeTexture = Asset("landscapeTexture", s);
        secondaryTexture = Asset("secondaryTexture", s);
        effectTexture = Asset("effectTexture", s);
        skyTexture = Asset("skyTexture", s);
        landscapeModel = Asset("landscapeModel", s);
        effectModel = Asset("effectModel", s);
        
        //params->allParameters.fromString(s.getValue("parameters",""));
        
        s.pushTag("parameters");
        for(int i=0; i<params->allParameters.size(); i++ ) {
            
            params->allParameters[i].fromString(s.getValue(params->allParameters[i].getEscapedName(),""));
            
            //cout<<params->allParameters[i].toString()<<endl;
            
        }
        s.popTag();
        
        
    }
    
    void save(ofxXmlSettings& s) {
        
        s.addValue("name", name);
        
        
        // save active assets
        landscapeTexture.saveAsset("landscapeTexture", s);
        secondaryTexture.saveAsset("secondaryTexture", s);
        effectTexture.saveAsset("effectTexture", s);
        skyTexture.saveAsset("skyTexture", s);
        landscapeModel.saveAsset("landscapeModel", s);
        effectModel.saveAsset("effectModel", s);
        
        // save parameters
        
        
        
        //
        
        
            s.addTag("parameters");
        s.pushTag("parameters");
        for(int i=0; i<params->allParameters.size(); i++ ) {
            s.addValue(params->allParameters[i].getEscapedName(),params->allParameters[i].toString());
            
            //cout<<params->allParameters[i].toString()<<endl;

            
            
        }
        s.popTag();
        
        
        //s.addValue("parameters",params->allParameters.toString());
        
        //cout<<params->allParameters.toString()<<endl;
        
        //params->allParameters.toString()
        
        
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
    
    void setup();
    void load(string filename);
    void save();
    void save(string filename);
    
    void cloneScene(Scene cloneFromScene);
    void addScene(string name);
    void addScene();
    
    void removeScene(Scene * _scene);
    
    //todo use ref instead of pointer maybe
    ofTexture * getTextureAsset(Asset _asset) {
        
        if(_asset.type == "syphon") {
            
            SyphonTexture * s = &syphonTextures[_asset.nid];
            
            if(s->client &&
               s->client->isSetup() &&
               s->armed &&
               s->client->getTextureRef().isAllocated()) {
                
                return &s->client->getTextureRef();
            }
            
        } else if( _asset.type == "texture" ) {
            
            // todo loop through and match nid instead of depend on location
            
                return &textures[_asset.nid].getTexture();
        } else if(_asset.type == "none") {
            return &defaultTexture.getTexture();
        }
        
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
    ofImage defaultTexture;
    
    bool bTextureAssetsChanged = false;
    bool bModelAssetsChanged = false;
    
    
    int outWidth, outHeight;
    bool enablePreview;
    
    
private:
    string projectPath;
    ofxXmlSettings settings;
    

};
