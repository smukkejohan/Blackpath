//
//  ui.cpp
//  Blackpath
//
//  Created by Johan Bichel Lindegaard on 18/02/15.
//
//

#include "Interface.h"

#include <algorithm>    // std::min

//#define OFX_UI_LABEL_DRAW_BACK true
//#define OFX_UI_DRAW_PADDING true
//#define OFX_UI_DRAW_PADDING_OUTLINE true


void Interface::setup() {
    
    /*NSWindow * appWindow = (NSWindow *)ofGetCocoaWindow();
    if(appWindow) {
     
        
        NSMenu* bar = [[NSMenu alloc] init];
        [NSApp setMainMenu:bar];
        
        NSMenuItem* appMenuItem =
        [bar addItemWithTitle:@"" action:NULL keyEquivalent:@""];
        
        NSMenu* appMenu = [[NSMenu alloc] init];
        [appMenuItem setSubmenu:appMenu];
        
        [appMenu addItemWithTitle:[NSString stringWithFormat:@"About Black Path"]
                           action:@selector(orderFrontStandardAboutPanel:)
                    keyEquivalent:@""];
     
    }*/
    
    /*
    NSOpenPanel *opanel = [NSOpenPanel openPanel];
    [opanel setAllowsMultipleSelection:NO];
    int returnCode  = [opanel runModalForDirectory:nil file:nil types:nil];
    
    if(returnCode == NSOKButton) {
        NSArray *filenames = [opanel filenames];
        NSString *file = [filenames objectAtIndex:0];
        //[self loadFile:file];
    }*/
    
    
    
    Parameters * tplParams = new Parameters();
    tplParams->init();
    
    selectedScene = project->previewScene;
    
    projectTopMenu = new ofxUICanvas();
    ofAddListener(projectTopMenu->newGUIEvent, this, &Interface::guiEvent);
    projectTopMenu->setDrawBack(false);
    projectTopMenu->setDrawFill(false);
    
    newSceneBtn = new ofxUILabelButton("+", false);
    newSceneBtn->setName("ADD_SCENE");
    removeSceneBtn = new ofxUILabelButton("-", false);
    newSceneBtn->setName("REMOVE_SCENE");
    cloneSceneBtn = new ofxUILabelButton("C", false);
    //saveSceneBtn = new ofxUILabelButton("S", false);
    saveProjectBtn = new ofxUILabelButton("SP", false);
    
    projectTopMenu->addWidgetRight(newSceneBtn);
    projectTopMenu->addWidgetRight(cloneSceneBtn);
    //projectTopMenu->addWidgetRight(saveSceneBtn);
    projectTopMenu->addWidgetRight(removeSceneBtn);
    projectTopMenu->addWidgetRight(saveProjectBtn);
    
    sceneSelect = new ofxUICanvas();
    sceneSelect->setDrawBack(false);
    sceneSelect->setDrawFill(false);
    
    sceneSelect->setName("Scene Select");
    ofAddListener(sceneSelect->newGUIEvent, this, &Interface::guiEvent);
    
    sceneSelect->setHeight(28);
    
    projectSettings = new ofxUICanvas();
    projectSettings->setDrawBack(false);
    projectSettings->setDrawFill(false);
    
    ofAddListener(projectSettings->newGUIEvent, this, &Interface::guiEvent);
    projectSettings->addFPSSlider("FPS");
    //projectSettings
    
    textureSelect = new ofxUIScrollableCanvas();
    textureSelect->setDrawBack(false);
    textureSelect->setDrawFill(false);
    
    ofAddListener(textureSelect->newGUIEvent, this, &Interface::guiEvent);

    modelSelect = new ofxUIScrollableCanvas();
    modelSelect->setDrawBack(false);
    modelSelect->setDrawFill(false);
    
    ofAddListener(modelSelect->newGUIEvent, this, &Interface::guiEvent);

    sceneSettings = new ofxUICanvas();
    sceneSettings->setDrawBack(false);
    sceneSettings->setDrawFill(false);
    
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
    
    clippingPlaneSlider = new ofxUIRangeSlider("Clipping plane", CAM_MIN_NEAR_CLIP, CAM_MAX_FAR_CLIP, CAM_MIN_NEAR_CLIP, CAM_MAX_FAR_CLIP/2, 280, 18);

    sceneSettings->addWidgetSouthOf(clippingPlaneSlider, "FOV");
    
    /// Effect stuff
    effectSettings = new ofxUICanvas();
    effectSettings->setDrawBack(false);
    ofAddListener(effectSettings->newGUIEvent, this, &Interface::guiEvent);
    effectSettings->setWidth(300);
    
    effectSettings->addLabel("Effect");
    
    effectXYPad = new ofxUI2DPad("position",
                                 ofVec3f(-1, 1, 0),ofVec3f(-1,1, 0), &effectPositionXY, 280, 280);
    effectXYPad->setLabelVisible(false);
    effectSettings->addWidgetDown(effectXYPad);
    
    
    effectDistanceSlider = new ofxUISlider_<float>("posz", 0, -1, &effectPositionZ, 20, 280);
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
    
    effectReplicate.setup(tplParams->replicate);
    effectReplicate.addWidgetsDown(effectSettings);
    
    effectSpacing.setup(tplParams->replicateSpacing);
    effectSpacing.addWidgetsDown(effectSettings);
    
    
    cueLiveButton = new ofxUIButton("Cue Live", false, 40, 40);
    
    projectSettings->addWidgetDown(cueLiveButton);
    
    previewToggle = new ofxUIToggle("Enable preview", &project->enablePreview, 20, 20);
    
    projectSettings->addWidgetDown(previewToggle);
    
    
    widthInput = new ofxUITextInput("Output width", ofToString(project->outWidth), 60);
    widthInput->setOnlyNumericInput(true);
    widthInput->setAutoClear(false);
    
    heightInput = new ofxUITextInput("Output height", ofToString(project->outHeight), 60);
    heightInput->setOnlyNumericInput(true);
    heightInput->setAutoClear(false);
    

    
    projectSettings->addLabel("Width:");
    projectSettings->addWidgetDown(widthInput);
    projectSettings->addLabel("Height:");
    projectSettings->addWidgetDown(heightInput);

    
    sceneNameInput = new ofxUITextInput("Scene name", selectedScene->name, 100);
    sceneNameInput->setAutoClear(false);

    
    projectNameInput = new ofxUITextInput("Project name", project->projectPath, 600);
    projectNameInput->setAutoClear(false);

    projectSettings->addLabel("Scene name:");
    projectSettings->addWidgetDown(sceneNameInput);
    
    projectSettings->addLabel("Save Project As (type in full path) enter to save:");
    //projectNameInput->getRect()->setWidth(200);
    projectSettings->addWidgetDown(projectNameInput);
    
    
    
    resetTextureSelector();
    resetModelSelector();
    
    
    layoutUIInWindow(ofGetWidth(), ofGetHeight());
    
    selectScene(project->previewScene);
    
    
    /*
    effectSettings->setDrawFill(false);
    textureSelect->setVisible(false);
    modelSelect->setVisible(false);
    sceneSettings->setVisible(false);
    effectSettings->setVisible(false);
    */
    //effectSettings->setVisible(false);
    
    
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
        } else if(name == "clear") {
            if(e.getToggle()->getValue()) {
                
                
                if(selectedScene->effectModel.nid != _id &&
                   selectedScene->landscapeModel.nid != _id) {
                    
                    project->models[_id].armed = false;
                    resetModelSelector();

                }
                
            }
        }
        
        updateModelSelector();
    }
    
    
    if(e.getCanvasParent() == textureSelect) {
        
        string name = ofSplitString(e.getName(), "_")[0];
        string type = ofSplitString(e.getName(), "_")[1];
        int _id = ofToInt(ofSplitString(e.getName(), "_")[2]);
        
        if(name == "primary") {
            if(e.getToggle()->getValue()) {
                selectedScene->landscapeTexture = Asset(type,  _id);
            } else {
                selectedScene->landscapeTexture = Asset();
            }
        }else if(name == "secondary") {
            if(e.getToggle()->getValue()) {
                selectedScene->secondaryTexture = Asset(type,  _id);
            } else {
                selectedScene->secondaryTexture = Asset();
            }
        } else if(name == "effect") {
            if(e.getToggle()->getValue()) {
                selectedScene->effectTexture = Asset(type,  _id);
            } else {
                selectedScene->effectTexture = Asset();
            }
        }else if(name == "sky") {
            if(e.getToggle()->getValue()) {
                selectedScene->skyTexture = Asset(type,  _id);
            } else {
                selectedScene->skyTexture = Asset();
            }
        } else if(name == "clear") {
            if(e.getToggle()->getValue()) {
                if(type == "texture") {
                   
                    
                    if(selectedScene->skyTexture.nid != _id &&
                       selectedScene->effectTexture.nid != _id &&
                       selectedScene->secondaryTexture.nid != _id &&
                       selectedScene->landscapeTexture.nid != _id) {
                         project->textures[_id].armed = false;
                        resetTextureSelector();
                    }
                        
                }
                
            }
        }
        
        
        resetTextureSelector();
    }
    
    
    if(e.getCanvasParent() == sceneSelect)
    {
        for(int i=0; i<sceneTabs.size(); i++) {
            if(e.widget == sceneTabs[i]) {
                selectScene(project->scenes[i]);
            }
        }
    }
    
    if(e.getCanvasParent() == projectTopMenu)
    {
        if(e.widget == newSceneBtn) {
            
            if(newSceneBtn->getValue()) {
                project->addScene();
                selectScene(project->scenes.back());
            }
            
        } else if(e.widget == removeSceneBtn) {
            if(removeSceneBtn->getValue()){
                if(project->scenes.size() > 1 && selectedScene != project->activeScene){
                    project->removeScene(selectedScene);
                    selectScene(project->scenes.back());
                }
            }
        } else if(e.widget == cloneSceneBtn) {
            if(cloneSceneBtn->getValue()){
                {
                    project->addScene(selectedScene);
                    selectScene(project->scenes.back());
                }
            }
        } else if(e.widget == saveProjectBtn) {
            if(saveProjectBtn->getValue()){
                //ofSystemSaveDialog(project->projectPath, "save project");
                project->save();
            }
        } else if(e.widget == saveSceneBtn) {
            if(saveSceneBtn->getValue()){
                //ofSystemSaveDialog(project->projectPath, "save project");
                //selectedScene->save();
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
        } else if(e.widget == resetOrientationButton) {
            if(resetOrientationButton->getValue()) {
                
                camAutoOrientationToggle->setValue(false);
                camAutoOrientationToggle->triggerSelf();
                camOrientationZSlider->setValue(0);
                camOrientationZSlider->triggerSelf();
                camOrientationXYPad->setValue(ofVec3f(0,0,0));
                camOrientationXYPad->triggerSelf();
                camOffsetPad->setValue(ofVec3f(0,0,0));
                camOffsetPad->triggerSelf();
                
                
                
                
                
                
                
            }
        }
    }
    
    if(e.getCanvasParent() == effectSettings && selectedScene != NULL && selectedScene->params != NULL)
    {
        Parameters * p = selectedScene->params;
        
        if(e.widget == effectXYPad || e.widget == effectDistanceSlider ) {
            p->effectOffset.set(ofVec3f(effectPositionXY.x, effectPositionXY.y, effectPositionZ));
            
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
        
        effectReplicate.updateParams(p->replicate, e.widget);
        effectSpacing.updateParams(p->replicateSpacing, e.widget);
    }
    
    if(e.getCanvasParent() == projectSettings && selectedScene != NULL && selectedScene->params != NULL)
    {
        Parameters * p = selectedScene->params;
        
        if(e.widget == cueLiveButton ) {
            if(e.getButton()->getValue()) {
                project->activeScene = selectedScene;
            }
        } else if(e.widget == widthInput) {
            if(widthInput->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER)
            {
                //cout << "ON ENTER: ";
                string output = widthInput->getTextString();
                //cout << output << endl;
                
                project->outWidth = ofToInt(output);
                project->updateOutputRes = 2;
            }
        } else if(e.widget == heightInput) {
            if(heightInput->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER)
            {
                //cout << "ON ENTER: ";
                string output = heightInput->getTextString();
                //cout << output << endl;
                
                project->outHeight = ofToInt(output);
                project->updateOutputRes = 2;
            }
        } else if(e.widget == projectNameInput) {
            if(projectNameInput->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER)
            {
                //cout << "ON ENTER: ";
                string path = projectNameInput->getTextString();
                //cout << output << endl;
                //project->projectPath = path;
                project->save(path);
            }
        } else if(e.widget == sceneNameInput) {
            if(sceneNameInput->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER)
            {
                //cout << "ON ENTER: ";
                string name = sceneNameInput->getTextString();
                //cout << output << endl;
                
                selectedScene->name = name;
                resetSceneSelector();
                
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
    
    projectSettings->autoSizeToFitWidgets();
    projectSettings->setPosition(colWidth, 600);
    
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

void Interface::selectScene(Scene * _scene) {
    
    selectedScene = _scene;
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
    
    effectReplicate.x = p->replicate.get().x;
    effectReplicate.y = p->replicate.get().y;
    effectReplicate.z = p->replicate.get().z;
    effectSpacing.val.x = p->replicateSpacing.get().x;
    effectSpacing.val.y = p->replicateSpacing.get().y;
    effectSpacing.val.z = p->replicateSpacing.get().z;
    
    for(int i=0; i<sceneTabs.size(); i++) {
        if(project->scenes[i] == _scene) {
            sceneTabs[i]->setValue(true);
        } else {
            sceneTabs[i]->setValue(false);
        }
    }
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
            
            textureSelect->addWidgetRight(getToggleBtn("primary_texture", i));
            textureSelect->addWidgetRight(getToggleBtn("secondary_texture", i));
            textureSelect->addWidgetRight(getToggleBtn("effect_texture", i));
            textureSelect->addWidgetRight(getToggleBtn("sky_texture", i));
            
            ofxUIButton * clearBtn = new ofxUIButton("clear_texture_"+ofToString(i), false, 20, 20);
            clearBtn->setLabelVisible(false);
            
            textureSelect->addWidgetRight(clearBtn);
        }
    }
    
    for(int i =0; i < project->syphonTextures.size(); i++) {
        if(project->syphonTextures[i].armed) {
            
            numTextures++;
            ofxUIImage * img = new ofxUIImage(40, 20, project->syphonTextures[i].getThumb(), "img_syphon_"+ofToString(i));
            
            img->setLabelVisible(false);
            img->setCropImageToFitRect(true);
            textureSelect->addWidgetDown(img);
            
            textureSelect->addWidgetRight(getToggleBtn("primary_syphon", i));
            textureSelect->addWidgetRight(getToggleBtn("secondary_syphon", i));
            textureSelect->addWidgetRight(getToggleBtn("effect_syphon", i));
            textureSelect->addWidgetRight(getToggleBtn("sky_syphon", i));
            
            /*ofxUIButton * clearBtn = new ofxUIButton("update_syphon_"+ofToString(i), false, 20, 20);
            clearBtn->setLabelVisible(false);*/
            //textureSelect->addWidgetRight(clearBtn);
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
            string _type = ofSplitString(t->getName(), "_")[1];
            int _id = ofToInt(ofSplitString(t->getName(), "_")[2]);
            //cout<<_type<<" "<<_name<<" "<<_id<<endl;
            if(_name == "primary" ) {
                if(_id == selectedScene->landscapeTexture.nid && selectedScene->landscapeTexture.type == _type){
                    t->setValue(true);
                } else {
                    t->setValue(false);
                }
            } else if(_name == "secondary" ) {
                if(_id == selectedScene->secondaryTexture.nid && selectedScene->secondaryTexture.type == _type){
                    t->setValue(true);
                } else {
                    t->setValue(false);
                }
            } else if(_name == "effect" ) {
                if(_id == selectedScene->effectTexture.nid && selectedScene->effectTexture.type == _type){
                    t->setValue(true);
                } else {
                    t->setValue(false);
                }
            } else if(_name == "sky" ) {
                if(_id == selectedScene->skyTexture.nid && selectedScene->skyTexture.type == _type){
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


void Interface::resetSceneSelector() {
    sceneTabs.clear();
    sceneSelect->clearWidgets();
    
    for(int i=0; i<project->getScenes().size(); i++ ) {
        addSceneTab(project->getScenes()[i]->name, i);
    }

}

void Interface::update() {
    
    // keep interface in sync with project
    // optimally this should potentially support another UI controlling the same project in parrallel
 
    if(project->getScenes().size() != sceneTabs.size()) {
        resetSceneSelector();
        
    }
    
    
    for(int i=0; i<sceneTabs.size(); i++) {
        if(project->scenes[i] == selectedScene) {
            sceneTabs[i]->setValue(true);
        } else {
            sceneTabs[i]->setValue(false);
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

void Interface::addSceneTab(string _name, int _index) {
    ofxUILabelButton * btn = new ofxUILabelButton(_name, false);
    btn->setID(_index);
    sceneTabs.push_back(btn);
    
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
            else if(ofContains(ACCEPTED_MODEL_EXTENSIONS, ofToLower(file.getExtension()))) {
                // if coordinates are on texture selector
                project->modelQueue.push_back(file);
                // else if coordinates are on skybox selector
                // use is hit method on ofxUIButton
            }
            
            else if(ofToLower(file.getExtension()) == "xml"){
                
                project->load(file.getAbsolutePath());
                
                projectNameInput->setTextString(project->projectPath);
                
                }
            
        }
    }
}
