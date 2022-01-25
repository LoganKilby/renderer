/* date = August 16th 2021 2:41 pm */

#ifndef BOARD_LOADING_H
#define BOARD_LOADING_H

// TODO: Add support for runtime laser selection
#define STD_JS50 1
#define STD_M6 0
#define SCANNER_ANGLED 0

#define PINK V3(1.0f / 255.0f, 1.0f / 192.0f, 1.0f / 203.0f)

#include "include/board_processing/VisionDefectStruct.h"
//#include "include/board_processing/TAV_Interface.h"

#include "include/board_processing/hi_tech.h"

// NOTE: vision image data is packed BGRA

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
    
    bounds Bounds;
    u32 ProfileCount;
    int Capacity;
    int Count;
};

struct board_render_config
{
    int GradeSelection; // NOTE: There's typically 10 out of 40 active in my experience: Index 0-9
    int DrawCuts;
};

#endif //BOARD_LOADING_H
