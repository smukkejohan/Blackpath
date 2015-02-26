//
//  ofxMultiSelector.h
//  Blackpath
//
//  Created by Johan Bichel Lindegaard on 15/02/15.
//
//

#pragma once

#include "ofMain.h"

class MultiSelectorItem {
    
public:
    
    MultiSelectorItem() {
        selected = false;
        hover = false;
        pressed = false;
        active = false;
        bChanged = false;
        //type = "texture";
    }
    
    int nid;
    ofRectangle rect;
    bool bChanged;
    string type;
    
    ofImage * img;
    //SyphonTexture * syphonTexture;
    
    bool active;
    
    void update() {
        
    };
    
    void draw() {
        ofPushMatrix();
        ofPushStyle();
        if(hover || selected) {
            ofSetColor(255,255,255);
        } else {
            ofSetColor(100,100,100);
        }
        
        ofDrawRectangle(rect);
        
        if(type == "syphon") {
            ofPushMatrix();
            ofTranslate(rect.x,rect.y);
            //syphonTexture->drawThumb();
            ofPopMatrix();
        } else {
            if(img->isAllocated()){
                img->draw(rect);
            }
        }
        
        if(pressed || selected) {
            ofRectangle pressedRect = rect;
            pressedRect.scaleFromCenter(0.9);
            ofDrawRectangle(pressedRect);
        } else {
            
        }
        
        ofPopStyle();
        ofPopMatrix();
    };
    
    bool hover;
    bool selected; // might need multiple states
    bool pressed;
    //ofEvent selected;
};


class MultiSelectorEventData {
public:
    MultiSelectorEventData() {
    }
    string title;
    string event;
    MultiSelectorItem * item;
    int thumbNum;
};



class MultiSelector {
    friend MultiSelectorItem;
    
public:
    
    MultiSelector(int _w, int _h, int _itemW, int _itemH, string _title = "Selector"){
        rect.width = _w;
        rect.height = _h;
        itemWidth = _itemW;
        itemHeight = _itemH;
        padding = 6;
        title = _title;
        center = false;
        ofRegisterMouseEvents(this);
    };
    
    ~MultiSelector() {
        ofUnregisterMouseEvents(this);
    };
    
    ofEvent<MultiSelectorEventData> selectorEvent;
    
    void update() {
        // todo only update an fbo when needed
        
        for(int i=0; i<items.size(); i++) {
            items[i]->update();
        }
    };
    
    void draw(int _x=0, int _y=0) {
        rect.x = _x;
        rect.y = _y;
        
        int itemsPerRow = (rect.width-(padding*2)) / (itemWidth+padding);
        float rowWidth = (itemsPerRow * (itemWidth+padding)) + (padding);
        int rows = ceil(items.size()/itemsPerRow);
        float columnHeight = (rows * (itemHeight+padding)) + padding;
        masterPadding = 0;
        if(center) {
            masterPadding = (getWidth()-rowWidth)/2; // X padding
        }
        
        innerRect.set(rect.x, rect.y, rowWidth, columnHeight);
        
        ofSetColor(60,60,60);
        ofDrawRectangle(rect);
        
        for(int i=0; i<items.size(); i++) {
            int row = i/itemsPerRow;
            int xin = masterPadding + rect.x + padding + (i - ((row)*itemsPerRow))*(itemWidth+padding);
            int yin = rect.y + padding + (row*(itemHeight+padding));
            
            ofNoFill();
            items[i]->rect.set(xin, yin, itemWidth, itemHeight);
            items[i]->draw();
        }
        
        if(hover) {
            ofSetColor(255,255,255);
            ofDrawRectangle(innerRect);
        } else {
            ofSetColor(100,100,100);
            ofDrawRectangle(innerRect);
        }
    };
    
    float getHeight() {
        return rect.height;
    };
    
    float getWidth() {
        return rect.width;
    };
    
    void mouseMoved(ofMouseEventArgs &args){
        // if mouse on top highlight
        hover = rect.inside(args.x, args.y);
        for(int i=0; i< items.size(); i++) {
            items[i]->hover = items[i]->rect.inside(args.x, args.y);
        }
    };
    void mouseScrolled(ofMouseEventArgs &args){
    };
    void mouseReleased(ofMouseEventArgs &args) {
        for(int i=0; i< items.size(); i++) {
            items[i]->selected = false;
            
            if(items[i]->pressed) {
                
                items[i]->pressed = false;
                
                if(items[i]->rect.inside(args.x, args.y)) {
                    items[i]->selected = true;
                    MultiSelectorEventData eData;
                    eData.title = title;
                    eData.item = items[i];
                    eData.event = "inside";
                    eData.thumbNum = i;
                    
                    ofNotifyEvent(selectorEvent, eData, this);
                    
                } else {
                    MultiSelectorEventData eData;
                    eData.title = title;
                    eData.item = items[i];
                    eData.event = "outside";
                    eData.thumbNum = i;
                    
                    ofNotifyEvent(selectorEvent, eData, this);
                }
            }
        }
    };
    
    void mousePressed(ofMouseEventArgs &args){
        if(rect.inside(args.x, args.y)) {
            for(int i=0; i< items.size(); i++) {
                if(items[i]->rect.inside(args.x, args.y)) {
                    items[i]->pressed = true;
                } else {
                    items[i]->pressed = false;
                }
            }
        }
    };
    
    void mouseDragged(ofMouseEventArgs &args){
    };
    
    MultiSelectorItem * getSelected() {
        return selectedItem;
    };
    
    //int rows;
    //int columns;
    vector<MultiSelectorItem *> items;
    
private:
    MultiSelectorItem * selectedItem;
    bool center;
    string title;
    float padding;
    float itemWidth;
    float itemHeight;
    float masterPadding;
    bool hover;
    ofRectangle rect;
    ofRectangle innerRect;
    
};
