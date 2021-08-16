/* date = August 16th 2021 2:41 pm */

#ifndef BOARD_LOADING_H
#define BOARD_LOADING_H

#include <vector>

// A laser must be defined before including this file. Example: #define STD_824S1 1
#include "hi_tech.h"

struct raw_vertex
{
    float x;
    float y;
    float z;
};

struct raw_vertex_data
{
    std::vector<raw_vertex> TopData;
    std::vector<raw_vertex> BottomData;
};

static void *ReadFile(char *Filename, long int *BufferSize)
{
    FILE *FileHandle = fopen(Filename, "rb");
    fseek(FileHandle, 0L, SEEK_END);
    long int FileSize = ftell(FileHandle);
    rewind(FileHandle);
    
    void *Result = malloc(FileSize);
    fread((void *)Result, FileSize, 1, FileHandle);
    fclose(FileHandle);
    
    *BufferSize = FileSize;
    return Result;
}

static raw_vertex_data
GenerateVertexData(SBoardData *BoardData)
{
    
    float OffsetPerSample = 1.0f / 64.0f; // in inches
    
    raw_vertex_data Result;
    raw_vertex Vertex = {};
    
    int BottomProfileCount = 0;
    for(int ProfileIndex = 0; ProfileIndex < MAX_NUM_LASERS; ++ProfileIndex)
    {
        if(BoardData->bot_raw_laser_data[ProfileIndex].count)
        {
            BottomProfileCount++;
            
            for(int SampleIndex = 0; SampleIndex < MAX_NUM_CNTS_PER_LUG; ++SampleIndex)
            {
                if(BoardData->bot_raw_laser_data[ProfileIndex].range[SampleIndex])
                {
                    Vertex.x = (SampleIndex + 1) * OffsetPerSample;
                    Vertex.y = BoardData->bot_raw_laser_data[ProfileIndex].range[SampleIndex];
                    //Vertex.z = 0;
                    Result.BottomData.push_back(Vertex);
                }
            }
            
        }
    }
    
    int TopProfileCount = 0;
    for(int ProfileIndex = 0; ProfileIndex < MAX_NUM_LASERS; ++ProfileIndex)
    {
        if(BoardData->top_raw_laser_data[ProfileIndex].count)
        {
            TopProfileCount++;
            
            for(int SampleIndex = 0; SampleIndex < MAX_NUM_CNTS_PER_LUG; ++SampleIndex)
            {
                if(BoardData->top_raw_laser_data[ProfileIndex].range[SampleIndex])
                {
                    Vertex.x = (SampleIndex + 1) * OffsetPerSample;
                    Vertex.y = BoardData->bot_raw_laser_data[ProfileIndex].range[SampleIndex];
                    //Vertex.z = 0;
                    Result.TopData.push_back(Vertex);
                }
            }
        }
        
    }
    
    return Result;
}

#endif //BOARD_LOADING_H
