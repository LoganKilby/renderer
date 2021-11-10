/* date = August 16th 2021 2:41 pm */

#ifndef BOARD_LOADING_H
#define BOARD_LOADING_H

#include <vector>

// TODO: Add support for runtime laser selection
#define STD_JS50 1
#define STD_M6 0
#define SCANNER_ANGLED 0

#define PINK V3(1.0f / 255.0f, 1.0f / 192.0f, 1.0f / 203.0f)

#include "include/board_processing/VisionDefectStruct.h"
//#include "include/board_processing/TAV_Interface.h"

#include "include/board_processing/hi_tech.h"

// NOTE: vision image data is packed BGRA

struct v3
{
    union
    {
        struct 
        {
            float x;
            float y;
            float z;
        };
        
        struct
        {
            float r;
            float g;
            float b;
        };
    };
};

struct v2
{
    f32 x, y;
};

struct vertex_attributes
{
    v3 Point;
    v3 Color;
    v2 TexCoords;
};

static v2
V2(f32 a, f32 b)
{
    v2 result = {a, b};
    
    return result;
}

static v2
V2(f32 a)
{
    v2 result = {a, a};
    
    return result;
}

static v3
V3(f32 x, f32 y, f32 z)
{
    v3 result = {x, y, z};
    return result;
}

static v3
V3(f32 a)
{
    v3 result = {a, a, a};
    return result;
}

struct board_color_segments
{
    v3 vne;
    v3 vne_1;
    v3 thick;
    v3 scant;
};

struct vertex_buffer
{
    // NOTE: oraganized by { v3 Point, v3 Color, v2 TexCoord }
    vertex_attributes *Attributes;
    u32 VBO;
    u32 VAO;
    
    f32 MaxX, MinX;
    f32 MaxY, MinY;
    f32 MaxZ, MinZ;
    int Capacity;
    int Count;
};

struct board_render_config
{
    int GradeSelection; // NOTE: There's typically 10 out of 40 active in my experience: Index 0-9
    int DrawCuts;
};

#endif //BOARD_LOADING_H
