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

    projectSettings->setVisible(false);
    
    sceneSettings = new ofxUICanvas();
    ofAddListener(sceneSettings->newGUIEvent, this, &Interface::guiEvent);
    
    textureSelector = new MultiSelector(400, 500, 50, 50, "Texture");
    ofAddListener(textureSelector->selectorEvent, this, &Interface::thumbEventListener);
    
    for(int i =0; i < project->textures.size(); i++) {
        MultiSelectorItem * thumb = new MultiSelectorItem();
        thumb->img =  project->textures[i]->getThumb();
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
        thumb->img =  project->models[i]->getThumb();
        thumb->nid = i;
        modelSelector->items.push_back(thumb);
    }
    
    
    
    /*sceneSettings->addLabel("Camera Orientation");
    sceneSettings->add2DPad("Cam XY Pad",
                  ofVec3f(camOrientationParam.getMin().x,camOrientationParam.getMax().x,0),
                  ofVec3f(camOrientationParam.getMin().y,camOrientationParam.getMax().y,0),
                  &uiCamOrientationXY);
    
    gui->addSlider("Cam orientation x", camOrientationParam.getMin().x, camOrientationParam.getMax().x, &uiCamOrientationXY.x);
    gui->addSlider("Cam orientation y", camOrientationParam.getMin().y, camOrientationParam.getMax().y, &uiCamOrientationXY.y);
    gui->addSlider("Cam orientation z", camOrientationParam.getMin().z, camOrientationParam.getMax().z, &uiCamOrientationZ);
    
    gui->addButton("Reset Cam Rotation", &resetCamRot);
    
    ofxUITextInput * widthInput = new ofxUITextInput("Output width", ofToString(_width), 60);
    widthInput->setOnlyNumericInput(true);
    widthInput->setAutoClear(false);
    
    ofxUITextInput * heightInput = new ofxUITextInput("Output height", ofToString(_height), 60);
    heightInput->setOnlyNumericInput(true);
    heightInput->setAutoClear(false);
    
    gui->addLabel("Width:");
    gui->addWidgetDown(widthInput);
    gui->addLabel("Height:");
    gui->addWidgetDown(heightInput);
    
    //gui->addButton("Update output resolution", &bUpdateOutput);
    
    gui->autoSizeToFitWidgets();
    gui->setPosition(ofGetWidth()-gui->getRect()->width, ofGetHeight()-gui->getRect()->height);
     
     uiCamOrientationXY = ofVec3f(0,0,0);
     uiCamOrientationZ = 0;
     
    */
    
    sceneSettings->setVisible(false);
    
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
        project->selectScene(pressedLabel->getName());
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
    
    if(args.event == "inside") {
        // Mouse released inside a thumb in a selector, this means we have to schedule it
        if(args.title == "Texture") {
            
            ofTexture * tex;
            if(args.item->type == "syphon") {
                
                if(project->syphonTextures[args.item->nid].client && project->syphonTextures[args.item->nid].client->isSetup() && project->syphonTextures[args.item->nid].armed && project->syphonTextures[args.item->nid].client->getTexture().isAllocated()) {
                    
                    tex = &project->syphonTextures[args.item->nid].client->getTexture();
                }
            } else if( args.item->type == "color") {
                
            } else {
                tex = &project->textures[args.thumbNum]->getTexture();
            }
            
            
            if(tex && tex->isAllocated()) {
                if(ofGetKeyPressed('1')) {
                    //landscapeTextureFader.setWait(tex);
                    // todo tie into gui - and affect current UI scene be it preview or live
                }
                if(ofGetKeyPressed('2')) {
                    //secondaryTextureFader.setWait(tex);
                }
                if(ofGetKeyPressed('3')) {
                    //effectTextureFader.setWait(tex);
                }
                if(ofGetKeyPressed('4')) {
                    //skyTextureFader.setWait(tex);
                }
            }
            
        } else if(args.title == "Model") {
            
            if(ofGetKeyPressed('1')) {
                //landscapeFader.setWait(&models[args.thumbNum]);
                //resetCamRot = true;
            }
            
            if(ofGetKeyPressed('2')) {
                //effectModelFader.setWait(&models[args.thumbNum]);
                //resetEffectRot = true;
            }
            
        }
    }
    
}


void Interface::draw() {
    textureSelector->draw(0,0);
    modelSelector->draw(0, textureSelector->getHeight());
}
