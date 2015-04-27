//
//  ui.cpp
//  Blackpath
//
//  Created by Johan Bichel Lindegaard on 18/02/15.
//
//

#include "Interface.h"


void Interface::setup() {
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

    projectSettings->setVisible(true);
    
    textureSelector = new MultiSelector(400, 500, 50, 50, "Texture");
    ofAddListener(textureSelector->selectorEvent, this, &Interface::thumbEventListener);
    
    for(int i =0; i < project->textures.size(); i++) {
        MultiSelectorItem * thumb = new MultiSelectorItem();
        thumb->img =  project->textures[i].getThumb();
        thumb->nid = i;
        textureSelector->items.push_back(thumb);
    }
    
    for(int i =0; i <  project->syphonTextures.size(); i++) {
        MultiSelectorItem * thumb = new MultiSelectorItem();
        thumb->type = "syphon";
        //thumb->syphonTexture = &syphonTextures[i];
        thumb->nid = i;
        textureSelector->items.push_back(thumb);
    }
    
    modelSelector = new MultiSelector(400, 500, 50, 50, "Model");
    ofAddListener(modelSelector->selectorEvent, this, &Interface::thumbEventListener);
    
    for(int i =0; i<  project->models.size(); i++) {
        MultiSelectorItem * thumb = new MultiSelectorItem();
        thumb->img =  project->models[i].getThumb();
        thumb->nid = i;
        modelSelector->items.push_back(thumb);
    }
    
    
    
    
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
    
    //sceneSettings->addSlider("Cam ", CAM_MIN_FOV, CAM_MAX_FOV, &camFov);
    
    sceneSettings->addRangeSlider("Clip", CAM_MIN_NEAR_CLIP, CAM_MAX_FAR_CLIP, CAM_MIN_NEAR_CLIP, CAM_MAX_FAR_CLIP);
    
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
    
}


void Interface::selectScene(string _name) {
    
    selectedScene = project->getScene(_name);
    project->previewScene = selectedScene;
    
    
    camOrientationXY = selectedScene->params->camOrientation.get();
    camOrientationZ = selectedScene->params->camOrientation.get().z;
    camFov = selectedScene->params->camFov.get();
    
}

void Interface::update() {
    
    // keep interface in sync with project
    // optimally this should potentially support another UI controlling the same project in parrallel
    
    textureSelector->update();
    modelSelector->update();
    
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
    delete projectTopMenu;
}

void Interface::guiEvent(ofxUIEventArgs &e) {
    
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
            p->camOrientation.set(ofVec3f(camOrientationXY.x, camOrientationXY.y, camOrientationZ));
        }
        
        if(e.getName() == "Cam FOV") {
            p->camFov.set(camFov);
        }
        
        /*if(e.getName() == "Clip") {
            p->camNearClip.set(e.range)
        }*/
        
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

void Interface::thumbEventListener(MultiSelectorEventData& args) {
    cout<<"Received event "<<args.event<<args.title<<args.thumbNum<<endl;
    
    if(args.event == "inside" && selectedScene) {
        if(args.title == "Texture") {
            
            if(ofGetKeyPressed('1')) {
                    selectedScene->landscapeTexture = Asset(args.item->type, args.item->nid);
            }
            if(ofGetKeyPressed('2')) {
                    selectedScene->secondaryTexture = Asset(args.item->type, args.item->nid);
            }
            if(ofGetKeyPressed('3')) {
                    selectedScene->effectTexture = Asset(args.item->type, args.item->nid);
            }
            if(ofGetKeyPressed('4')) {
                    selectedScene->skyTexture = Asset(args.item->type, args.item->nid);
            }
            
        } else if(args.title == "Model") {
            if(ofGetKeyPressed('1')) {
                //resetCamRot = true;
                selectedScene->landscapeModel = Asset(args.item->type, args.item->nid);
            }
            
            if(ofGetKeyPressed('2')) {
                //resetEffectRot = true;
                selectedScene->effectModel = Asset(args.item->type, args.item->nid);
            }
        }
    }
}


void Interface::draw() {
    textureSelector->draw(0,0);
    modelSelector->draw(0, textureSelector->getHeight());
}
