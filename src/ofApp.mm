#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofShowCursor();
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetWindowTitle("Black Path");
    //syphonClients.resize(MAX_SYPHON_TEXTURES);
    
    
#ifdef OF_RELEASE
    ofSetDataPathRoot("../Resources/Data/");
    ofSetWorkingDirectoryToDefault();
    ofSetLogLevel(OF_LOG_ERROR);
    ofLogToFile("blackpath.log", true);
#endif
    
    project = new Project();
    project->setup();
    project->load("defaultProject.xml");
    ui = new Interface(project);
    
    glEnable(GL_TEXTURE_2D);
    ofSetFrameRate(30);
    _width  = project->outWidth;;
    _height = project->outHeight;
    
    ui->setup();
    
    liveRenderer    = new Renderer("live", project);
    previewRenderer = new Renderer("preview", project);
    
    liveRenderer->setup();
    previewRenderer->setup();// TODO pass in fbo settings for preview and live
    
    //setup our directory
    directory.setup();
    ofAddListener(directory.events.serverAnnounced, this, &ofApp::serverAnnounced);
    ofAddListener(directory.events.serverUpdated, this, &ofApp::serverUpdated);
    ofAddListener(directory.events.serverRetired, this, &ofApp::serverRetired);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ui->update();
    
    liveRenderer->scene =    project->activeScene; // todo active scene
    previewRenderer->scene = project->previewScene;
    
    liveRenderer->update();
    if(project->enablePreview) {
        previewRenderer->update();
    }
    
    project->update();
    
    for(int i =0; i< project->textures.size(); i++) {
        project->textures[i].update();
    }
    
    for(int i =0; i< project->syphonTextures.size(); i++) {
        project->syphonTextures[i].update();
    }
    
    for(int i =0; i< project->models.size(); i++) {
        project->models[i].update();
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
    
    // todo: if the live is the same as output we should reuse the fbo for preview instead of rendering again
    
    ofFill();
    ofBackground(60, 60, 60);
    ofSetColor(255, 255, 255);
    
    liveRenderer->render();
    if(project->enablePreview) {
        previewRenderer->render();
    
        previewRenderer->fbo.draw(300,100, previewRenderer->fbo.getWidth()/4,   previewRenderer->fbo.getHeight()/4);
    }
    
    ofSetColor(255,0,0);
    ofDrawRectangle(290, 110+ previewRenderer->fbo.getHeight()/4, liveRenderer->fbo.getWidth()/4+20, liveRenderer->fbo.getHeight()/4+20);
    ofSetColor(255, 255, 255);
    liveRenderer->fbo.draw(300,120+ previewRenderer->fbo.getHeight()/4, liveRenderer->fbo.getWidth()/4, liveRenderer->fbo.getHeight()/4);
    
    // TODO: Render output monitors for live and preview
    ofSetColor(255, 255, 255);
    
    ofDrawBitmapString("Preview: " + previewRenderer->scene->name, 300, 100);
    
    ofDrawBitmapString("Live: " + liveRenderer->scene->name, 300, 120+previewRenderer->fbo.getHeight()/4);
    
    //todo syponh enabled setting on renderers, don't publish preview
    previewRenderer->publishSyphon();
    liveRenderer->publishSyphon();

    
    /*for(int i=0; i<numSyphonClients; i++) {
        if(syphonClients[i].isSetup()) {
            syphonClients[i].draw(100, 100*i, 100, 100);
                        ofDrawBitmapString(syphonClients[i].getServerName()+" "+syphonClients[i].getApplicationName(), 70, 100*i);
        
        }
    }*/
    
    
    
    for(int i=0; i<MAX_SYPHON_TEXTURES; i++) {
     if(project->syphonTextures[i].armed && project->syphonTextures[i].client.isSetup()) {
         
         /*project->syphonTextures[i].client.draw(100, 100*i, 100, 100);
         ofDrawBitmapString(project->syphonTextures[i].client.getServerName()+" "+project->syphonTextures[i].client.getApplicationName(), 70, 100*i);
     
        }*/
         //project->syphonTextures[i].drawThumb()
     }
        
     }
    
    ui->draw();

    
    
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
    ui->windowResized(w,h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    ui->dragEvent(dragInfo);
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
        
        if(directory.serverExists(dir.serverName, dir.appName)) {
            
            /*syphonClients[numSyphonClients].setup();
            syphonClients[numSyphonClients].setApplicationName(dir.appName);
            syphonClients[numSyphonClients].setServerName(dir.serverName);
            numSyphonClients++;
            */
            
            for(int i=0; i<project->syphonTextures.size();i++) {
                if(project->syphonTextures[i].armed == false) {
                   
                    project->syphonTextures[i].client.setup();
                    project->syphonTextures[i].client.setApplicationName(dir.appName);
                    project->syphonTextures[i].client.setServerName(dir.serverName);
                    
                    project->syphonTextures[i].arm();
                    project->bTextureAssetsChanged = true;
                    break;
                }
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
