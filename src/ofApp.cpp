#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofShowCursor();
    
#ifdef OF_RELEASE
    ofSetDataPathRoot("../Resources/Data/");
    ofSetWorkingDirectoryToDefault();
    ofSetLogLevel(OF_LOG_ERROR);
    ofLogToFile("blackpath.log", true);
#endif
    
    glEnable(GL_TEXTURE_2D);
    
    ofShowCursor();
    ofSetLogLevel(OF_LOG_NOTICE);
    
    //setup our directory
    dir.setup();
    //register for our directory's callbacks
    ofAddListener(dir.events.serverAnnounced, this, &ofApp::serverAnnounced);
    // not yet implemented
    //ofAddListener(dir.events.serverUpdated, this, &testApp::serverUpdated);
    ofAddListener(dir.events.serverRetired, this, &ofApp::serverRetired);
    
    project = new Project();
    project->load("defaultProject.xml");
    ui = new Interface(project);
    
    ofSetWindowTitle("Black Path");
    
    _width  = project->getSettings().getValue("outwidth", 1920);
    _height = project->getSettings().getValue("outheight", 1080);
    
    syphonOut.setName("Landscape");
    ui->setup();
    
    liveRenderer    = new Renderer(project);
    previewRenderer = new Renderer(project);
    
    liveRenderer->setup();
    previewRenderer->setup();
    

}

//--------------------------------------------------------------
void ofApp::update(){
    
    ui->update();
    
    liveRenderer->scene =    project->activeScene;
    previewRenderer->scene = project->previewScene;
    
    liveRenderer->update();
    previewRenderer->update();
    
    project->update();
    
    for(int i =0; i< project->textures.size(); i++) {
        project->textures[i].update();
    }
    for(int i =0; i< project->models.size(); i++) {
        project->models[i].update();
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
    
    liveRenderer->render();
    previewRenderer->render();
    
    previewRenderer->fbo.draw(200,200, previewRenderer->fbo.getWidth()/2, previewRenderer->fbo.getHeight()/2);
    ui->draw();
    
    // TODO: Render output monitors for live and preview
    ofSetColor(255, 255, 255);
    syphonOut.publishTexture(&previewRenderer->fbo.getTexture());
    
    ofDrawBitmapString(previewRenderer->scene->name, 200, 200);
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

void ofApp::exit() {
    
    project->save();
    
    // free memory ?
}

//these are our directory's callbacks
void ofApp::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
        
        for(int i=0; i<project->syphonTextures.size(); i++) {
            if(!project->syphonTextures[i].armed) {
                ofxSyphonClient * c = new ofxSyphonClient();
                c->setup();
                c->set(dir.serverName, dir.appName);
                project->syphonTextures[i].arm(c);
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
