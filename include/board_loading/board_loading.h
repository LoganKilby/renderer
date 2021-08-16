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

static void *ReadFile(char *Filename)
{
    FILE *FileHandle = fopen(Filename, "rb");
    fseek(FileHandle, 0L, SEEK_END);
    long int FileSize = ftell(FileHandle);
    rewind(FileHandle);
    
    void *Result = malloc(FileSize);
    fread((void *)Result, FileSize, 1, FileHandle);
    fclose(FileHandle);
    
    return Result;
}

static float
Map(float Value, float L1, float H1, float L2, float H2)
{
    float Result = L2 + (Value - L1) * (H2 - L2) / (H1 - L1);
    Assert(Result >= L2 && Result <= H2);
    return Result;
};

static raw_vertex_data
GenerateVertexData(SBoardData *BoardData)
{
    
    float OffsetPerSample = 1.0f / 64.0f; // in inches
    
    raw_vertex_data Result;
    raw_vertex Vertex = {};
    
    float MaxX = 0;
    float MinX = 0;
    
    float MinY = 0;
    float MaxY = 0;
    
    for(int ProfileIndex = 0; ProfileIndex < MAX_NUM_LASERS; ++ProfileIndex)
    {
        if(BoardData->bot_raw_laser_data[ProfileIndex].count)
        {
            for(int SampleIndex = 0; SampleIndex < MAX_NUM_CNTS_PER_LUG; ++SampleIndex)
            {
                if(BoardData->bot_raw_laser_data[ProfileIndex].range[SampleIndex])
                {
                    Vertex.x = (SampleIndex + 1) * OffsetPerSample;
                    Vertex.y = BoardData->bot_raw_laser_data[ProfileIndex].range[SampleIndex];
                    //Vertex.z = 0;
                    Result.BottomData.push_back(Vertex);
                    
                    if(Vertex.x > MaxX)
                        MaxX = Vertex.x;
                    else if(Vertex.x < MinX)
                        MinX = Vertex.x;
                    
                    if(Vertex.y > MaxY)
                        MaxY = Vertex.y;
                    else if(Vertex.y < MinY)
                        MinY = Vertex.y;
                }
            }
        }
    }
    
    for(int ProfileIndex = 0; ProfileIndex < MAX_NUM_LASERS; ++ProfileIndex)
    {
        if(BoardData->top_raw_laser_data[ProfileIndex].count)
        {
            for(int SampleIndex = 0; SampleIndex < MAX_NUM_CNTS_PER_LUG; ++SampleIndex)
            {
                if(BoardData->top_raw_laser_data[ProfileIndex].range[SampleIndex])
                {
                    Vertex.x = (SampleIndex + 1) * OffsetPerSample;
                    Vertex.y = BoardData->bot_raw_laser_data[ProfileIndex].range[SampleIndex];
                    //Vertex.z = 0;
                    Result.TopData.push_back(Vertex);
                    
                    if(Vertex.x > MaxX)
                        MaxX = Vertex.x;
                    else if(Vertex.x < MinX)
                        MinX = Vertex.x;
                    
                    if(Vertex.y > MaxY)
                        MaxY = Vertex.y;
                    else if(Vertex.y < MinY)
                        MinY = Vertex.y;
                }
            }
        }
    }
    
    for(int i = 0; i < Result.TopData.size(); ++i)
    {
        Result.TopData[i].x = Map(Result.TopData[i].x, MinX, MaxX, -1, 1);
        Result.TopData[i].y = Map(Result.TopData[i].y, MinY, MaxY, -1, 1);
        if(Result.TopData[i].y > 0) printf("T.y: %f\n", Result.TopData[i].y);
    }
    
    for(int i = 0; i < Result.BottomData.size(); ++i)
    {
        Result.BottomData[i].x = Map(Result.BottomData[i].x, MinX, MaxX, -1, 1);
        Result.BottomData[i].y = Map(Result.BottomData[i].y, MinY, MaxY, -1, 1);
        if(Result.BottomData[i].y > 0) printf("B.y: %f\n", Result.BottomData[i].y);
    }
    
    return Result;
}

static void
WriteRawVertexData(char *Filename, raw_vertex_data Data)
{
    FILE *FileHandle = fopen(Filename, "w+");
    fprintf(FileHandle, "\nTOP\n");
    for(int i = 0; i < Data.TopData.size(); ++i)
    {
        fprintf(FileHandle, 
                "v %f %f %f\n", 
                Data.TopData[i].x, 
                Data.TopData[i].y, 
                Data.TopData[i].z);
    }
    
    fprintf(FileHandle, "\nBOTTOM\n");
    for(int i = 0; i < Data.BottomData.size(); ++i)
    {
        fprintf(FileHandle, 
                "v %f %f %f\n", 
                Data.BottomData[i].x, 
                Data.BottomData[i].y, 
                Data.BottomData[i].z);
    }
    
    fclose(FileHandle);
}

#endif //BOARD_LOADING_H
