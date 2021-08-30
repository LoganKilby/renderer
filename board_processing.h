/* date = August 16th 2021 2:41 pm */

#ifndef BOARD_LOADING_H
#define BOARD_LOADING_H

#include <vector>

// TODO: Add support for runtime laser selection
#define STD_824S1 1
#include "include/board_processing/hi_tech.h"

// Idk
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

struct board_color_segments
{
    v3 vne;
    v3 vne_1;
    v3 thick;
    v3 scant;
};

struct v6
{
    float x;
    float y;
    float z;
    
    float r;
    float g;
    float b;
};

struct raw_vertex_data
{
    std::vector<v6> TopData;
    std::vector<v6> BottomData;
};

struct vertex_buffer
{
    v6 *Memory;
    int ElementCapacity;
    int ElementCount;
};

struct board_render_config
{
    int GradeSelection; // NOTE: There's typically 10 out of 40 active in my experience: Index 0-9
    int DrawCuts;
};

#endif //BOARD_LOADING_H
