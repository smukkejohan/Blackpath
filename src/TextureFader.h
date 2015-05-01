//
//  TextureFader.h
//  Blackpath
//
//  Created by Johan Bichel Lindegaard on 18/02/15.
//
//

#pragma once
#include "ofMain.h"
#include "ofxTween.h"
#include "Project.h"

class TextureFader {
public:
    
    TextureFader(bool _useShader = true) {
        useShader = _useShader;
        textures.resize(3);
        transitionDuraion.set("Duration", 1000, 0, 20000);
    }
    
    void setup() {
        if(useShader) shader.load(ofToDataPath("shadersGL2/shader"));
    }
    
    void begin() {
        shader.begin();
        shader.setUniform1f("fadeToB", tween.update());
        
        if(getCurrent() && getCurrent()->isAllocated() ){
            shader.setUniformTexture("tex0", *getCurrent(), 1);
            shader.setUniform2f("aTexSize", ofVec2f(getCurrent()->getWidth(), getCurrent()->getHeight()));
        }

        if(getNext() && getNext()->isAllocated() ){
            shader.setUniform2f("bTexSize", ofVec2f(getNext()->getWidth(), getNext()->getHeight()));
            shader.setUniformTexture("tex1", *getNext(), 2);
        }
    }
    
    void end() {
        shader.end();
    }
    
    void draw(float _width, float _height, float fade = 1) {
        if(getCurrent() && getCurrent()->isAllocated()) {
            ofSetColor(255,255,255,fade*255);
            getCurrent()->draw(0, 0, _width, _height);
        }
        if(getNext() && getNext()->isAllocated()) {
            ofSetColor(255,255,255,fade*255*tween.update());
            getNext()->draw(0, 0, _width, _height);
        }
    }
    
    void update() {
        
        if(setClear) {
            setNext(NULL);
            setWait(NULL);
            // todo - fade it out
            setCurrent(NULL);
        }
        
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
    
    bool hasTexture() {
        
        for(int i=0; i<3; i++) {
            if (textures[i] && textures[i]->isAllocated()) return true;
        }
        return false;
    };
    
    ofTexture * getCurrent() {
        return textures[0];
    };
    
    void setCurrent(ofTexture * _tex) {
        textures[0] = _tex;
    }
    
    void setNext(ofTexture * _tex) {
        textures[1] = _tex;
    }
    
    void setWait(ofTexture * _tex) {
        textures[2] = _tex;
    }
    
    ofTexture * getNext() {
        return textures[1];
    };
    
    ofTexture * getWait() {
        return textures[2];
    };
    
    void clear() {
    }
    
    ofParameter<float> transitionDuraion;
    Asset asset;
    
private:
    vector<ofTexture *> textures; // current, fadeto, wait
    ofxTween tween;
    ofxEasingLinear easinglinear;
    ofShader shader;
    bool useShader;
    bool setClear;
};
