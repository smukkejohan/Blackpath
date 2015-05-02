//
//  ui.cpp
//  Blackpath
//
//  Created by Johan Bichel Lindegaard on 18/02/15.
//
//

#include "Interface.h"

#include <algorithm>    // std::min


void Interface::setup() {
    selectedScene = project->previewScene;
    
    projectTopMenu = new ofxUICanvas();
    ofAddListener(projectTopMenu->newGUIEvent, this, &Interface::guiEvent);
    
    newSceneBtn = new ofxUILabelButton("+", false);
    newSceneBtn->setName("ADD_SCENE");
    removeSceneBtn = new ofxUILabelButton("-", false);
    newSceneBtn->setName("REMOVE_SCENE");
    cloneSceneBtn = new ofxUILabelButton("C", false);
    saveSceneBtn = new ofxUILabelButton("S", false);
    saveProjectBtn = new ofxUILabelButton("SP", false);
    
    projectTopMenu->addWidgetRight(newSceneBtn);
    projectTopMenu->addWidgetRight(cloneSceneBtn);
    projectTopMenu->addWidgetRight(saveSceneBtn);
    projectTopMenu->addWidgetRight(removeSceneBtn);
    projectTopMenu->addWidgetRight(saveProjectBtn);
    
    sceneSelect = new ofxUICanvas();
    sceneSelect->setName("Scene Select");
    ofAddListener(sceneSelect->newGUIEvent, this, &Interface::guiEvent);
    
    sceneSelect->setHeight(28);
    
    projectSettings = new ofxUICanvas();
    ofAddListener(projectSettings->newGUIEvent, this, &Interface::guiEvent);
    projectSettings->addFPSSlider("FPS");
    //projectSettings
    
    textureSelect = new ofxUIScrollableCanvas();
    ofAddListener(textureSelect->newGUIEvent, this, &Interface::guiEvent);

    
    
    modelSelect = new ofxUIScrollableCanvas();
    ofAddListener(modelSelect->newGUIEvent, this, &Interface::guiEvent);
    
    /*for(int i =0; i <  project->syphonTextures.size(); i++) {
        MultiSelectorItem * thumb = new MultiSelectorItem();
        thumb->type = "syphon";
        //thumb->syphonTexture = &syphonTextures[i];
        thumb->nid = i;
        textureSelector->items.push_back(thumb);
    }*/

    sceneSettings = new ofxUICanvas();
    ofAddListener(sceneSettings->newGUIEvent, this, &Interface::guiEvent);
    
    sceneSettings->setWidth(300);
    
    /*sceneSettings->setFont("GUI/Arial.ttf");                     //This loads a new font and sets the GUI font
    sceneSettings->setFontSize(OFX_UI_FONT_LARGE, 14);            //These call are optional, but if you want to resize the LARGE, MEDIUM, and SMALL fonts, here is how to do it.
    sceneSettings->setFontSize(OFX_UI_FONT_MEDIUM, 10);
    sceneSettings->setFontSize(OFX_UI_FONT_SMALL, 7);            //SUPER IMPORTANT NOTE: CALL THESE FUNTIONS BEFORE ADDING ANY WIDGETS, THIS AFFECTS THE SPACING OF THE GUI
    */
    
    sceneSettings->addLabel("Camera");
    
    resetOrientationButton = new ofxUIButton("Reset", false, 18, 18);
    sceneSettings->addWidgetRight(resetOrientationButton);
    
    camSpeedSlider = new ofxUISlider_<float>("Speed", -1, 1, &camSpeed, 600-8, 18);
    sceneSettings->addWidgetDown(camSpeedSlider);
    
    
    camOrientationXYPad = new ofxUI2DPad("orientationxy",
                        ofVec3f(-1,1,0),ofVec3f(-1,1,0), &camOrientationXY, 280, 280);
    
    camOrientationXYPad->setLabelVisible(false);
    
    ofxUILabel * label = new ofxUILabel("Camera offset", OFX_UI_FONT_MEDIUM);
    //sceneSettings->addWidgetRight(label);
    
    camOrientationZSlider = new ofxUISlider_<float>("orientationz", -1, 1, &camOrientationZ, 20, 280);
    camOrientationZSlider->setLabelVisible(false);
    
    sceneSettings->addWidgetDown(camOrientationXYPad);
    sceneSettings->addWidgetRight(camOrientationZSlider);
    
    camOffsetPad = new ofxUI2DPad("offset",
                                  ofVec3f(-1, 1, 0),ofVec3f(-1,1, 0), &camOffset, 280, 280);
    camOffsetPad->setLabelVisible(false);
    sceneSettings->addWidgetRight(camOffsetPad);
    
    sceneSettings->addWidgetSouthOf(getLabel("Orientation X,Y,"), "orientationxy");

    resetOrientationButton->setName("reset_cam_orientation");
    sceneSettings->addWidgetSouthOf(getLabel("Z"), "orientationz");
    sceneSettings->addWidgetSouthOf(getLabel("Position"), "offset");
    
    
    camAutoOrientationToggle = new ofxUIToggle("Auto camera orientation", &bAutoCamOrientation, 18, 18);
    
    sceneSettings->addWidgetDown(camAutoOrientationToggle);
    
    
    autoCamRotXSlider = new ofxUIRotarySlider(86, -1, 1, &camRotSpeed.x, "rot x");
    //autoCamRotXSlider->setLabelVisible(false);
    autoCamRotYSlider = new ofxUIRotarySlider(86, -1, 1, &camRotSpeed.y, "rot y");
    //autoCamRotYSlider->setLabelVisible(false);
    autoCamRotZSlider = new ofxUIRotarySlider(86, -1, 1, &camRotSpeed.z, "rot z");
    //autoCamRotZSlider->setLabelVisible(false);
    
    sceneSettings->addWidgetDown(autoCamRotXSlider);
    sceneSettings->addWidgetRight(autoCamRotYSlider);
    sceneSettings->addWidgetRight(autoCamRotZSlider);

    
    fovSlider = new ofxUISlider_<float>("FOV", CAM_MIN_FOV, CAM_MAX_FOV, &camFov, 280, 18);
    sceneSettings->addWidgetSouthOf(fovSlider, "Position");
    
    
    clippingPlaneSlider = new ofxUIRangeSlider("Clipping plane", CAM_MIN_NEAR_CLIP, CAM_MAX_FAR_CLIP, CAM_MIN_NEAR_CLIP, CAM_MAX_FAR_CLIP, 280, 18);

    sceneSettings->addWidgetSouthOf(clippingPlaneSlider, "FOV");
    
    /// Effect stuff
    effectSettings = new ofxUICanvas();
    ofAddListener(effectSettings->newGUIEvent, this, &Interface::guiEvent);
    effectSettings->setWidth(300);
    
    effectSettings->addLabel("Effect");
    
    effectXYPad = new ofxUI2DPad("position",
                                 ofVec3f(-1, 1, 0),ofVec3f(-1,1, 0), &effectPositionXY, 280, 280);
    effectXYPad->setLabelVisible(false);
    effectSettings->addWidgetDown(effectXYPad);
    
    
    effectDistanceSlider = new ofxUISlider_<float>("posz", 0, 1, &effectPositionZ, 20, 280);
    effectDistanceSlider->setLabelVisible(false);
    
    effectSettings->addWidgetRight(effectDistanceSlider);
    
    effectSettings->addWidgetSouthOf(getLabel("Z"), "posz");
    effectSettings->addWidgetSouthOf(getLabel("Position X,Y,"), "position");
    
    
    effectScaleSlider = new ofxUISlider_<float>("Scale", 0, 20, &effectScale, 280, 18);
    effectSettings->addWidgetDown(effectScaleSlider);
    
    
    effectRotXSlider = new ofxUIRotarySlider(86, -1, 1, &effectRotation.x, "rot x");
    effectRotYSlider = new ofxUIRotarySlider(86, -1, 1, &effectRotation.y, "rot y");
    effectRotZSlider = new ofxUIRotarySlider(86, -1, 1, &effectRotation.z, "rot z");
    
    effectSettings->addWidgetDown(effectRotXSlider);
    effectSettings->addWidgetRight(effectRotYSlider);
    effectSettings->addWidgetRight(effectRotZSlider);
    
    effectAutoRotationToggle = new ofxUIToggle("Auto effect rotation", &bAutoEffectRotation, 18, 18);
    
    effectSettings->addWidgetDown(effectAutoRotationToggle);
    
    autoEffectRotXSlider = new ofxUIRotarySlider(86, -1, 1, &effectRotSpeed.x, "rot x");
    autoEffectRotYSlider = new ofxUIRotarySlider(86, -1, 1, &effectRotSpeed.y, "rot y");
    autoEffectRotZSlider = new ofxUIRotarySlider(86, -1, 1, &effectRotSpeed.z, "rot z");
    
    effectSettings->addWidgetDown(autoEffectRotXSlider);
    effectSettings->addWidgetRight(autoEffectRotYSlider);
    effectSettings->addWidgetRight(autoEffectRotZSlider);
    
    
    /*ofxUITextInput * widthInput = new ofxUITextInput("Output width", ofToString(_width), 60);
    widthInput->setOnlyNumericInput(true);
    widthInput->setAutoClear(false);
    
    ofxUITextInput * heightInput = new ofxUITextInput("Output height", ofToString(_height), 60);
    heightInput->setOnlyNumericInput(true);
    heightInput->setAutoClear(false);
    
    gui->addLabel("Width:");
    gui->addWidgetDown(widthInput);
    gui->addLabel("Height:");
    gui->addWidgetDown(heightInput);*/
    
    //gui->addButton("Update output resolution", &bUpdateOutput);
    
    /*for(int i=0; i<selectedScene->params->allParameters.size(); i++ ) {
     cout<<selectedScene->params->allParameters[i].type()<<endl;
     cout<<selectedScene->params->allParameters[i].getName()<<endl;
     }*/
    
    
    cueLiveButton = new ofxUIButton("Cue Live", false, 40, 40);
    
    projectSettings->addWidgetDown(cueLiveButton);
    
    previewToggle = new ofxUIToggle("Enable preview", &project->enablePreview, 20, 20);
    
    projectSettings->addWidgetDown(previewToggle);
    
    resetTextureSelector();
    resetModelSelector();
    
    layoutUIInWindow(ofGetWidth(), ofGetHeight());
    
    
    
    selectScene(project->previewScene->name);
    
}

void Interface::guiEvent(ofxUIEventArgs &e) {
    
    if(e.getCanvasParent() == modelSelect) {
        
        string name = ofSplitString(e.getName(), "_")[0];
        int _id = ofToInt(ofSplitString(e.getName(), "_")[1]);
        
        
        if(name == "landscape") {
            if(e.getToggle()->getValue()) {
                selectedScene->landscapeModel = Asset("model",  _id);
            } else {
                selectedScene->landscapeModel = Asset();
            }
        } else if(name == "effect") {
            if(e.getToggle()->getValue()) {
                selectedScene->effectModel = Asset("model",  _id);
            } else {
                selectedScene->effectModel = Asset();
            }
        }
        
        updateModelSelector();
        
    }
    
    
    if(e.getCanvasParent() == textureSelect) {
        
        string name = ofSplitString(e.getName(), "_")[0];
        int _id = ofToInt(ofSplitString(e.getName(), "_")[1]);
        
        
        if(name == "primary") {
            if(e.getToggle()->getValue()) {
                selectedScene->landscapeTexture = Asset("texture",  _id);
            } else {
                selectedScene->landscapeTexture = Asset();
            }
        }else if(name == "secondary") {
            if(e.getToggle()->getValue()) {
                selectedScene->secondaryTexture = Asset("texture",  _id);
            } else {
                selectedScene->secondaryTexture = Asset();
            }
        } else if(name == "effect") {
            if(e.getToggle()->getValue()) {
                selectedScene->effectTexture = Asset("texture",  _id);
            } else {
                selectedScene->effectTexture = Asset();
            }
        }else if(name == "sky") {
            if(e.getToggle()->getValue()) {
                selectedScene->skyTexture = Asset("texture",  _id);
            } else {
                selectedScene->skyTexture = Asset();
            }
        }
        
        resetTextureSelector();
    }
    
    
    if(e.getCanvasParent() == sceneSelect)
    {
        ofxUILabelButton * pressedLabel = (ofxUILabelButton *) e.widget;
        
        vector<ofxUILabelButton *>::iterator it  = sceneTabs.begin();
        vector<ofxUILabelButton *>::iterator eit = sceneTabs.end();
        
        for(; it != eit; ++it) {
            ofxUILabelButton *w = (*it);
            w->setValue(false);
        }
        
        pressedLabel->setValue(true);
        selectScene(pressedLabel->getName());
    }
    
    if(e.getCanvasParent() == projectTopMenu)
    {
        if(e.widget == newSceneBtn) {
            
            if(newSceneBtn->getValue()) {
                project->addScene();
            }
            
        } else if(e.widget == removeSceneBtn) {
            
            if(removeSceneBtn->getValue()){
                if(project->scenes.size() > 1){
                    project->removeScene(selectedScene);
                    selectScene(project->scenes.front()->name);
                }
            }
            
        }
    }
    
    if(e.getCanvasParent() == sceneSettings && selectedScene != NULL && selectedScene->params != NULL)
    {
        
        Parameters * p = selectedScene->params;
        
        if(e.widget == camOrientationXYPad || e.widget == camOrientationZSlider) {
            p->camOrientationRef.set(ofVec3f(camOrientationXY.y, -camOrientationXY.x, camOrientationZ));
            
        }else if(e.widget == fovSlider) {
            p->camFov.set(camFov);
            
        } else if(e.widget == camSpeedSlider) {
            p->camSpeed.set(camSpeed);
            
        } else if(e.widget == camAutoOrientationToggle ||
                  e.widget == autoCamRotXSlider ||
                  e.widget == autoCamRotYSlider ||
                  e.widget == autoCamRotZSlider) {
            
            p->bAutoCameraRotation.set(bAutoCamOrientation);
            p->autoCamSpeed.set(camRotSpeed);
            
        } else if(e.widget == camOffsetPad) {
            p->camOffset.set(ofVec3f(camOffset.x, camOffset.y, 0));
        } else if(e.widget == clippingPlaneSlider) {
            p->camNearClip.set(clippingPlaneSlider->getValueLow());
            p->camFarClip.set(clippingPlaneSlider->getValueHigh());
        }
        
        
    }
    
    if(e.getCanvasParent() == effectSettings && selectedScene != NULL && selectedScene->params != NULL)
    {
        Parameters * p = selectedScene->params;
        
        if(e.widget == effectXYPad || e.widget == effectDistanceSlider ) {
            p->effectOffset.set(ofVec3f(effectPositionXY.y, -effectPositionXY.x, effectPositionZ));
            
        } else if(e.widget == effectScaleSlider) {
            p->effectScale.set(effectScale);
            
        } else if(e.widget == effectAutoRotationToggle) {
            p->bAutoEffectRotation.set(bAutoEffectRotation);
            p->autoEffectRotSpeed.set(effectRotSpeed);
            
        } else if(e.widget == effectRotXSlider ||
                  e.widget == effectRotYSlider ||
                  e.widget == effectRotZSlider) {
            p->effectOrientationRef.set(effectRotation);
            
        } else if(e.widget == autoEffectRotXSlider ||
                  e.widget == autoEffectRotYSlider ||
                  e.widget == autoEffectRotZSlider) {
            p->autoEffectRotSpeed.set(effectRotSpeed);
        }
        
    }
    
    
    if(e.getCanvasParent() == projectSettings && selectedScene != NULL && selectedScene->params != NULL)
    {
        Parameters * p = selectedScene->params;
        
        if(e.widget == cueLiveButton ) {
            if(e.getButton()->getValue()) {
                project->activeScene = selectedScene;
            }
        }
        
    }
    
    
    /*
     string name = e.widget->getName();
     int kind = e.widget->getKind();
     //cout << name << endl;
     
     //camOrientationParam.set( ofVec3f(-uiCamOrientationXY.y, uiCamOrientationXY.x, uiCamOrientationZ) );
     
     //effectOrientationRef.set( ofVec3f(-uiEffectRotationXY.y, uiEffectRotationXY.x, uiEffectRotationZ) );
     
     
     if(name == "Reset Cam Rotation") {
     uiCamOrientationXY = ofVec3f(0,0,0);
     uiCamOrientationZ = 0;
     
     camOffsetParam.set(ofVec2f(0,0));
     camOrientationParam.set( ofVec3f(-uiCamOrientationXY.y, uiCamOrientationXY.x, uiCamOrientationZ) );
     resetCamRot = false;
     }
     
     if(name == "Output width") {
     ofxUITextInput *ti = (ofxUITextInput *) e.widget;
     if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER)
     {
     cout << "ON ENTER: ";
     string output = ti->getTextString();
     cout << output << endl;
     
     _width = ofToInt(output);
     setupOutput();
     }
     }
     
     if(name == "Output height") {
     
     ofxUITextInput *ti = (ofxUITextInput *) e.widget;
     
     if(ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER)
     {
     cout << "ON ENTER: ";
     string output = ti->getTextString();
     cout << output << endl;
     
     _height = ofToInt(output);
     setupOutput();
     }
     }
     
     if(name == "Update output resolution") {
     bUpdateOutput = false;
     }*/
    
}



void Interface::layoutUIInWindow(int w, int h) {
    
    colWidth = 300;
    topMenuHeight = 28;
    
    colHeight = h-topMenuHeight;
    halfColHeight = colHeight/2;
    
    int x = 0;
    int y = 0;
    
    projectTopMenu->autoSizeToFitWidgets();
    projectTopMenu->setPosition(w - projectTopMenu->getRect()->getWidth(), 0);
    
    sceneSelect->setWidth(w - projectTopMenu->getRect()->getWidth());
    
    textureSelect->setPosition(0,topMenuHeight);
    
    textureSelect->autoSizeToFitWidgets();
    textureSelect->setWidth(colWidth);
    if(textureSelect->getRect()->getHeight() < halfColHeight) textureSelect->setHeight(halfColHeight);
    
    modelSelect->autoSizeToFitWidgets();
    modelSelect->setWidth(colWidth);
    modelSelect->setScrollAreaHeight(halfColHeight);
    
    modelSelect->setPosition(0, topMenuHeight+halfColHeight);
    
    if(modelSelect->getRect()->getHeight() < halfColHeight) modelSelect->setHeight(halfColHeight);
    
    
    projectSettings->setPosition(colWidth, 400);
    
    //sceneSettings->setWidth(colWidth);
    sceneSettings->autoSizeToFitWidgets();
    sceneSettings->setPosition(colWidth*2, topMenuHeight);
    sceneSettings->setHeight(colHeight);
    
    x = sceneSettings->getRect()->getMaxX();
    effectSettings->autoSizeToFitWidgets();
    effectSettings->setPosition(x+8, topMenuHeight);
    //effectSettings->setHeight(halfColHeight);
    
    
}


/*void Interface::XYZSlidersFromParam(ofParameter<ofVec3f> & param) {
    
}*/

void Interface::selectScene(string _name) {
    
    selectedScene = project->getScene(_name);
    if(!selectedScene) return;
    
    project->previewScene = selectedScene;
    
    updateTextureSelector();
    updateModelSelector();
    /*for(int i=0; i<selectedScene->params->allParameters.size(); i++ ) {
        selectedScene->params->allParameters[i].type();
        selectedScene->params->allParameters[i].getName();
    }*/
    
    // TODO: go through and making naming more similar and verbose across classes
    
    Parameters * p = selectedScene->params;
    
    camOrientationXY    = p->camOrientationRef.get();
    camOrientationZ     = p->camOrientationRef.get().z;
    camFov              = p->camFov.get();
    camSpeed            = p->camSpeed.get();
    camOffset           = p->camOffset.get();
    effectScale         = p->effectScale.get();
    camRotSpeed         = p->autoCamSpeed.get();
    bAutoCamOrientation = p->bAutoCameraRotation.get();
    clippingPlaneSlider->setMaxAndMin(p->camFarClip.get(), p->camNearClip.get());
    effectPositionXY    = p->effectOffset.get();
    effectPositionZ     = p->effectOffset.get().z;
    bAutoEffectRotation = p->bAutoEffectRotation.get();
    effectRotation      = p->effectOrientationRef.get();
    effectRotSpeed      = p->autoEffectRotSpeed.get();
}


void Interface::resetModelSelector() {
    
    modelSelect->clearWidgets();
    
    modelSelect->addLabel("Models");
    
    numModels = 0;
    for(int i =0; i<project->models.size(); i++) {
        //  project->models[i].
        
        if(project->models[i].armed) {
            numModels++;
            
        ofxUILabel * label = new ofxUILabel(120, "label_"+ofToString(i), project->models[i].file.getBaseName(), OFX_UI_FONT_SMALL);
        
        modelSelect->addWidgetDown(label);
        modelSelect->addWidgetRight(getToggleBtn("landscape", i));
        modelSelect->addWidgetRight(getToggleBtn("effect", i));
        
        ofxUIButton * clearBtn = new ofxUIButton("clear_"+ofToString(i), false, 20, 20);
        clearBtn->setLabelVisible(false);
        
        modelSelect->addWidgetRight(clearBtn);
        }
    }
    
    modelSelect->autoSizeToFitWidgets();
    modelSelect->setWidth(colWidth);
    if(modelSelect->getRect()->getHeight() < halfColHeight) modelSelect->setHeight(halfColHeight);
    
    updateModelSelector();
    
    
}

void Interface::updateModelSelector() {
    
    for(int i=0; i<modelSelect->getWidgets().size(); i++) {
        ofxUIWidget * w = modelSelect->getWidgets()[i];
        
        if(w->getKind() == OFX_UI_WIDGET_TOGGLE) {
            ofxUIToggle * t = (ofxUIToggle *) w;
            
            string _name = ofSplitString(t->getName(), "_")[0];
            int _id = ofToInt(ofSplitString(t->getName(), "_")[1]);
            
            if(_name == "landscape" ) {
                if(_id == selectedScene->landscapeModel.nid){
                    t->setValue(true);
                } else {
                    t->setValue(false);
                }
            } else if(_name == "effect" ) {
                if(_id == selectedScene->effectModel.nid){
                    t->setValue(true);
                } else {
                    t->setValue(false);
                }
            }
        }
    }
}

void Interface::resetTextureSelector() {

    textureSelect->clearWidgets();
    
    textureSelect->addLabel("Textures");
    
    numTextures = 0;
    for(int i =0; i < project->textures.size(); i++) {
        if(project->textures[i].armed) {
            
            numTextures++;
            ofxUIImage * img = new ofxUIImage(40, 20, project->textures[i].getThumb(), "img"+ofToString(i));
            
            img->setLabelVisible(false);
            img->setCropImageToFitRect(true);
            
            textureSelect->addWidgetDown(img);
            
            textureSelect->addWidgetRight(getToggleBtn("primary", i));
            textureSelect->addWidgetRight(getToggleBtn("secondary", i));
            textureSelect->addWidgetRight(getToggleBtn("effect", i));
            textureSelect->addWidgetRight(getToggleBtn("sky", i));
            
            ofxUIButton * clearBtn = new ofxUIButton("clear_"+ofToString(i), false, 20, 20);
            clearBtn->setLabelVisible(false);
            
            textureSelect->addWidgetRight(clearBtn);
        }
    }
    
    textureSelect->autoSizeToFitWidgets();
    textureSelect->setWidth(300);
    if(textureSelect->getRect()->getHeight() < halfColHeight) textureSelect->setHeight(halfColHeight);
    
    updateTextureSelector();
    
}

void Interface::updateTextureSelector() {
    
    for(int i=0; i<textureSelect->getWidgets().size(); i++) {
        ofxUIWidget * w = textureSelect->getWidgets()[i];
        
        if(w->getKind() == OFX_UI_WIDGET_TOGGLE) {
            ofxUIToggle * t = (ofxUIToggle *) w;
            
            string _name = ofSplitString(t->getName(), "_")[0];
            int _id = ofToInt(ofSplitString(t->getName(), "_")[1]);
            
            if(_name == "primary" ) {
                if(_id == selectedScene->landscapeTexture.nid){
                    t->setValue(true);
                } else {
                    t->setValue(false);
                }
            } else if(_name == "secondary" ) {
                if(_id == selectedScene->secondaryTexture.nid){
                    t->setValue(true);
                } else {
                    t->setValue(false);
                }
            } else if(_name == "effect" ) {
                if(_id == selectedScene->effectTexture.nid){
                    t->setValue(true);
                } else {
                    t->setValue(false);
                }
            } else if(_name == "sky" ) {
                if(_id == selectedScene->skyTexture.nid){
                    t->setValue(true);
                } else {
                    t->setValue(false);
                }
            }
        }
    }
}

void Interface::windowResized(int w, int h) {
    layoutUIInWindow(w, h);
}

void Interface::update() {
    
    // keep interface in sync with project
    // optimally this should potentially support another UI controlling the same project in parrallel
 
    if(project->getScenes().size() != sceneTabs.size()) {
        sceneTabs.clear();
        sceneSelect->clearWidgets();
        
        for(int i=0; i<project->getScenes().size(); i++ ) {
            addSceneTab(project->getScenes()[i]->name);
        }
    }
    
    if(project->bTextureAssetsChanged) {
        resetTextureSelector();
        project->bTextureAssetsChanged = false;
    }
    
    if(project->bModelAssetsChanged) {
        resetModelSelector();
        project->bModelAssetsChanged = false;
    }
    
}

void Interface::addSceneTab(string _name) {
    sceneTabs.push_back(new ofxUILabelButton(_name, false));
    sceneSelect->addWidgetRight(sceneTabs.back());
}

void Interface::exit() {
    delete sceneSelect;
    delete projectSettings;
    delete sceneSettings;
    delete effectSettings;
    delete projectTopMenu;
    delete textureSelect;
    delete modelSelect;
}


void Interface::draw() {
    //textureSelector->draw(0,0);
    //modelSelector->draw(0, textureSelector->getHeight());
}

void Interface::dragEvent(ofDragInfo dragInfo) {
    
    // if(dragInfo.position in bounds of window )
    for(int i=0; i < dragInfo.files.size(); i++) {
        ofFile file(dragInfo.files[i]);
        
        if(file.isDirectory()) {
            
            ofLogWarning()<<"Loading of directories not supported. "<<file.getFileName()<<" is a directory. Skipping."<<endl;
            
        } else if(file.isFile()) {
            ofLogNotice()<<"Drag and drop file: "<<file.getFileName()<<endl;
            
            if(ofContains(ACCEPTED_IMAGE_EXTENSIONS, ofToLower(file.getExtension()))) {
                // if coordinates are on texture selector
                project->textureQueue.push_back(file);
                // else if coordinates are on skybox selector
            }
            
            // if coordinates are on model selector
            if(ofContains(ACCEPTED_MODEL_EXTENSIONS, ofToLower(file.getExtension()))) {
                // if coordinates are on texture selector
                project->modelQueue.push_back(file);
                // else if coordinates are on skybox selector
                // use is hit method on ofxUIButton
            }
        }
    }
}
