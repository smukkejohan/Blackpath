//
//  ui.cpp
//  Blackpath
//
//  Created by Johan Bichel Lindegaard on 18/02/15.
//
//

#include "Interface.h"

void Interface::setup() {

    selectedScene = project->previewScene;
    
    projectTopMenu = new ofxUICanvas();
    
    projectTopMenu->setWidth(400);
    
    ofAddListener(projectTopMenu->newGUIEvent, this, &Interface::guiEvent);
    
    ofxUILabelButton * newSceneBtn = new ofxUILabelButton("+", false);
    newSceneBtn->setName("ADD_SCENE");
    ofxUILabelButton * cloneSceneBtn = new ofxUILabelButton("C", false);
    ofxUILabelButton * saveSceneBtn = new ofxUILabelButton("S", false);
    ofxUILabelButton * saveProjectBtn = new ofxUILabelButton("SP", false);
    
    projectTopMenu->addWidgetRight(newSceneBtn);
    projectTopMenu->addWidgetRight(cloneSceneBtn);
    projectTopMenu->addWidgetRight(saveSceneBtn);
    projectTopMenu->addWidgetRight(saveProjectBtn);
    
    projectTopMenu->autoSizeToFitWidgets();
    projectTopMenu->setPosition(ofGetWidth() - projectTopMenu->getRect()->getWidth(), 0);
    
    sceneSelect = new ofxUICanvas();
    sceneSelect->setName("Scene Select");
    ofAddListener(sceneSelect->newGUIEvent, this, &Interface::guiEvent);
    
    projectSettings = new ofxUICanvas();
    ofAddListener(projectSettings->newGUIEvent, this, &Interface::guiEvent);
    projectSettings->addFPSSlider("FPS");
    //projectSettings

    projectSettings->setVisible(false);
    
    
    
    textureSelect = new ofxUIScrollableCanvas();
    ofAddListener(textureSelect->newGUIEvent, this, &Interface::guiEvent);

    textureSelect->addLabel("Textures");
    
    for(int i =0; i < project->textures.size(); i++) {
        
        if(project->textures[i].armed) {
            
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
    
    textureSelect->setPosition(0, projectTopMenu->getRect()->height);
    textureSelect->autoSizeToFitWidgets();
    
    textureSelect->setWidth(300);
    textureSelect->setScrollAreaHeight((ofGetHeight()-projectTopMenu->getRect()->height)/2);
    
    modelSelect = new ofxUIScrollableCanvas();
    ofAddListener(modelSelect->newGUIEvent, this, &Interface::guiEvent);
    
    modelSelect->addLabel("Models");
    
    for(int i =0; i<project->models.size(); i++) {
        //  project->models[i].
        
        if(project->models[i].armed) {
            
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
    
    modelSelect->setWidth(300);
    modelSelect->setScrollAreaHeight((ofGetHeight()-projectTopMenu->getRect()->height)/2);

    
    modelSelect->setPosition(0, projectTopMenu->getRect()->height+(ofGetHeight()-projectTopMenu->getRect()->height)/2);
    
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
    
    sceneSettings->addLabel("Camera Orientation");
    camXYPad = new ofxUI2DPad("Cam XY Pad",
                        ofVec3f(CAM_MIN_ORIENTATION.x, CAM_MAX_ORIENTATION.x, 0),ofVec3f(CAM_MIN_ORIENTATION.y,CAM_MAX_ORIENTATION.y, 0), &camOrientationXY, 280, 280);
    
    sceneSettings->addWidgetDown(camXYPad);
    
    //sceneSettings->addSlider("Cam orientation x", CAM_MIN_ORIENTATION.x, CAM_MAX_ORIENTATION.x, &camOrientationXY.x);
    
    //sceneSettings->addSlider("Cam orientation y", CAM_MIN_ORIENTATION.y, CAM_MAX_ORIENTATION.y, &camOrientationXY.y);
    
    sceneSettings->addSlider("Cam orientation z", CAM_MIN_ORIENTATION.z, CAM_MAX_ORIENTATION.z, &camOrientationZ);
    
    sceneSettings->addButton("Reset Cam Rotation", false);
    
    sceneSettings->addSlider("Cam FOV", CAM_MIN_FOV, CAM_MAX_FOV, &camFov);
    
    sceneSettings->addRangeSlider("Clip", CAM_MIN_NEAR_CLIP, CAM_MAX_FAR_CLIP, CAM_MIN_NEAR_CLIP, CAM_MAX_FAR_CLIP);
    
    sceneSettings->addSlider("Cam Speed", CAM_MIN_SPEED, CAM_MAX_SPEED, &camSpeed);
    
    sceneSettings->addLabel("Camera offset");
    camOffsetPad = new ofxUI2DPad("Cam offset XY Pad",
                              ofVec3f(CAM_MIN_OFFSET.x, CAM_MAX_OFFSET.x, 0),ofVec3f(CAM_MIN_OFFSET.y,CAM_MAX_OFFSET.y, 0), &camOffset, 280, 280);
    sceneSettings->addWidgetDown(camOffsetPad);
    
    sceneSettings->addLabel("Camera rotation speed");
    sceneSettings->addToggle("bAutoCameraRotation", &bAutoCameraRotation);
    
    sceneSettings->addSlider("Cam Rot Speed X", CAM_MIN_SPEED_AUTO_ORIENTATION.x, CAM_MAX_SPEED_AUTO_ORIENTATION.x, &camRotSpeed.x);
    sceneSettings->addSlider("Cam Rot Speed Y", CAM_MIN_SPEED_AUTO_ORIENTATION.y, CAM_MAX_SPEED_AUTO_ORIENTATION.y, &camRotSpeed.y);
    sceneSettings->addSlider("Cam Rot Speed Z", CAM_MIN_SPEED_AUTO_ORIENTATION.z, CAM_MAX_SPEED_AUTO_ORIENTATION.z, &camRotSpeed.z);
    
    
    /// Effect stuff
    
    effectSettings = new ofxUICanvas();
    ofAddListener(effectSettings->newGUIEvent, this, &Interface::guiEvent);
    effectSettings->setWidth(300);
    
    
    effectSettings->addLabel("Effect");
    
    effectSettings->addSlider("Effect Scale", 0, 20, &effectScale);
    
    
    effectSettings->addSlider("Effect Rot X", -1, 1, &effectRotation.x);
    effectSettings->addSlider("Effect Rot Y", -1, 1, &effectRotation.y);
    effectSettings->addSlider("Effect Rot Z", -1, 1, &effectRotation.z);
    
    
    effectSettings->addToggle("bAutoEffectRotation", &bAutoEffectRotation);
    
    effectSettings->addSlider("Effect Rot Speed X", -1, 1, &autoEffectRotSpeed.x);
    effectSettings->addSlider("Effect Rot Speed Y", -1, 1, &autoEffectRotSpeed.y);
    effectSettings->addSlider("Effect Rot Speed Z", -1, 1, &autoEffectRotSpeed.z);
    
    effectXYPad = new ofxUI2DPad("Effect offset XY Pad",
                                  ofVec3f(EFFECT_MIN_OFFSET.x, EFFECT_MAX_OFFSET.x, 0),ofVec3f(EFFECT_MIN_OFFSET.y,EFFECT_MAX_OFFSET.y, 0), &effectPositionXY, 280, 280);
    effectSettings->addWidgetDown(effectXYPad);
    
    effectSettings->addSlider("Effect Distance", EFFECT_MIN_OFFSET.z, EFFECT_MAX_OFFSET.z, &effectPositionZ);
    
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
    
    sceneSettings->autoSizeToFitWidgets();
    sceneSettings->setPosition(ofGetWidth()-sceneSettings->getRect()->width, ofGetHeight()-sceneSettings->getRect()->height);
    
    sceneSettings->setVisible(true);
    
    effectSettings->autoSizeToFitWidgets();
    effectSettings->setPosition(ofGetWidth()-sceneSettings->getRect()->width*2, ofGetHeight()-sceneSettings->getRect()->height);
    
    effectSettings->setVisible(true);
    
    
    /*for(int i=0; i<selectedScene->params->allParameters.size(); i++ ) {
     cout<<selectedScene->params->allParameters[i].type()<<endl;
     cout<<selectedScene->params->allParameters[i].getName()<<endl;
     }*/
    
}


/*void Interface::XYZSlidersFromParam(ofParameter<ofVec3f> & param) {
    
}*/

void Interface::selectScene(string _name) {
    
    
    selectedScene = project->getScene(_name);
    project->previewScene = selectedScene;
    
    resetTextureSelector();
    
    
    /*for(int i=0; i<selectedScene->params->allParameters.size(); i++ ) {
        selectedScene->params->allParameters[i].type();
        selectedScene->params->allParameters[i].getName();
    }*/
    
    camOrientationXY = selectedScene->params->camOrientation.get();
    camOrientationZ = selectedScene->params->camOrientation.get().z;
    camFov = selectedScene->params->camFov.get();
    camSpeed = selectedScene->params->camSpeed.get();
    
    camOffset = selectedScene->params->camOffset.get();
    effectScale =selectedScene->params->effectScale;
    
}


void Interface::resetTextureSelector() {
    
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

void Interface::update() {
    
    // keep interface in sync with project
    // optimally this should potentially support another UI controlling the same project in parrallel
    
    //textureSelector->update();
    //modelSelector->update();
    
    
    if(project->getScenes().size() != sceneTabs.size()) {
        sceneTabs.clear();
        sceneSelect->clearWidgets();
        
        for(int i=0; i<project->getScenes().size(); i++ ) {
            addSceneTab(project->getScenes()[i]->name);
        }
        
        sceneSelect->autoSizeToFitWidgets();
        sceneSelect->setWidth(ofGetWidth() - sceneSelect->getRect()->getWidth());
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
                selectedScene->effectModel = Asset("none", -1);
            }
        }

    }
    
    
    if(e.getCanvasParent() == textureSelect) {
        
        string name = ofSplitString(e.getName(), "_")[0];
        int _id = ofToInt(ofSplitString(e.getName(), "_")[1]);
        
        
        if(name == "primary") {
            if(e.getToggle()->getValue()) {
                selectedScene->landscapeTexture = Asset("texture",  _id);
            } else {
                selectedScene->landscapeTexture = Asset("none", -1);
            }
        }else if(name == "secondary") {
            if(e.getToggle()->getValue()) {
                selectedScene->secondaryTexture = Asset("texture",  _id);
            } else {
                selectedScene->secondaryTexture = Asset("none", -1);
            }
        } else if(name == "effect") {
            if(e.getToggle()->getValue()) {
                selectedScene->effectTexture = Asset("texture",  _id);
            } else {
                selectedScene->effectTexture = Asset("none", -1);
            }
        }else if(name == "sky") {
            if(e.getToggle()->getValue()) {
                selectedScene->skyTexture = Asset("texture",  _id);
            } else {
                selectedScene->skyTexture = Asset("none", -1);
            }
        }
        
        resetTextureSelector();
    }
    
    
    if(e.getCanvasParent() == sceneSelect)
    {
        ofxUILabelButton * pressedLabel = (ofxUILabelButton *) e.widget;
        
        vector<ofxUILabelButton *>::iterator it = sceneTabs.begin();
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
        if(e.getName() == "ADD_SCENE") {
            ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
            if(button->getValue()) {
                project->addScene();
            }
        }
    }
    
    if(e.getCanvasParent() == sceneSettings && selectedScene != NULL && selectedScene->params != NULL)
    {
        Parameters * p = selectedScene->params;
        
        ///cout<<e.getName()<<endl;
        if(e.getName() == "Cam XY Pad" || e.getName() == "Cam Orientation z" ) {
            p->camOrientation.set(ofVec3f(camOrientationXY.y, -camOrientationXY.x, camOrientationZ));
        }
        
        // consider looping through and matching param name
        
        if(e.getName() == "Cam FOV") {
            p->camFov.set(camFov);
        }
        
        if(e.getName() == "Cam Speed") {
            p->camSpeed.set(camSpeed);
        }
        
        if(e.getName() == "Cam offset XY Pad") {
            p->camOffset.set(camOffset * ofVec3f(1,1,0));
        }
        
        if(e.getName() == "bAutoCameraRotation") {
            p->bAutoCameraRotation.set(bAutoCameraRotation);
            p->autoCamSpeed.set(camRotSpeed);
        }

        if(e.getName() == "Cam Rot Speed X") {
            p->bAutoCameraRotation.set(bAutoCameraRotation);
            p->autoCamSpeed.set(camRotSpeed);
        }
        if(e.getName() == "Cam Rot Speed Y") {
            p->bAutoCameraRotation.set(bAutoCameraRotation);
            p->autoCamSpeed.set(camRotSpeed);
        }
        if(e.getName() == "Cam Rot Speed Z") {
            p->bAutoCameraRotation.set(bAutoCameraRotation);
            p->autoCamSpeed.set(camRotSpeed);
        }
    }
    
    if(e.getCanvasParent() == effectSettings && selectedScene != NULL && selectedScene->params != NULL)
    {
        Parameters * p = selectedScene->params;
        
        if(e.getName() == "Effect offset XY Pad" || e.getName() == "Effect Distance" ) {
            p->effectOffset.set(ofVec3f(effectPositionXY.y, -effectPositionXY.x, effectPositionZ));
        }
    
        if(e.getName() == "Effect Scale") {
            p->effectScale.set(effectScale);
        }
        
        if(e.getName() == "bAutoEffectRotation") {
            p->bAutoEffectRotation.set(bAutoEffectRotation);
            p->autoEffectRotSpeed.set(autoEffectRotSpeed);
        }
        
        if(e.getName() == "Effect Rot Speed X" || e.getName() == "Effect Rot Speed Y" || e.getName() == "Effect Rot Speed Z") {
            p->autoEffectRotSpeed.set(autoEffectRotSpeed);
        }
        
        if(e.getName() == "Effect Rot X" || e.getName() == "Effect Rot Y" || e.getName() == "Effect Rot Z") {
            p->effectOrientationRef.set(effectRotation);
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


void Interface::draw() {
    //textureSelector->draw(0,0);
    //modelSelector->draw(0, textureSelector->getHeight());
}
