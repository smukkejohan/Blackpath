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
#include "ofxBiquadFilter.h"

// 1. load images an create thumbnails
// 2. load models and create thumbnails
// 3. put texture on models
// 4. create camera animation
// 5. create nice gui selector

#define MAX_TEXTURES 42
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
    
};

class SyphonTexture {
public:
    // get Thumb
    // name - title
    ofxSyphonClient * client;
    
    SyphonTexture() {
        thumbFbo.allocate(50, 50);
        client = NULL;
    };
    
    void arm(ofxSyphonClient * _client) {
        client = _client;
        //_client->setup();
        //fbo.allocate(_client->getWidth(), _client->getHeight());
        armed = true;
        bChanged = true;
    }
    
    void update() {
        if(bChanged) {
            getThumb();
        }
    };
    
    // todo: reset thumb on update
    ofImage * getThumb(int _width=50, int _height=50) {
        
        if(!bChanged && _width == thumb.getWidth() && _height == thumb.getHeight()) return &thumb;
        
            thumbFbo.begin();
            if(client && client->isSetup() && client->getTexture().isAllocated()) {
                //todo - this is not working
                //client->draw(0,0);
                ofBackground(0, 255, 0);
            } else {
                ofBackground(255, 0, 0);
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
        int rows = ceil(thumbs.size()/thumbsPerRow);
        float columnHeight = (rows * (thumbHeight+padding)) + padding;
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
    void mouseScrolled(ofMouseEventArgs &args){
        
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

class ModelFader {
public:
    
    ModelFader() {
        models.resize(3);
        transitionDuraion.set("Duration", 1000, 0, 20000);
    }
    
    void draw() {
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
    }
    
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

private:
    bool bClear;
    vector<Model *> models; // current, fadeto, wait
    ofxEasingLinear easinglinear;
    string transitionType = "fade"; //todo: scale, morph
    
};

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
            
        if(getNext() &&  getNext()->isAllocated() ){
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
    
private:
    vector<ofTexture *> textures; // current, fadeto, wait
    ofxTween tween;
    ofxEasingLinear easinglinear;
    ofShader shader;
    bool useShader;
    bool setClear;
    
};

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
    
    ofFbo masterOutFbo;
    ofCamera cam;
    ofCamera skyboxcam;
    ofxThreadedImageLoader threadImgLoader;
    
    float zTravel;
    ofVec3f camRefPos;
    
    vector<ofFile> textureQueue;
    vector<ofFile> skyQueue;
    vector<ofFile> modelQueue;
    
    ThumbSelector * textureSelector;
    ThumbSelector * modelSelector;
    
    vector<Texture> textures;
    vector<Model> models;
    
    int totalTextures, totalModels, totalSkyimages;
    
    TextureFader landscapeTextureFader;
    TextureFader secondaryTextureFader;
    TextureFader effectTextureFader;
    TextureFader skyTextureFader;
    
    //deque<Model *> activeLandscapes;
    
    ModelFader landscapeFader;
    ModelFader effectModelFader;
    
    ofLight light;
    
    ofVec3f travelPos;
    ofVec3f basePos;
    ofVec3f modelOffset;
    ofVec3f modelEndOffset;
    
    ofVec3f landScapeRefPos;
    
    ofxCubeMap cubeMap;
    ofxUICanvas * gui;
    void guiEvent(ofxUIEventArgs &e);
    ofImage tmpLoadingImage;
    ofxSyphonServer syphonOut;
    
    // cam settings
    ofParameter<float> camFarClip;
    ofParameter<ofVec3f> camOrientationRef;
    ofParameter<ofVec3f> camOffset;
    ofParameter<float> camSpeed;
    ofParameter<float> camFov;
    ofParameter<float> camNearClip;
    
    ofParameter<float> directSyphon;
    ofParameter<float> blackout;
    
    ofParameter<ofVec3f> effectOffset;
    ofParameter<float>  effectScale;
    
    // todo:
    ofParameter<ofVec3f> effectOrientation;
    ofParameter<ofVec3f> effectReplicator;
    
    ofParameter<float> autoSpeed;
    
    ofParameter<bool> autoCameraRotation;
    
    ofParameter<ofVec3f> autoCamSpeed;
    
    ofParameter<ofVec3f> autoEffectRotSpeed;
    
    ofParameter<bool> autoCameraOffset;
    
    ofParameter<bool> autoEffectRotation;
    ofParameter<bool> autoEffectScale;
    
    ofParameter<bool> clearLandscape;
    ofParameter<bool> clearEffect;
    
    ofParameterGroup camParams;
    
    ofParameterGroup effectParams;
    
    ofParameterGroup landscapeParams;
    
    ofParameterGroup otherParams;
    
    ofxPanel guiPanel;
    
    ofxSyphonServerDirectory dir;
    vector<SyphonTexture> syphonTextures;
    
    int dirIdx;
    void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
    void serverUpdated(ofxSyphonServerDirectoryEventArgs &args);
    void serverRetired(ofxSyphonServerDirectoryEventArgs &arg);
    
    ofFbo syphonInFbo;
    
    ofxXmlSettings settings;
    
    ofFbo testfbo;
    
    ofxTween effectTween;
    
    void drawLandscape();
    void drawLandscapeVboMeshes(Model * m, float fade, bool prim);
    
    
    void drawEffectModel(Model * model, float fade);

    ofxBiquadFilter3f camOrientationFilter;
    ofVec3f camOrientation;

    ofxBiquadFilter3f effectOrientationFilter;
    
    
};
