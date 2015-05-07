//
//  Renderer.cpp
//  Blackpath
//
//  Created by Johan Bichel Lindegaard on 2/26/15.
//
//

#include "Renderer.h"

void Renderer::setupFilters() {
    camOrientationFilter    = new ofxBiquadFilter3f(OFX_BIQUAD_TYPE_LOWPASS, 0.01, 0.7, 0.0);
    effectOrientationFilter = new ofxBiquadFilter3f(OFX_BIQUAD_TYPE_LOWPASS, 0.01, 0.7, 0.0);
    effectOffsetFilter      = new ofxBiquadFilter3f(OFX_BIQUAD_TYPE_LOWPASS, 0.01, 0.7, 0.0);
    camOffsetFilter         = new ofxBiquadFilter3f(OFX_BIQUAD_TYPE_LOWPASS, 0.01, 0.7, 0.0);
    
    // todo filter scale and FOV
}

void Renderer::setup() {
    
    // Do I need new here? Do I need pointers at all?
    landscapeTextureFader = new TextureFader();
    secondaryTextureFader = new TextureFader();
    effectTextureFader = new TextureFader();
    skyTextureFader= new TextureFader();
    
    landscapeFader = new ModelFader();
    effectModelFader = new ModelFader();
    
    modelOffset = ofVec3f(0,0,0);
    
    cam.setupPerspective();
    cam.setVFlip(true);
    
    cubeMap.initEmptyTextures( 512 );
    
    camRefPos = cam.getPosition();
    landscapeTextureFader->setup();
    effectTextureFader->setup();
    skyTextureFader->setup();
    secondaryTextureFader->setup();
    
    setupFilters();
    setOutput();
}

// todo pass nt width, int height, samples, preview / live mode
void Renderer::setOutput() {
    
    // todo make output settings configurable
    
        ofFbo::Settings fboSettings;
    fboSettings.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    fboSettings.useDepth = true;
    fboSettings.internalformat = GL_RGBA;
    
    
    if(name == "live") {
    

    fboSettings.height = project->outHeight;
    fboSettings.width  = project->outWidth;
    fboSettings.numSamples = 8;
    
    } else {
        
        fboSettings.height = project->outHeight;
        fboSettings.width  = project->outWidth; ///4; TODO scale values so a lower resolution render will have same composition
        fboSettings.numSamples = 1;
        
        
    }
    
    fbo.allocate(fboSettings);
}

void Renderer::renderSky() {
    
    ofPushStyle();
    ofFill();
    ofSetColor(255);
    
    for( int i = 0; i < 6; i++ )
    {
        cubeMap.beginDrawingInto2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i );
        
        ofClear(0,0,0);
        skyTextureFader->draw(cubeMap.getWidth(), cubeMap.getHeight());
        
        cubeMap.endDrawingInto2D();
    }
    ofPopStyle();
}

void Renderer::renderLandscape() {
    
    ofPushStyle();
    ofFill();
    ofSetColor(255);
    
    ofPushMatrix(); {
        ofTranslate(modelOffset);
        ofPushMatrix(); {
            ofPushMatrix(); {
                
                ofTranslate(fbo.getWidth()/2,fbo.getHeight()/2);
                
                landscapeTextureFader->begin();{
                    renderLandscapeVboMeshes(landscapeFader->getCurrent(), 1-landscapeFader->tween.update(), true);
                    renderLandscapeVboMeshes(landscapeFader->getNext(),      landscapeFader->tween.update(), true);
                    
                } landscapeTextureFader->end();
                
                secondaryTextureFader->begin();{
                    renderLandscapeVboMeshes(landscapeFader->getCurrent(), 1-landscapeFader->tween.update(), false);
                    renderLandscapeVboMeshes(landscapeFader->getNext(),      landscapeFader->tween.update(), false);
                    
                } secondaryTextureFader->end();
                
            } ofPopMatrix();
        } ofPopMatrix();
    } ofPopMatrix();
    ofPopStyle();
    

}

void Renderer::setTextureFromAsset(TextureFader * textureFader, Asset _asset) {
    //cout<<_asset.nid<<"  "<<textureFader->asset.nid<<endl;
    
        if(textureFader->hasTexture() && textureFader->asset.nid == _asset.nid && textureFader->asset.type == _asset.type) {
            // no change
        } else {
            //cout<<name<<":new texture"<<endl;
            textureFader->setWait(project->getTextureAsset(_asset));
            textureFader->asset = _asset;
        }

}

void Renderer::setModelFromAsset(ModelFader * modelFader, Asset _asset) {
    
    if(modelFader->hasModel() && modelFader->asset.nid == _asset.nid && modelFader->asset.type == _asset.type) {
        
    } else {
        //cout<<name<<": new model"<<endl;
        modelFader->setWait(project->getModelAsset(_asset));
        modelFader->asset = _asset;
    }
}

void Renderer::update() {
    
    Parameters * params = scene->params;
    
    // update textures
    setTextureFromAsset(secondaryTextureFader,  scene->secondaryTexture);
    setTextureFromAsset(landscapeTextureFader,  scene->landscapeTexture);
    setTextureFromAsset(effectTextureFader,     scene->effectTexture);
    setTextureFromAsset(skyTextureFader,        scene->skyTexture);
    
    // update models
    setModelFromAsset(landscapeFader,  scene->landscapeModel);
    setModelFromAsset(effectModelFader,  scene->effectModel);
    
    if(params->bAutoCameraRotation.get()) {
        params->camOrientation += (params->autoCamSpeed.get() * ofGetLastFrameTime() * 100);
    } else {
        params->camOrientation = params->camOrientationRef.get()*180;
    }
    
    cam.setOrientation(camOrientationFilter->updateDegree(params->camOrientation.get() + params->camOrientationRef.get()*180));
    
    if(params->bAutoEffectRotation.get()) {
        params->effectOrientation.set(params->effectOrientation += (params->autoEffectRotSpeed.get() * ofGetLastFrameTime()));
    } else {
        params->effectOrientation.set(params->effectOrientationRef.get());
    }
    
    cam.setFarClip(params->camFarClip);
    cam.setFov(params->camFov.get() );
    cam.setNearClip(params->camNearClip);
    
    params->zTravel -= params->camSpeed * ofGetLastFrameTime() * 1000;
    
    camOffset = camOffsetFilter->update(camRefPos + (params->camOffset.get()*fbo.getWidth()) ) + ofVec3f(0,0,params->zTravel);
    
    cam.setPosition(camOffset);
    
    landscapeTextureFader->update();
    effectTextureFader->update();
    skyTextureFader->update();
    secondaryTextureFader->update();
    effectModelFader->update();
    landscapeFader->update();
}

void Renderer::renderLandscapeVboMeshes(Model * m, float fade, bool prim = true) {
    if(m == NULL || !m->hasMeshes()) return;
    
    ofPushMatrix();{
        // tile forward and backward to far clip
        // float zMin = m->getSceneMin().z;
        
        // as in depth
        float zMin = abs(m->getSceneMin().z - m->getSceneMax().z);
        int tileN = ceil((cam.getFarClip()*2) / zMin) + 2;
        
        ofTranslate(ofVec3f(0,0,floor(cam.getPosition().z / zMin) * zMin));
        //cout<<m->getSceneMin()<<"   "<<m->getSceneMax()<<"   "<<m->getSceneCenter()<<endl;
        
        for(int i=0; i<tileN; i++) {
            ofPushMatrix(); {
                ofTranslate(0, 0, (zMin*i) - ((tileN/2) * zMin) );
                
                ofScale(fade,fade,fade); // TODO: have an alpha transition option also
                if(prim) {
                    
                    m->drawVboMesh(0);
                    //m->vboMeshes[0].draw();
                    
                } else {
                    
                    for(int i=1; i <m->getMeshCount(); i++) {
                        m->drawVboMesh(i);
                        //m->vboMeshes[i].draw();
                        
                    }
                }
            }ofPopMatrix();
        }
    }ofPopMatrix();
}

void Renderer::renderEffectModel(Model * m, float fade) {
    
    if(m == NULL || !m->hasMeshes()) return;
    Parameters * params = scene->params;
    
    ofPushStyle();
    ofFill();
    
    ofPushMatrix();
    
    ofTranslate( -m->getSceneCenter()   );
    
    ofVec3f filteredRot = effectOrientationFilter->updateDegree((params->effectOrientation.get()*120) + params->effectOrientationRef.get()*120);
    ofRotateX(filteredRot.x);
    ofRotateY(filteredRot.y);
    ofRotateZ(filteredRot.z);
    
    ofScale(params->effectScale.get(), params->effectScale.get(), params->effectScale.get());
    
    ofScale(fade,fade,fade);
    
    for(int i=0; i <m->getMeshCount(); i++) {
        m->drawVboMesh(i);
    }
    
    ofPopMatrix();
    ofPopStyle();
}


void Renderer::render() {
    
    //if(name == "preview" && ofGetFrameNum() % 4 != 0) return;
    //if(name == "preview" && ofGetFrameRate() < 26) return;
    
    if(scene == NULL) return;
    
    ofPushMatrix();
    ofPushStyle();
    
    /*if(name == "preview") {
        ofScale(0.25,0.25,0.25);
    }*/
    
    Parameters * params = scene->params;
    
    ofEnableAlphaBlending();
    //ofDisableArbTex();
    
    renderSky();
    
    ofSetColor(255);
    
    fbo.begin(); {
        //ofEnableNormalizedTexCoords();
        
        ofBackground(0,0,0);
        
        //ofEnableLighting();
        //ofLight light;
        ofEnableDepthTest();
        
        skyBoxCam = cam;
        skyBoxCam.setFarClip(70000);
        
        skyBoxCam.begin(); {
            ofPushMatrix(); {
                ofTranslate(cam.getPosition());
                cubeMap.bind();
                ofFill();
                cubeMap.drawSkybox(65000);
                cubeMap.unbind();
            } ofPopMatrix();
        } skyBoxCam.end();
        
        cam.begin(); {
            ofDrawSphere(0, 0, 20);

            renderLandscape();
            
            ofPushMatrix(); {
                ofPushStyle(); {
                    ofTranslate(cam.getPosition());
                    float a, x,y,z;
                    cam.getOrientationQuat().getRotate(a, x, y, z);
                    ofRotate(a, x, y, z);
                    
                    effectOffset = effectOffsetFilter->update(params->effectOffset.get()*fbo.getWidth());
                    ofTranslate(effectOffset);
                    
                    effectTextureFader->begin(); {
                        //for(int i=0; i < effectReplicator.get().x; i++) {
                        //    ofTranslate(100, 0);
                        renderEffectModel(effectModelFader->getCurrent(), 1-effectModelFader->tween.update());
                        renderEffectModel(effectModelFader->getNext(), effectModelFader->tween.update());
                        //}
                    } effectTextureFader->end();
                    
                    
                } ofPopStyle();
            } ofPopMatrix();
            
        } cam.end();
        
        ofDisableDepthTest();
        
        /*if(directSyphon > 0) { //TODO direct syphon
            landscapeTextureFader.draw(fbo.getWidth(), fbo.getHeight(), directSyphon);
        }*/
        
        
    } fbo.end();
    
    ofPopStyle();
    ofPopMatrix();
    
}