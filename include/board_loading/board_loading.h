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
    float ToInches = 1.0f / 1000.0f; // Raw laser data points are given in 1000ths of an inch
    float OffsetPerSample = 1.0f / 64.0f; // in inches
    float OffsetPerProfile = 25.0f; // NOTE: For the hermary configuration, the laser map "dist_from_even_end" increments dist by 25 for each laser head. What is the unit of this distance? Likely 100th of an inch
    
    raw_vertex_data Result;
    raw_vertex Vertex = {};
    
    float MaxX = 0;
    float MinX = 0;
    
    float MinY = 0;
    float MaxY = 0; // TODO: Test the maximum height of an edger board as maximum height
    
    float MinZ = 0;
    float MaxZ = OffsetPerProfile * MAX_NUM_LASERS;
    
    for(int ProfileIndex = 0; ProfileIndex < MAX_NUM_LASERS; ++ProfileIndex)
    {
        if(BoardData->bot_raw_laser_data[ProfileIndex].count)
        {
            float ProfileOffset = ProfileIndex * OffsetPerProfile;
            for(int SampleIndex = 0; SampleIndex < MAX_NUM_CNTS_PER_LUG; ++SampleIndex)
            {
                if(BoardData->bot_raw_laser_data[ProfileIndex].range[SampleIndex])
                {
                    Vertex.x = (SampleIndex + 1) * OffsetPerSample;
                    Vertex.y = BoardData->bot_raw_laser_data[ProfileIndex].range[SampleIndex] * ToInches;
                    Vertex.z = ProfileOffset;
                    Result.BottomData.push_back(Vertex);
                    
                    if(Vertex.x > MaxX)
                        MaxX = Vertex.x;
                    
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
            int ProfileOffset = ProfileIndex * OffsetPerProfile;
            for(int SampleIndex = 0; SampleIndex < MAX_NUM_CNTS_PER_LUG; ++SampleIndex)
            {
                if(BoardData->top_raw_laser_data[ProfileIndex].range[SampleIndex])
                {
                    Vertex.x = (SampleIndex + 1) * OffsetPerSample;
                    Vertex.y = BoardData->bot_raw_laser_data[ProfileIndex].range[SampleIndex] * ToInches;
                    Vertex.z = ProfileOffset;
                    Result.TopData.push_back(Vertex);
                    
                    if(Vertex.x > MaxX)
                        MaxX = Vertex.x;
                    
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
        Result.TopData[i].y = -Map(Result.TopData[i].y, MinY, MaxY, 0, 0.25);
        Result.TopData[i].z = Map(Result.TopData[i].z, MinZ, MaxZ, -1, 1);
    }
    
    for(int i = 0; i < Result.BottomData.size(); ++i)
    {
        Result.BottomData[i].x = Map(Result.BottomData[i].x, MinX, MaxX, -1, 1);
        Result.BottomData[i].y = -Map(Result.BottomData[i].y, MinY, MaxY, 0, 0.25f);
        Result.BottomData[i].z = Map(Result.BottomData[i].z, MinZ, MaxZ, -1, 1);
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
