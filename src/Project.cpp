//
//  Project.cpp
//  Blackpath
//
//  Created by Johan Bichel Lindegaard on 18/02/15.
//
//

//char letters[] = "ABCDEFGHIJKLMNOPQRST";


#include "Project.h"

void Project::load(string _filename) {
    projectPath = ofToDataPath(_filename);
    settings.load(projectPath);
    
    /// FOR DEBUGGING - load this from project xml
    
    addScene("debug");
    
    
    activeScene = scenes.front();
    previewScene = scenes.front();
    
    textures.clear();
    textures.resize(MAX_TEXTURES);
    
    models.clear();
    models.resize(MAX_MODELS);
    
    syphonTextures.clear();
    syphonTextures.resize(MAX_SYPHON_TEXTURES);
    
    int texTags = settings.getNumTags("texture");
    for(int i=0; i<texTags; i++) {
        string path = settings.getValue("texture:path", "", i);
        ofFile f(path);
        if(f.isFile()) {
            if(textures.size() > i){
                textures[i]->load(f, &threadImgLoader);
            }
        }
    }
    
    int modTags = settings.getNumTags("model");
    for(int i=0; i<modTags; i++) {
        string path = settings.getValue("model:path", "", i);
        ofFile f(path);
        if(f.isFile()) {
            models[i]->load(f);
        }
    }
    
    
}


void Project::update() {
    // load images in parallel thread one image at a time
    if(textureQueue.size() > 0) {
        
        ofFile file = textureQueue.back();
        ofLogNotice()<<"Adding texture: "<<" "<<file.getFileName()<<endl;
        
        for(int i=0; i<textures.size(); i++) {
            if(!textures[i]->armed) {
                textures[i]->load(file, &threadImgLoader);
                break;
            }
        }
        textureQueue.pop_back();
    }
    
    if(modelQueue.size() > 0) {
        ofFile file = modelQueue.back();
        ofLogNotice()<<"Adding model: "<<" "<<file.getFileName()<<endl;
        
        // TODO: load threaded from buffer then load to model when needed by GPU
        for(int i=0; i<models.size(); i++) {
            if(!models[i]->armed) {
                models[i]->load(file);
                break;
            }
        }
        modelQueue.pop_back();
    }
    
}


void Project::save() {
    settings.save(projectPath);
}

void Project::save(string _filename) {
    projectPath = ofToDataPath(_filename);
    save();
}

void Project::addScene() {
    string n = ofToString(scenes.size());
    addScene(n);
}

/*void Project::setActiveScene(string _sceneName) {
}*/

void Project::addScene(string _name) {
    Scene * scene = new Scene();
    scene->name = _name;
    scenes.push_back(scene);
}

void Project::selectScene(string _name) {
    
    for(int i=0; i<scenes.size(); i++ ) {
        if(scenes[i]->name == _name) {
            previewScene = scenes[i];
            activeScene = scenes[i];
        }
    }
    
}


void Project::cloneScene(Scene _cloneFromScene) {
    //Scene scene = _cloneFromScene;
    scenes.push_back(&_cloneFromScene);
}
