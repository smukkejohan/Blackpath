//
//  defines.h
//  Blackpath
//
//  Created by Johan Bichel Lindegaard on 18/02/15.
//
//

#pragma once
#include "ofMain.h"

#define MAX_TEXTURES 42
#define MAX_SYPHON_TEXTURES 6
#define MAX_COLOR_TEXTURES 3
#define MAX_MODELS 36
#define MAX_TABS 24
#define DEBUG true

static const string _AIE[] = {"png","jpg","jpeg"};
static const vector<string> ACCEPTED_IMAGE_EXTENSIONS(_AIE, _AIE + (sizeof _AIE / sizeof _AIE[0]));

static const string _AME[] = {"obj"};
static const vector<string> ACCEPTED_MODEL_EXTENSIONS(_AME, _AME + (sizeof _AME / sizeof _AME[0]));
