/* date = August 16th 2021 2:41 pm */

#ifndef BOARD_LOADING_H
#define BOARD_LOADING_H

#include <vector>

// TODO: Add support for runtime laser selection
#define STD_824S1 1
#include "hi_tech.h"

struct v3
{
    float x;
    float y;
    float z;
};

struct raw_vertex_data
{
    std::vector<v3> TopData;
    std::vector<v3> BottomData;
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
    float XOffsetPerSample = 1.0f / 64.0f; // in inches
    float ZOffsetPerProfile = 25.0f; // NOTE: For the hermary configuration, the laser map "dist_from_even_end" increments dist by 25 for each laser head. What is the unit of this distance? Likely 100th of an inch
    
    raw_vertex_data Result;
    
    v3 Vertex = {};
    
    float MaxX = 0;
    float MinX = 0;
    
    float MinY = 0;
    float MaxY = 0; // TODO: Use the maximum height of an edger board
    
    float MinZ = 0;
    float MaxZ = ZOffsetPerProfile * MAX_NUM_LASERS;
    
    for(int ProfileIndex = 0; ProfileIndex < MAX_NUM_LASERS; ++ProfileIndex)
    {
        if(BoardData->bot_raw_laser_data[ProfileIndex].count)
        {
            float ZProfileOffset = ProfileIndex * ZOffsetPerProfile;
            
            for(int SampleIndex = 0; SampleIndex < MAX_NUM_CNTS_PER_LUG; ++SampleIndex)
            {
                if(BoardData->bot_raw_laser_data[ProfileIndex].range[SampleIndex])
                {
                    Vertex.x = (SampleIndex + 1) * XOffsetPerSample;
                    Vertex.y = BoardData->bot_raw_laser_data[ProfileIndex].range[SampleIndex] * ToInches;
                    Vertex.z = ZProfileOffset;
                    
                    Result.BottomData.push_back(Vertex);
                    
                    if(Vertex.y > MaxY)
                        MaxY = Vertex.y;
                    else if(Vertex.y < MinY)
                        MinY = Vertex.y;
                    
                    if(Vertex.x > MaxX)
                        MaxX = Vertex.x;
                }
            }
        }
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
    
    fprintf(FileHandle, "TOP\n");
    fprintf(FileHandle, "%zd\n", Data.TopData.size());
    for(int i = 0; i < Data.TopData.size(); ++i)
    {
        fprintf(FileHandle, 
                "v %f %f %f\n", 
                Data.TopData[i].x, 
                Data.TopData[i].y, 
                Data.TopData[i].z);
    }
    
    fprintf(FileHandle, "\nBOTTOM\n");
    fprintf(FileHandle, "%zd\n", Data.BottomData.size());
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

static bool
FloatLessThanEqual(float A, float B)
{
    return A <= B ? true : false;
}

static bool
FloatGreaterThanEqual(float A, float B)
{
    return A >= B ? true : false;
}

static std::vector<v3>
FilterRawData(std::vector<v3> *RawData, bool CompareFunction(float, float))
{
    std::vector<v3> Result;
    
    float YValueSum = 0.0f;
    float AverageHeight;
    
    for(int i = 0; i < RawData->size(); ++i)
    {
        YValueSum += abs((*RawData)[i + 1].y - (*RawData)[i].y);
    }
    
    AverageHeight = YValueSum / RawData->size();
    
    v3 CurrentPoint;
    v3 NextPoint;
    v3 PrevPoint = (*RawData)[0];
    for(int i = 0; i < RawData->size(); ++i)
    {
        CurrentPoint = (*RawData)[i];
        NextPoint = (*RawData)[i + 1];
        PrevPoint = (*RawData)[i - 1];
        
        if(abs(CurrentPoint.y) - abs(PrevPoint.y) < AverageHeight)
        {
            Result.push_back(CurrentPoint);
        }
    }
    
    return Result;
}

#endif //BOARD_LOADING_H
