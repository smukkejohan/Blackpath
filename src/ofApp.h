#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxAssimpModelLoader.h"
#include "ofxThreadedImageLoader.h"
#include "ofxUI.h" // probably decide on one of theese - Use parameters and parameterGroups
#include "ofxGui.h"
#include "ofxCubeMap.h"
#include "ofxXmlSettings.h"
#include "ofxTween.h"

// 1. load images an create thumbnails
// 2. load models and create thumbnails
// 3. put texture on models
// 4. create camera animation
// 5. create nice gui selector

#define MAX_TEXTURES 48
#define MAX_SYPHON_TEXTURES 3
#define MAX_COLOR_TEXTURES 3
#define MAX_MODELS 36

static const string _AIE[] = {"png","jpg","jpeg"};
static const vector<string> ACCEPTED_IMAGE_EXTENSIONS(_AIE, _AIE + (sizeof _AIE / sizeof _AIE[0]));

static const string _AME[] = {"obj"};
static const vector<string> ACCEPTED_MODEL_EXTENSIONS(_AME, _AME + (sizeof _AME / sizeof _AME[0]));

class BaseImage : public ofImage {
public:
    
    BaseImage() {
        //loadImage("defaultImg.png");
    }
    
    void update() {
        if(bChanged) {
            getThumb();
        }
    }
    
    // todo: reset thumb on update
    ofImage * getThumb(int _width=50, int _height=50) {
        if(isAllocated() && isUsingTexture())  {
            if(!bChanged && thumb.isAllocated() && _width == thumb.getWidth() && _height == thumb.getHeight()) return &thumb;
            
            thumb.setFromPixels(getPixels());
            // todo: maintain aspect ratio with cropping
            // todo: cache thumbs in different sizes
            // this resize is resource hunry - optimize it maybe make it threaded
            thumb.resize(_width, _height);
            
            bChanged = false;
        }
        
        return &thumb;
    };
    
    void load(ofFile _file, ofxThreadedImageLoader * threadLoader) {
        file = _file;
        
        threadLoader->loadFromDisk(*(this), file.getAbsolutePath());
        
        armed = true;
        bChanged = true;
    }
    
    bool armed = false;
    
    void setChanged() {
        bChanged = true;
    };
    
    ofFile file;
    
private:
    ofImage thumb;
    bool bChanged;
};


class Texture : public BaseImage {
public:
    
    
    
    // todo: getSyphonTexture
};

class SyphonTexture {
public:
    // get Thumb
    // name - title
    ofxSyphonClient * client;
    
    SyphonTexture() {
        thumbFbo.allocate(50, 50);
    };
    
    void arm(ofxSyphonClient * _client) {
        client = _client;
        //_client->setup();
        //fbo.allocate(_client->getWidth(), _client->getHeight());
        armed = true;
    }
    
    void update() {
        if(bChanged) {
            getThumb();
        }
    };
    
    void updateFbo() {
       /* if(client && client->isSetup()) {
            fbo.begin();
            client->draw(0,0);
            fbo.end();
        }*/
    }
    
    // todo: reset thumb on update
    ofImage * getThumb(int _width=50, int _height=50) {
        
        if(!bChanged && _width == thumb.getWidth() && _height == thumb.getHeight()) return &thumb;
        
            thumbFbo.begin();
            if(client && client->isSetup()) {
                ofBackground(255, 0, 0);
            } else {
                ofBackground(0, 255, 0);
            }
            ofSetColor(255,255,255);
            thumbFbo.end();
            
            ofPixels pix;
            thumbFbo.readToPixels(pix);
            thumb.setFromPixels(pix);
            // todo: maintain aspect ratio with cropping
            thumb.resize(_width, _height);
            bChanged = false;
    
            return &thumb;
    };
    
    bool armed = false;
    //ofFbo fbo;

private:
    string appName;
    string serverName;
    ofImage thumb;
    bool bChanged;
    ofFbo thumbFbo;
    //bool active;
};


class Model : public ofxAssimpModelLoader {
public:
    
    Model() {
        thumbFbo.allocate(50, 50);
        thumbCam.setupPerspective();
    }
    
    void update() {
        if(bChanged) {
            getThumb();
        }
    }
    
    void load(ofFile _file) {
        file = _file;
        loadModel(file.getAbsolutePath());
        
        for(int i=0; i < getMeshCount(); i++) {
            vboMeshes.push_back(getMesh(i));
        }
        
        bChanged = true;
        armed = true;
    }
    
    bool armed = false;
    ofFile file;
    
    ofImage * getThumb(int _width=50, int _height=50) {
        if(hasMeshes()) {
        if(!bChanged && thumb.isAllocated() && _width == thumb.getWidth() && _height == thumb.getHeight()) return &thumb;
        
        ofEnableLighting();
        thumbCam.setPosition(vboMeshes[0].getCentroid().x, 0, 0);

        thumbFbo.begin();
        thumbCam.begin();
        ofBackground(100, 100, 100);
        ofSetColor(255,255,255);
            drawFaces();
        thumbCam.end();
        thumbFbo.end();
        
        ofDisableLighting();
        ofPixels pix;
        thumbFbo.readToPixels(pix);
        thumb.setFromPixels(pix);
        // todo: maintain aspect ratio with cropping
        thumb.resize(_width, _height);
        bChanged = false;
        }
        return &thumb;
    };
    
    void setChanged() {
        bChanged = true;
    }
    
    vector<ofVboMesh> vboMeshes;
    
private:
    ofFbo thumbFbo;
    ofImage thumb;
    ofCamera thumbCam;
    bool bChanged;
};

class Thumb {
    
public:
    int nid;
    ofRectangle rect;
    bool bChanged;
    string type;
    
    ofImage * img;
    bool active;
    
    void update() {
        
    };
    
    void draw() {
        ofPushMatrix();
        ofPushStyle();
        if(hover || selected) {
            ofSetColor(255,255,255);
        } else {
            ofSetColor(100,100,100);
        }
        
        ofRectRounded(rect, 5);
        
        if(img->isAllocated()){
            img->draw(rect);
        }
        
        if(pressed || selected) {
            ofRectangle pressedRect = rect;
            pressedRect.scaleFromCenter(0.9);
            ofRectRounded(pressedRect, 5);
        } else {

        }
        
        ofPopStyle();
        ofPopMatrix();
        
    };
    
    bool hover;
    bool selected; // might need multiple states
    bool pressed;
    //ofEvent selected;
};


class ThumbSelectorEventData {
public:
    ThumbSelectorEventData() {
    }
    string title;
    string event;
    Thumb * thumb;
    int thumbNum;
};

class ThumbSelector {
    friend Thumb;
    
public:
    
    ThumbSelector(int _w, int _h, int _thumbW, int _thumbH, string _title = "Selector"){
        rect.width = _w;
        rect.height = _h;
        thumbWidth = _thumbW;
        thumbHeight = _thumbH;
        padding = 6;
        title = _title;
        center = false;
        ofRegisterMouseEvents(this);
    };
    
    ~ThumbSelector() {
        ofUnregisterMouseEvents(this);
    };
    
    ofEvent<ThumbSelectorEventData> thumbSelectEvent;
    
    void update() {
        // todo only update an fbo when needed
        
        for(int i=0; i<thumbs.size(); i++) {
            thumbs[i]->update();
        }
    };
    
    void draw(int _x=0, int _y=0) {
        rect.x = _x;
        rect.y = _y;
        
        int thumbsPerRow = (rect.width-(padding*2)) / (thumbWidth+padding);
        float rowWidth = (thumbsPerRow * (thumbWidth+padding)) + (padding);
        float columnHeight = ((thumbs.size()/thumbsPerRow) * (thumbHeight+padding)) + padding;
        masterPadding = 0;
        if(center) {
            masterPadding = (getWidth()-rowWidth)/2; // X padding
        }
        
        innerRect.set(rect.x, rect.y, rowWidth, columnHeight);
        
        ofSetColor(60,60,60);
        ofRect(rect);
        
        for(int i=0; i<thumbs.size(); i++) {
            int row = i/thumbsPerRow;
            int xin = masterPadding + rect.x + padding + (i - ((row)*thumbsPerRow))*(thumbWidth+padding);
            int yin = rect.y + padding + (row*(thumbHeight+padding));
            
            ofNoFill();
            thumbs[i]->rect.set(xin, yin, thumbWidth, thumbHeight);
            thumbs[i]->draw();
        }
        
        if(hover) {
            ofSetColor(255,255,255);
            ofRectRounded(innerRect, 5);
        } else {
            ofSetColor(100,100,100);
            ofRectRounded(innerRect, 5);
        }
        
    };
    
    float getHeight() {
        return rect.height;
    };
    
    float getWidth() {
        return rect.width;
    };
    
    void mouseMoved(ofMouseEventArgs &args){
        // if mouse on top highlight
        hover = rect.inside(args.x, args.y);
        for(int i=0; i< thumbs.size(); i++) {
            thumbs[i]->hover = thumbs[i]->rect.inside(args.x, args.y);
        }
    };
    
    void mouseReleased(ofMouseEventArgs &args) {
            for(int i=0; i< thumbs.size(); i++) {
                thumbs[i]->selected = false;

                if(thumbs[i]->pressed) {
                    
                    thumbs[i]->pressed = false;

                    if(thumbs[i]->rect.inside(args.x, args.y)) {
                        
                        thumbs[i]->selected = true;
                        
                        ThumbSelectorEventData eData;
                        eData.title = title;
                        eData.thumb = thumbs[i];
                        eData.event = "inside";
                        eData.thumbNum = i;
                        
                        ofNotifyEvent(thumbSelectEvent, eData, this);

                    } else {
                        ThumbSelectorEventData eData;
                        eData.title = title;
                        eData.thumb = thumbs[i];
                        eData.event = "outside";
                        eData.thumbNum = i;
                        
                        ofNotifyEvent(thumbSelectEvent, eData, this);
                    }
                }
            }
    };
    
    void mousePressed(ofMouseEventArgs &args){
        if(rect.inside(args.x, args.y)) {
            for(int i=0; i< thumbs.size(); i++) {
                
                if(thumbs[i]->rect.inside(args.x, args.y)) {
                    thumbs[i]->pressed = true;
                } else {
                    thumbs[i]->pressed = false;
                }
            }
        }
    };
    
    void mouseDragged(ofMouseEventArgs &args){
    };
    
    Thumb * getSelected() {
        return selectedThumb;
    };
    
    //int rows;
    //int columns;
    vector<Thumb *> thumbs;

    
private:
    Thumb * selectedThumb;
    bool center;
    string title;
    float padding;
    float thumbWidth;
    float thumbHeight;
    float masterPadding;
    bool hover;
    ofRectangle rect;
    ofRectangle innerRect;
    
};

/*class crossFadeTextureShader : public ofShader {
public:
    void setup();

private:
    ofTexture * texA;
    ofTexture * texB;
    
};*/

//class SkyBoxImage
//class ModelTextureImage

//class Base3DModel
// load method creates a thumbnail
// check if image loading stalls maybe do load image threaded

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void thumbEventListener(ThumbSelectorEventData& args);
    // todo:
    // master fader
    // syphon in
    
    ofFbo masterOutFbo;
    ofCamera cam;
    ofCamera skyboxcam;
    ofxThreadedImageLoader threadImgLoader;
    
    vector<ofFile> textureQueue;
    vector<ofFile> skyQueue;
    vector<ofFile> modelQueue;
    
    ThumbSelector * textureSelector;
    ThumbSelector * modelSelector;
    
    vector<Texture> textures;
    vector<Model> models;
    
    int totalTextures, totalModels, totalSkyimages;
    
    vector<Texture *> activeSecondaryTextures;
    //vector<Texture *> activeModelTextures;
    
    ofTexture * landTexWait;
    ofTexture * landTexCurrent;
    ofTexture * landTexFadeTo;
    
    // todo same thing as with landTex
    vector<Texture *> activeSkyTextures;
    vector<Texture *> activeEffectTextures;
    
    float landscapeTexTransitionP = 0;
    float transitionTime = 10000;
    unsigned int long landscapeTransitionBegin;
    bool aToB = true;
    
    ofxTween landscapeTexTween;
    ofxEasingLinear 	easinglinear;
    
    vector<Model *> activeLandscapes;
    
    vector<Model *> activeEffectModels;
        
    ofEasyCam easyCam;
    ofLight light;
    
    ofVec3f travelPos;
    ofVec3f basePos;
    ofVec3f modelOffset;
    
    ofxCubeMap cubeMap;
    ofxUICanvas * gui;
    void guiEvent(ofxUIEventArgs &e);
    ofImage tmpLoadingImage;
    
    ofxSyphonServer syphonOut;
    
    // cam settings
    ofParameter<float> camFarClip;
    ofParameter<ofVec3f> camOrientation;
    ofParameter<ofVec3f> camOffset;
    ofParameter<float> camSpeed;
    ofParameter<float> camFov;
    
    ofParameter<bool> useSyphon;
    
    ofParameter<bool> directSyphon;
    ofParameter<bool> blackout;
    
    ofParameter<ofVec3f> effectOffset;
    ofParameter<float>  effectScale;
    
    ofParameterGroup camParams;
    
    ofxPanel guiPanel;
    
    ofxSyphonServerDirectory dir;
    vector<SyphonTexture> syphonTextures;
    
    int dirIdx;
    void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
    void serverUpdated(ofxSyphonServerDirectoryEventArgs &args);
    void serverRetired(ofxSyphonServerDirectoryEventArgs &arg);
    
    ofFbo syphonInFbo;
    
    ofxXmlSettings settings;
    
    ofShader landscapeTextureShader;
    ofShader secondaryTextureShader;
    ofShader effectTextureShader;
    ofShader skyBoxTextureShader;
    
    ofFbo testfbo;
    
    //ofxSyphonClient syphonClient;
    //ofxSyphonClient syphonClient2;


};
