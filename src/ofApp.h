#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxAssimpModelLoader.h"
#include "ofxThreadedImageLoader.h"
#include "ofxUI.h"

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
#define MAX_SKY_IMAGES 24
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

class SkyImage : public BaseImage {
public:
};

class Model : public ofxAssimpModelLoader {
public:
    
    Model() {
        thumbFbo.allocate(200, 200);
        thumb = new ofImage();
    }
    
    ofImage * getThumb(int _width=50, int _height=50) {
        if(!bChanged && thumb->isAllocated() && _width == thumb->getWidth() && _height == thumb->getHeight()) return thumb;
        
        thumbFbo.begin();
        thumbCam.begin();
        ofBackground(100, 100, 100);
        ofSetColor(255,255,255);
        drawFaces();
        thumbCam.end();
        thumbFbo.end();
        
        ofPixels pix;
        thumbFbo.readToPixels(pix);
        thumb->setFromPixels(pix);
        // todo: maintain aspect ratio with cropping
        thumb->resize(_width, _height);
        bChanged = false;
        return thumb;
    };
    
private:
    ofFbo thumbFbo;
    ofImage * thumb;
    ofCamera thumbCam;
    bool bChanged;
};


class Thumb {
public:
    int num;
    
    ofRectangle rect;
    bool bChanged;
    
    ofImage * img;
    
    void draw() {
        if(img->isAllocated()){
            img->draw(rect);
        }
    }

};

class ThumbSelector {
public:
    
    ThumbSelector(int _w, int _h, int _thumbW, int _thumbH){
        rect.width = _w;
        rect.height = _h;
        thumbWidth = _thumbW;
        thumbHeight = _thumbH;
        padding = 6;
    };
    
    void update() {
        // todo only update an fbo when needed
    }
    
    void draw(int _x=0, int _y=0) {
        rect.x = _x;
        rect.y = _y;
        
        int thumbsPerCol = (rect.width-(padding*2)) / (thumbWidth+padding);
        
        //for(int c=0;c<cols;c++) {
        //}
        int cn = 0;
        
        for(int i=0; i<thumbs.size(); i++) {
            int row = i/thumbsPerCol;
            int xin = padding + (i - ((row)*thumbsPerCol))*(thumbWidth+padding);
            int yin = padding + (row*(thumbHeight+padding));
            
            ofNoFill();
            thumbs[i]->rect.set(xin, yin, thumbWidth, thumbHeight);
            thumbs[i]->draw();
            ofRect(xin, yin, thumbWidth, thumbHeight);
        }
        
    }
    
    //int rows;
    //int columns;

    int padding;
    int thumbWidth;
    int thumbHeight;
    
    ofRectangle rect;
    vector<Thumb *> thumbs;
    
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
    
        ofFbo masterOut;
        ofCamera cam;
        ofxThreadedImageLoader threadImgLoader;
    
        vector<ofFile> textureQueue;
        vector<ofFile> skyQueue;
        vector<ofFile> modelQueue;
    
        ThumbSelector * textureSelector;
        ThumbSelector * modelSelector;
        ThumbSelector * skyboxSelector;
    
        vector<ModelTexture> modelTextures;
        vector<SkyImage> skyImages;
        vector<Model> models;
        int totalTextures, totalModels, totalSkyimages;
    
        ModelTexture * activeTexture;
        SkyImage * activeSky;
    
        ofEasyCam easyCam;
    ofLight light;
    
    ofxUICanvas * gui;
    void guiEvent(ofxUIEventArgs &e);
    
    ofImage tmpLoadingImage;

    
};
