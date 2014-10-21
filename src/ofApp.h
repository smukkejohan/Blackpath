#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxAssimpModelLoader.h"
#include "ofxThreadedImageLoader.h"
#include "ofxUI.h" // probably decide on one of theese - Use parameters and parameterGroups
#include "ofxGui.h"
// 1. load images an create thumbnails
// 2. load models and create thumbnails
// 3. put texture on models
// 4. create camera animation
// 5. create nice gui selector

static const string _AIE[] = {"png","jpg","jpeg"};
static const vector<string> ACCEPTED_IMAGE_EXTENSIONS(_AIE, _AIE + (sizeof _AIE / sizeof _AIE[0]));

static const string _AME[] = {"3ds","dxf","obj", "c4d"};
static const vector<string> ACCEPTED_MODEL_EXTENSIONS(_AME, _AME + (sizeof _AME / sizeof _AME[0]));

#define MAX_MODEL_TEXTURES 24
#define MAX_SKY_TEXTURES 24
#define MAX_MODELS 24

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
    
    void setChanged() {
        bChanged = true;
    };
    
private:
    ofImage thumb;
    bool bChanged;
};

class ModelTexture : public BaseImage {
public:
};

class SkyTexture : public BaseImage {
public:
};

class Model : public ofxAssimpModelLoader {
public:
    
    Model() {
        thumbFbo.allocate(200, 200);
    }
    
    void update() {
        if(bChanged) {
            getThumb();
        }
    }
    
    ofImage * getThumb(int _width=50, int _height=50) {
        if(hasMeshes()) {
        if(!bChanged && thumb.isAllocated() && _width == thumb.getWidth() && _height == thumb.getHeight()) return &thumb;
        
        thumbFbo.begin();
        thumbCam.begin();
        ofBackground(100, 100, 100);
        ofSetColor(255,255,255);
        vboMesh.drawFaces();
        thumbCam.end();
        thumbFbo.end();
        
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
    
    ofVboMesh vboMesh;
    
private:
    ofFbo thumbFbo;
    ofImage thumb;
    ofCamera thumbCam;
    bool bChanged;
};


class Thumb {
public:
    int num;
    
    ofRectangle rect;
    bool bChanged;
    
    ofImage * img;
    
    void update() {
        
    };
    
    void draw() {
        if(img->isAllocated()){
            img->draw(rect);
        }
        ofRectRounded(rect, 5);

    };
    
};

class ThumbSelector {
public:
    
    ThumbSelector(int _w, int _h, int _thumbW, int _thumbH, string _title = "Selector"){
        rect.width = _w;
        rect.height = _h;
        thumbWidth = _thumbW;
        thumbHeight = _thumbH;
        padding = 6;
        title = _title;
        center = true;
    };
    
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
        
        if(center) {
            float rowWidth = (thumbsPerRow * (thumbWidth+padding)) + (padding*2);

            masterPadding = (getWidth()-rowWidth)/2;
        }
        
        for(int i=0; i<thumbs.size(); i++) {
            int row = i/thumbsPerRow;
            int xin = masterPadding + rect.x + padding + (i - ((row)*thumbsPerRow))*(thumbWidth+padding);
            int yin = rect.y + padding + (row*(thumbHeight+padding));
            
            ofNoFill();
            thumbs[i]->rect.set(xin, yin, thumbWidth, thumbHeight);
            thumbs[i]->draw();
        }
    }
    
    float getHeight() {
        return rect.height;
    }
    
    float getWidth() {
        return rect.width;
    }
    
    //int rows;
    //int columns;
    vector<Thumb *> thumbs;

    
private:
    bool center;
    string title;
    float padding;
    float thumbWidth;
    float thumbHeight;
    float masterPadding;
    ofRectangle rect;
    
};

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
    
    // todo:
    // master fader
    // syphon in
    
    ofFbo masterOutFbo;
    ofCamera cam;
    ofxThreadedImageLoader threadImgLoader;
    
    vector<ofFile> textureQueue;
    vector<ofFile> skyQueue;
    vector<ofFile> modelQueue;
    
    ThumbSelector * textureSelector;
    ThumbSelector * skyboxSelector;
    ThumbSelector * modelSelector;
    
    vector<ModelTexture> modelTextures;
    vector<SkyTexture> skyTextures;
    vector<Model> models;
    int totalTextures, totalModels, totalSkyimages;
    
    ModelTexture * activeModelTexture;
    SkyTexture * activeSkyTexture;
    
    ofEasyCam easyCam;
    ofLight light;
    
    ofxUICanvas * gui;
    void guiEvent(ofxUIEventArgs &e);
    
    ofImage tmpLoadingImage;
    
    ofxSyphonServer syphonOut;
    
    // cam settings
    ofParameter<float> camFarClip;
    ofParameter<ofVec3f> camOrientation;
    ofParameter<float> camSpeed;
    ofParameter<float> camFov;
    
    ofParameterGroup camParams;
    
    
    ofxPanel guiPanel;
    
    
};
