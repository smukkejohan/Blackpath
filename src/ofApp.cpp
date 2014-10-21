#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofShowCursor();
    ofSetLogLevel(OF_LOG_NOTICE);
    
    totalTextures = 0;
    totalModels = 0;
    
    modelTextures.resize(MAX_MODEL_TEXTURES);
    skyImages.resize(MAX_SKY_IMAGES);
    models.resize(MAX_MODELS);
    
    light.enable();
    light.setPointLight();
    light.setPosition(0,0,300);
    
    ofSetWindowTitle("Black Path");
    
    gui = new ofxUICanvas();
    
    //gui->addFPSSlider("FPS");
    gui->autoSizeToFitWidgets();
    
    textureSelector = new ThumbSelector(400, 200, 50, 50);
    for(int i =0; i< MAX_MODEL_TEXTURES; i++) {
        Thumb * thumb = new Thumb();
        thumb->img = modelTextures[i].getThumb();
        textureSelector->thumbs.push_back(thumb);
    }
    
}


//--------------------------------------------------------------
void ofApp::update(){
    
    // load images in paralel thread one image at a time
    if(textureQueue.size() > 0) {
    
        ofFile file = textureQueue.back();
            
        ofLogNotice()<<"Adding texture: "<<" "<<file.getFileName()<<endl;

        threadImgLoader.loadFromDisk(modelTextures[totalTextures], file.getAbsolutePath());
        modelTextures[totalTextures].setChanged();
        
        totalTextures++;
        
        textureQueue.pop_back();
    }
    
    
    if(modelQueue.size() > 0) {
        
        ofFile file = modelQueue.back();
        
        ofLogNotice()<<"Adding model: "<<" "<<file.getFileName()<<endl;
        
        //threadImgLoader.loadFromDisk(modelTextures[totalModels], file.getAbsolutePath());
        
        // todo load threaded from buffer then load to model when needed by GPU
        models[totalModels].loadModel(file.getAbsolutePath());
        //models[totalModels].getMesh(0).getNormalsPointer()
        totalModels++;
        
        modelQueue.pop_back();
    }
    
    
    for(int i =0; i< modelTextures.size(); i++) {
        modelTextures[i].update();
    }
    
    
    textureSelector->update();
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableLighting();
    ofEnableDepthTest();
    
    cam.begin();
    
    if(totalTextures > 0 && totalModels > 0) {
        modelTextures[totalTextures-1].bind();
        ofSetColor(255, 255, 255);
        models[totalModels-1].drawFaces();
        modelTextures[totalTextures-1].unbind();
    }
    
    cam.end();

    ofDisableDepthTest();
    ofDisableLighting();
    
    ofSetColor(255, 255, 255);
    
    vector<ModelTexture>::iterator it;
    int i=0;
    for(it = modelTextures.begin(); it != modelTextures.end(); ++it, ++i) {
        //if(it->isAllocated()) it->getThumb()->draw(0,i*50);
        // todo make selectable
    }
    
    vector<Model>::iterator mit;
    i=0;
    for(mit = models.begin(); mit != models.end(); ++mit, ++i) {
        if(mit->hasMeshes()) mit->getThumb()->draw(0,i*50);
    }
    
    textureSelector->draw();
    
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
    delete gui;
}
