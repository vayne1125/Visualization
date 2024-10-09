#ifndef CONSTANTS_H
#define CONSTANTS_H

#pragma once
#include<bits/stdc++.h>

#define PI acos(-1)

#if defined(__linux__) || defined(__APPLE__)
    #define SHADER_DIR "../../src/shaders/"
    #define ASSET_DIR "../../src/asset/"
#else
    #define SHADER_DIR "..\\..\\src\\shaders\\"
    #define ASSET_DIR "..\\..\\src\\asset\\"
#endif

using namespace std;

enum METHODS
{
    ISO_SURFACE, SLICE_METHOD, RAY_CASTING, STREAMLINE, SAMMON_MAPPING, SOM_METHOD
};

enum MODE
{
    AFTER_CALC, BEFORE_CALC
};

enum PROJECTION_METHODS
{
    PERSPECTIVE, ORTHO
};

enum NODE_INIT_POS
{
    RANDOM, GRID
};

enum RENDER_TO
{
    LINES, SURFACES
};

enum FITTING_MESH
{
    CYLINDER, PLANE
};


#endif
