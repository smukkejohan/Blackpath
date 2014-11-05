#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
#ifdef OF_RELEASE
    ofSetDataPathRoot("../Resources/Data/");
    ofSetWorkingDirectoryToDefault();
    
    ofSetLogLevel(OF_LOG_FATAL_ERROR);
    cout << "data path root: " <<  ofToDataPath("", true)  << endl;
#endif
    
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
    settings.load(ofToDataPath("settings.xml"));
    
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
    camParams.add(camSpeed.set("Speed", 0, -1, 1));
    camParams.add(camFov.set("Fov", 60, 0, 300));
    camParams.add(camFarClip.set("FarClip", 4000.0, 1, 20000));
    camParams.add(camNearClip.set("NearClip", 8.0, 0.0, 200));
    camParams.add(camOffset.set("Offset", ofVec3f(0,100,0), ofVec3f(-2000,-2000,-2000), ofVec3f(200,200,200)));
    
    camParams.add(camOrientationRef.set("Orientation Reference", ofVec3f(0,0,0), ofVec3f(-360,-360,-360), ofVec3f(360,360,360)));


    camParams.add(autoCameraRotation.set("Auto Camera Orientation", false));
    camParams.add(autoCamSpeed.set("Auto orientation speed", ofVec3f(0,0,0), ofVec3f(-1,-1,-1), ofVec3f(1,1,1)));
    
    effectParams.setName("Effect Model");
    effectParams.add(effectOffset.set("Offset", ofVec3f(0,0,-100), ofVec3f(-5000,-5000,-5000), ofVec3f(5000,5000,5000)));
    effectParams.add(effectScale.set("Scale", 1, 0, 1));
    effectParams.add(effectOrientation.set("Orientation ref", ofVec3f(0,0,0), ofVec3f(-360,-360,-360), ofVec3f(360,360,360)));
    
    otherParams.setName("Other");
    otherParams.add(directSyphon.set("Syphon direct out", 0, 0, 1));

    zTravel = 0;
    
    guiPanel.setup();
    guiPanel.add(camParams);
    guiPanel.add(effectParams);
    guiPanel.add(otherParams);
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
    
    landscapeTextureFader.setup();
    effectTextureFader.setup();
    skyTextureFader.setup();
    secondaryTextureFader.setup();
    
    camOrientationFilter = ofxBiquadFilter3f(OFX_BIQUAD_TYPE_LOWPASS, 0.01, 0.7, 0.0);
    
    effectOrientationFilter = ofxBiquadFilter3f(OFX_BIQUAD_TYPE_LOWPASS, 0.01, 0.7, 0.0);
}


//--------------------------------------------------------------
void ofApp::update(){

    ofShowCursor();
    
    if(autoCameraRotation) {
        camOrientation += (autoCamSpeed.get() * ofGetLastFrameTime() * 1000);
    } else {
        camOrientation = camOrientationRef.get();
    }
    
    cam.setOrientation(camOrientationFilter.updateDegree(camOrientation + camOrientationRef.get()));
    
    
    if(clearLandscape) {
        landscapeFader.clear();
    }
    // todo offset next model to cam pos
    
    // handle transitions
    
    cam.setFarClip(camFarClip);
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
    effectModelFader.update();
    landscapeFader.update();
    
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
                landscapeFader.setWait(&models[args.thumbNum]);
            }
            
            if(ofGetKeyPressed('2')) {
                effectModelFader.setWait(&models[args.thumbNum]);
            }
            
        }
    }
}



void ofApp::drawLandscapeVboMeshes(Model * m, float fade, bool prim = true) {
    if(m == NULL || !m->hasMeshes()) return;
    
        ofPushMatrix();{
            // tile forward and backward to far clip
            float zMin = m->getSceneMin().z;
            int tileN = ceil((cam.getFarClip()*2) / abs(zMin)) + 2;
            
            ofTranslate(ofVec3f(0,0,floor(cam.getPosition().z / zMin) * zMin));
            //cout<<floor(cam.getPosition().z / zMin) * zMin<<endl;
            
            for(int i=0; i<tileN; i++) {
                ofPushMatrix(); {
                    ofTranslate(0, 0, (zMin*i) - ((tileN/2) * zMin) );
                    
                    ofScale(fade,fade,fade);
                    if(prim) {
                        m->vboMeshes[0].draw();
                    } else {
                        for(int i=1; i <m->vboMeshes.size(); i++) {
                            
                            m->vboMeshes[i].draw();
                            
                        }
                    }
                }ofPopMatrix();
            }
        }ofPopMatrix();
}


void ofApp::drawLandscape() {
    
    ofPushMatrix(); {
        ofTranslate(modelOffset);
        
        ofVec3f off = ofVec3f(0,0,0);
        ofPushMatrix(); {
            ofTranslate(off);
            ofPushMatrix(); {
                
                ofTranslate(masterOutFbo.getWidth()/2,masterOutFbo.getHeight()/2);
                
                landscapeTextureFader.begin();{
                    
                    drawLandscapeVboMeshes(landscapeFader.getCurrent(), 1-landscapeFader.tween.update(), true);
                    drawLandscapeVboMeshes(landscapeFader.getNext(), landscapeFader.tween.update(), true);
                    
                } landscapeTextureFader.end();
                
                secondaryTextureFader.begin();{
                    
                    drawLandscapeVboMeshes(landscapeFader.getCurrent(), 1-landscapeFader.tween.update(), false);
                    drawLandscapeVboMeshes(landscapeFader.getNext(), landscapeFader.tween.update(), false);
                    
                } secondaryTextureFader.end();
                
                
            }ofPopMatrix();
            
            
        } ofPopMatrix();
        
    } ofPopMatrix();
    
    
    
}

void ofApp::drawEffectModel(Model * m, float fade) {
    if(m == NULL || !m->hasMeshes()) return;
    
    ofPushMatrix();
    
        ofTranslate(-m->getSceneCenter());
    
        ofScale(effectScale, effectScale, effectScale);
    
    ofDrawGrid();
    
        ofScale(fade,fade,fade);

        m->vboMeshes[0].draw();


    
    ofPopMatrix();
    
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
            
            drawLandscape();
            
            
            ofPushMatrix(); {
                ofPushStyle(); {
                    ofTranslate(cam.getPosition());
                    float a, x,y,z;
                    cam.getOrientationQuat().getRotate(a, x, y, z);
                    ofRotate(a, x, y, z);
                    
                    ofTranslate(effectOffset.get());
                    
                    ofRotateX(effectOrientation.get().x);
                    ofRotateY(effectOrientation.get().y);
                    ofRotateZ(effectOrientation.get().z);
                    
                    //ofSetColor(255,0,0);
                    //ofDrawSphere(0, 0, 20);
                    
                    effectTextureFader.begin(); {
                        //for(int i=0; i < effectReplicator.get().x; i++) {
                        //    ofTranslate(100, 0);
                        drawEffectModel(effectModelFader.getCurrent(), 1-effectModelFader.tween.update());
                        drawEffectModel(effectModelFader.getNext(), effectModelFader.tween.update());
                        //}
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
    string guide = "Drag and drop image files or 3d models into the window to add them to the file picker.\n\nTo select textures click the texture while holding down keys:\n [1]: to Add as primary texture.\n [2]: to add as secondary texture. \n [3]: To add as effect texture. \n [4]: to add as skybox texture. \n\nTo select models click the model while holding down keys: \n [1]: To add as landscape. \n [2]: to add as effect model.";
    ofDrawBitmapString(guide, ofGetWidth()-700, ofGetHeight()-200);
    
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
    
    settings.save(ofToDataPath("settings.xml"));
    
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
