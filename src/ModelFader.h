//
//  ModelFader.h
//  Blackpath
//
//  Created by Johan Bichel Lindegaard on 15/02/15.
//
//

#pragma once
#include "ofMain.h"
#include "Model.h"
#include "Project.h"

class ModelFader {
public:
    
    ModelFader() {
        models.resize(3);
        transitionDuraion.set("Duration", 1000, 0, 20000);
    }
    
    /*void draw() {
     ofPushMatrix();
     if(getCurrent() && getCurrent()->hasMeshes()) {
     ofScale(1-tween.update(),1-tween.update());
     getCurrent()->vboMeshes[0].draw();
     }
     ofPopMatrix();
     
     ofPushMatrix();
     if(getNext() && getNext()->hasMeshes()) {
     ofScale(tween.update(), tween.update());
     
     getNext()->vboMeshes[0].draw();
     }
     ofPopMatrix();
     }*/
    
    void update() {
        if(tween.isCompleted() && getNext() != NULL) {
            setCurrent(getNext());
            setNext(NULL);
            tween = ofxTween();
        }
        
        if(getWait() != NULL && !tween.isRunning() && getNext() == NULL) {
            setNext(getWait());
            setWait(NULL);
            tween.setParameters(1,easinglinear,ofxTween::easeIn,0.0,1.0,transitionDuraion,0);
        }
    };
    
    bool hasModel() {
        for(int i=0; i<3; i++) {
            if (models[i] && models[i]->hasMeshes()) return true;
        }
        return false;
    };
    
    void setCurrent(Model * _mod) {
        models[0] = _mod;
    }
    
    void setNext(Model * _mod) {
        models[1] = _mod;
    }
    
    void setWait(Model * _mod) {
        models[2] = _mod;
    }
    
    Model * getCurrent() {
        return models[0];
    };
    
    Model * getNext() {
        return models[1];
    };
    
    Model * getWait() {
        return models[2];
    };
    
    void clear() {
        bClear = true;
    };
    
    ofParameter<float> transitionDuraion;
    
    ofxTween tween;
    Asset asset;
    
private:
    bool bClear = false;
    vector<Model *> models; // current, fadeto, wait
    ofxEasingLinear easinglinear;
    string transitionType = "fade"; //todo: scale, morph
    
};
