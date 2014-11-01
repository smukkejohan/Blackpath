#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    glEnable(GL_TEXTURE_2D);
    
    ofShowCursor();
    ofSetLogLevel(OF_LOG_NOTICE);
    
    syphonInFbo.allocate(1000, 1000);
    
    modelOffset = ofVec3f(0,0,0);
    
    textures.resize(MAX_TEXTURES);
    models.resize(MAX_MODELS);
    syphonTextures.resize(MAX_SYPHON_TEXTURES);
    
    ofFbo::Settings fboSettings;
    fboSettings.height = 1080;
    fboSettings.width = 1920;
    fboSettings.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    fboSettings.numSamples = 8;
    fboSettings.useDepth = true;
    fboSettings.internalformat = GL_RGBA;
    //fboSettings.depthStencilAsTexture = true;
    
    masterOutFbo.allocate(fboSettings);
    
    //light.enable();
    //light.setPointLight();
    //light.setPosition(0,0,300);
    
    ofSetWindowTitle("Black Path");
    gui = new ofxUICanvas();
    
    gui->addFPSSlider("FPS");
    gui->autoSizeToFitWidgets();
    gui->setPosition(ofGetWidth()-gui->getRect()->width, ofGetHeight()-gui->getRect()->height);
    settings.load("settings.xml");
    
    int texTags = settings.getNumTags("texture");
    for(int i=0; i<texTags; i++) {
        string path = settings.getValue("texture:path", "", i);
        ofFile f(path);
        if(f.isFile()) {
            if(textures.size() > i){
                textures[i].load(f, &threadImgLoader);
            }
        }
    }
    
    int modTags = settings.getNumTags("model");
    for(int i=0; i<modTags; i++) {
        string path = settings.getValue("model:path", "", i);
        ofFile f(path);
        if(f.isFile()) {
            models[i].load(f);
        }
    }
    
    textureSelector = new ThumbSelector(400, 500, 50, 50, "Texture");
    ofAddListener(textureSelector->thumbSelectEvent, this, &ofApp::thumbEventListener);
    
    for(int i =0; i < textures.size(); i++) {
        Thumb * thumb = new Thumb();
        thumb->img = textures[i].getThumb();
        
        thumb->nid = i;
        textureSelector->thumbs.push_back(thumb);
    }
    
    for(int i =0; i < syphonTextures.size(); i++) {
        Thumb * thumb = new Thumb;
        thumb->type = "syphon";
        thumb->img = syphonTextures[i].getThumb();
        thumb->nid = i;
        textureSelector->thumbs.push_back(thumb);
    }
    
    
    modelSelector = new ThumbSelector(400, 500, 50, 50, "Model");
    ofAddListener(modelSelector->thumbSelectEvent, this, &ofApp::thumbEventListener);
    
    for(int i =0; i< models.size(); i++) {
        Thumb * thumb = new Thumb();
        thumb->img = models[i].getThumb();
        thumb->nid = i;
        modelSelector->thumbs.push_back(thumb);
    }
    
    syphonOut.setName("Landscape");
    camParams.setName("Camera");
    camParams.add(camFarClip.set("FarClip", 18000.0f, 1, 20000));
    camParams.add(camNearClip.set("NearClip", 20.0f, 0.0, 200));
    camParams.add(camOrientation.set("Orientation", ofVec3f(0,0,0), ofVec3f(-360,-360,-360), ofVec3f(360,360,360)));
    camParams.add(camSpeed.set("Speed", 0, 0, 1));
    camParams.add(camFov.set("Fov", 60, 0, 300));
    camParams.add(camOffset.set("Start Offset", ofVec3f(0,100,0), ofVec3f(-200,-200,-200), ofVec3f(200,200,200)));
    
    camParams.add(directSyphon.set("Direct Syphon", 0, 0, 1));
    
    camParams.add(effectOffset.set("effect offset", ofVec3f(0,0,-3000), ofVec3f(-5000,-5000,-5000), ofVec3f(5000,5000,5000)));
    
    camParams.add(effectScale.set("effect scale", 1, 0, 1));
    
    zTravel = 0;
    
    guiPanel.setup(camParams);
    guiPanel.setPosition(1200, 100);
    cam.setupPerspective();
    cam.setVFlip(false);
    
    cubeMap.initEmptyTextures( 512 );
    //setup our directory
    dir.setup();
    
    //register for our directory's callbacks
    ofAddListener(dir.events.serverAnnounced, this, &ofApp::serverAnnounced);
    // not yet implemented
    //ofAddListener(dir.events.serverUpdated, this, &testApp::serverUpdated);
    ofAddListener(dir.events.serverRetired, this, &ofApp::serverRetired);
    

    testfbo.allocate(512,512);
    
    testfbo.begin(); {
        ofBackgroundGradient(ofColor::red, ofColor::blue);
        
    } testfbo.end();
    
    camRefPos = cam.getPosition();
    
}


//--------------------------------------------------------------
void ofApp::update(){
    
    
    
    
    // handle transitions
    
    cam.setFarClip(camFarClip);
    cam.setOrientation(camOrientation);
    cam.setFov(camFov);
    cam.setNearClip(camNearClip);
    zTravel -= camSpeed * ofGetLastFrameTime() * 1000;
    cam.setPosition(camRefPos + camOffset.get() + ofVec3f(0,0,zTravel));
    
    // load images in parallel thread one image at a time
    if(textureQueue.size() > 0) {
        
        ofFile file = textureQueue.back();
        ofLogNotice()<<"Adding texture: "<<" "<<file.getFileName()<<endl;
        
        for(int i=0; i<textures.size(); i++) {
            if(!textures[i].armed) {
                textures[i].load(file, &threadImgLoader);
                break;
            }
        }
        textureQueue.pop_back();
    }
    
    if(modelQueue.size() > 0) {
        ofFile file = modelQueue.back();
        ofLogNotice()<<"Adding model: "<<" "<<file.getFileName()<<endl;
        
        // todo load threaded from buffer then load to model when needed by GPU
        for(int i=0; i<models.size(); i++) {
            if(!models[i].armed) {
                models[i].load(file);
                break;
            }
        }
        modelQueue.pop_back();
    }
    
    
    for(int i =0; i< textures.size(); i++) {
        textures[i].update();
    }
    
    for(int i =0; i< models.size(); i++) {
        models[i].update();
    }
    
    textureSelector->update();
    modelSelector->update();
    
    landscapeTextureFader.update();
    effectTextureFader.update();
    skyTextureFader.update();
    secondaryTextureFader.update();
    
    // todo add listener for selector things that require independent logic
    
    // if we are approaching far cap
    // todo: transition queues
    //
}

void ofApp::thumbEventListener(ThumbSelectorEventData& args) {
    cout<<"Received event "<<args.event<<args.title<<args.thumbNum<<endl;
    
    if(args.event == "inside") {
        // Mouse released inside a thumb in a selector, this means we have to schedule it
        if(args.title == "Texture") {
            
            ofTexture * tex;
            if(args.thumb->type == "syphon") {
                tex = &syphonTextures[args.thumb->nid].client->getTexture();
            } else if( args.thumb->type == "color") {
                
            } else {
                tex = &textures[args.thumbNum].getTextureReference();
            }
            
            if(ofGetKeyPressed('1')) {
                //landTexWait = tex;
                landscapeTextureFader.setWait(tex);
            }
            
            if(ofGetKeyPressed('2')) {
                secondaryTextureFader.setWait(tex);
            }
            
            if(ofGetKeyPressed('3')) {
                effectTextureFader.setWait(tex);
            }
            
            if(ofGetKeyPressed('4')) {
                skyTextureFader.setWait(tex);
            }
            
        } else if(args.title == "Model") {
            
            if(ofGetKeyPressed('1')) {
                activeLandscapes.push_back(&models[args.thumbNum]);
            }
            
            if(ofGetKeyPressed('2')) {
                activeEffectModels.push_back(&models[args.thumbNum]);
            }
            
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    

    
    ofEnableAlphaBlending();
    //ofDisableArbTex();
    
    
    for( int i = 0; i < 6; i++ )
    {
        cubeMap.beginDrawingInto2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i );
        
        ofClear(0,0,0);
        
        skyTextureFader.draw(cubeMap.getWidth(), cubeMap.getHeight());
        //ofLine(0,0, cubeMap.getWidth(), cubeMap.getHeight() );
        //ofLine(cubeMap.getWidth(), 0, 0, cubeMap.getHeight() );
        
        cubeMap.endDrawingInto2D();
    }
    
    ofSetColor(255);
    
    masterOutFbo.begin(); {
        
        //ofEnableNormalizedTexCoords();
        
        ofBackground(0,0,0);
        
        //ofEnableLighting();
        ofEnableDepthTest();
        
        skyboxcam = cam;
        skyboxcam.setFarClip(70000);
        
        skyboxcam.begin(); {
            ofPushMatrix(); {
                ofTranslate(cam.getPosition());
                cubeMap.bind();
                cubeMap.drawSkybox(65000);
                cubeMap.unbind();
            } ofPopMatrix();
        } skyboxcam.end();
        
        cam.begin(); {
            landscapeTextureFader.begin();{
                ofPushMatrix(); {
                    ofTranslate(modelOffset);
                    
                    ofVec3f off = ofVec3f(0,0,0);
                    
                    for(int i=0; i< activeLandscapes.size(); i++) {
                        ofPushMatrix(); {
                            ofTranslate(off);
                            ofPushMatrix(); {
                                
                                ofTranslate(masterOutFbo.getWidth()/2,masterOutFbo.getHeight()/2);
                                
                                for(int ii=0; ii <activeLandscapes[i]->vboMeshes.size(); ii++) {
                                    
                                    // todo: wireframe / shaded
                                    activeLandscapes[i]->vboMeshes[ii].setMode(OF_PRIMITIVE_TRIANGLES);
                                
                                    activeLandscapes[i]->vboMeshes[ii].draw();
                                    
                                
                                }
                            }ofPopMatrix();
                            
                            off.z += activeLandscapes[i]->getSceneMin().z;
                            
                        } ofPopMatrix();
                    }
                } ofPopMatrix();
                
            } landscapeTextureFader.end();
            
            ofPushMatrix(); {
                ofPushStyle(); {
                    
                    ofTranslate(cam.getPosition());
                    float a, x,y,z;
                    cam.getOrientationQuat().getRotate(a, x, y, z);
                    
                    ofRotate(a, x, y, z);
                    
                    ofTranslate(effectOffset);
                    //ofScale(effectScale, effectScale, effectScale);
                    
                    effectTextureFader.begin(); {
                        if(activeEffectModels.size() > 0) {
                            activeEffectModels[0]->vboMeshes[0].drawFaces();
                        }
                    } effectTextureFader.end();
                    
                } ofPopStyle();
            } ofPopMatrix();
            
        } cam.end();
        
        ofDisableDepthTest();
        if(directSyphon > 0) {
            landscapeTextureFader.draw(masterOutFbo.getWidth(), masterOutFbo.getHeight(), directSyphon);
        }
        
        
    } masterOutFbo.end();
    
    ofSetColor(255, 255, 255);
    
    ofPushMatrix(); {
        ofTranslate(ofGetWidth()/2, 80);
        
        ofScale(0.4,0.4);
        masterOutFbo.draw(-(masterOutFbo.getWidth()+10)/2,0);
        ofRectRounded(-(masterOutFbo.getWidth()+10)/2, 0, masterOutFbo.getWidth()+5, masterOutFbo.getHeight()+5, 8);
    } ofPopMatrix();
    
    textureSelector->draw(0,0);
    modelSelector->draw(0, textureSelector->getHeight());
    
    guiPanel.draw();
    
    ofSetColor(255,255,255);
    
    for(int i=0; i<syphonTextures.size(); i++) {
        
        if(syphonTextures[i].armed && syphonTextures[i].client) {
            syphonTextures[i].client->draw(150+(50*i), 0, 50, 50);
        }
    }
    
    syphonOut.publishTexture(&masterOutFbo.getTextureReference());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
    // if(dragInfo.position in bounds of window )
    for(int i=0; i < dragInfo.files.size(); i++) {
        ofFile file(dragInfo.files[i]);
        
        if(file.isDirectory()) {
            
            ofLogWarning()<<"Loading of directories not supported. "<<file.getFileName()<<" is a directory. Skipping."<<endl;
            
        } else if(file.isFile()) {
            ofLogNotice()<<"Drag and drop file: "<<file.getFileName()<<endl;
            
            if(ofContains(ACCEPTED_IMAGE_EXTENSIONS, ofToLower(file.getExtension()))) {
                // if coordinates are on texture selector
                textureQueue.push_back(file);
                // else if coordinates are on skybox selector
            }
            
            // if coordinates are on model selector
            if(ofContains(ACCEPTED_MODEL_EXTENSIONS, ofToLower(file.getExtension()))) {
                // if coordinates are on texture selector
                modelQueue.push_back(file);
                // else if coordinates are on skybox selector
                // use is hit method on ofxUIButton
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    cout << name << endl;
}

void ofApp::exit() {
    threadImgLoader.stopThread();
    
    settings.clear();
    
    for(int i=0; i<textures.size(); i++) {
        settings.addTag("texture");
        settings.pushTag("texture", i);
        settings.addValue("path", textures[i].file.getAbsolutePath());
        settings.popTag();
    }
    
    int mi = 0;
    for(int i=0; i<models.size(); i++) {
        settings.addTag("model");
        settings.pushTag("model", i);
        settings.addValue("path", models[i].file.getAbsolutePath());
        settings.popTag();
    }
    
    settings.save("settings.xml");
    
    delete gui;
}

//these are our directory's callbacks
void ofApp::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
        
        for(int i=0; i<syphonTextures.size(); i++) {
            if(!syphonTextures[i].armed) {
                ofxSyphonClient * c = new ofxSyphonClient();
                c->setup();
                c->set(dir.serverName, dir.appName);
                syphonTextures[i].arm(c);
                break;
            }
        }
    }
}

void ofApp::serverUpdated(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Updated")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
}

void ofApp::serverRetired(ofxSyphonServerDirectoryEventArgs &arg)
{
    // todo remove the syphon thumbs
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Retired")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
}
