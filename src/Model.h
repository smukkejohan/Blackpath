//
//  Model.h
//  Blackpath
//
//  Created by Johan Bichel Lindegaard on 15/02/15.
//
//

#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

class Model : public ofxAssimpModelLoader {
public:
    
    Model() {
        //thumbFbo.allocate(50, 50);
        thumbCam.setupPerspective();
    }
    
    void update() {
        if(bChanged) {
            //getThumb();
        }
    }
    
    void load(ofFile _file) {
        file = _file;
        loadModel(file.getAbsolutePath());
        setScaleNormalization(false); //todo normalize everything ?
        /*for(int i=0; i < getMeshCount(); i++) {
         vboMeshes.push_back(getMesh(i));
         }*/
        
        bChanged = true;
        armed = true;
    }
    
    bool armed = false;
    ofFile file;
    
    /*ofImage * getThumb(int _width=50, int _height=50) {
        if(hasMeshes()) {
            if(!bChanged && thumb.isAllocated() && _width == thumb.getWidth() && _height == thumb.getHeight()) return &thumb;
            
            ofEnableLighting();
            thumbCam.setPosition(getSceneCenter());
            
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
            thumb.resize(_width, _height);
            bChanged = false;
        }
        return &thumb;
    };*/
    
    void setChanged() {
        bChanged = true;
    }
    
    void drawVboMesh(int i, int renderType=OF_MESH_FILL) {
        if(!this) return;
        if(!(modelMeshes.size() >= i)) return;
        
        ofxAssimpMeshHelper * mesh = &modelMeshes[i];
        if(!mesh->mesh) return;
        
        ofPushStyle(); {
            ofFill();
            ofSetColor(255);
            
            ofPushMatrix(); {
                ofMultMatrix(modelMatrix);
                
                    if(mesh->indices.size() > 0) {
                
                    ofPushMatrix(); {
                        ofMultMatrix(mesh->matrix);
                        
                        if(mesh->twoSided) {
                            glEnable(GL_CULL_FACE);
                        }
                        else {
                            glDisable(GL_CULL_FACE);
                        }
                        
                        ofEnableBlendMode(mesh->blendMode);
                        
                        switch(renderType){
                            case OF_MESH_FILL:
                                mesh->vbo.drawElements(GL_TRIANGLES,mesh->indices.size());
                                break;
                            case OF_MESH_WIREFRAME:
                                mesh->vbo.drawElements(GL_LINES,mesh->indices.size());
                                break;
                            case OF_MESH_POINTS:
                                mesh->vbo.drawElements(GL_POINTS,mesh->indices.size());
                                break;
                        }
                    }ofPopMatrix();
                }
            }ofPopMatrix();
        }ofPopStyle();
    }
    
private:
    ofFbo thumbFbo;
    ofImage thumb;
    ofCamera thumbCam;
    bool bChanged;
};

