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

static void
YValueTest(SBoardData *BoardData)
{
    int BeginIndex = BoardData->last_board_data.implement_data.beg_ele;
    int EndIndex = BoardData->last_board_data.implement_data.end_ele + 1;
    int MinThkArrayCount = 2300;
    for(int i = BeginIndex; i < EndIndex; ++i)
    {
        //RAW_LASER_DATA_STRUCT *RawData = &BoardData->bot_raw_laser_data[i];
        PROFILE_DATA_STN *ProfileData = &BoardData->last_board_data.profile_data[i];
        int j = 0;
        while(ProfileData->thk_array[j] == 0)
        {
            ++j;
        }
        
        int Y1 = ProfileData->thk_array[j];
        int Y2 = ProfileData->lead_edge_y;
        int Y3 = ProfileData->trail_edge_y;
        Assert(ProfileData->thk_array[j] == ProfileData[i].lead_edge_y);
    }
    
    printf("min count in thk_array: %d\n", MinThkArrayCount);
}

static raw_vertex_data
GenerateVertexDataFromRaw(SBoardData *BoardData)
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
        if(BoardData->bot_raw_laser_data[ProfileIndex].count == 0)
        {
            continue;
        }
        
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
    
    for(int i = 0; i < Result.BottomData.size(); ++i)
    {
        Result.BottomData[i].x = Map(Result.BottomData[i].x, MinX, MaxX, -1, 1);
        Result.BottomData[i].y = -Map(Result.BottomData[i].y, MinY, MaxY, -1, 1);
        Result.BottomData[i].z = Map(Result.BottomData[i].z, MinZ, MaxZ, -1, 1);
    }
    
    return Result;
}

// Smooth data plan size: MAX_NUM_FT_OF_LIGHT_CURTAIN * NUM_LIGHT_CURTAIN_ELEMENTS_FT
// MAX_NUM_FT_OF_LIGHT_CURTAIN = MAX_LEN_BRD + 1
// NUM_LIGHT_CURTAIN_ELEMENTS_FT = 48

internal std::vector<v3>
ProcessProfileData(SBoardData *BoardData)
{
    PROFILE_DATA_STN *Profiles = &BoardData->last_board_data.profile_data[0];
    
    int CutCount = 0;
    int CutPositions[5];
    for(int i = 0; i < 5; ++i)
    {
        if(Profiles[0].cut[i])
        {
            CutPositions[i] = Profiles[0].cut[i];
            CutCount++;
        }
    }
    
    float MaxX = 0, MinX = 0, MaxY = 0, MinY = 0;
    float ZOffsetPerProfile = 25.0f;
    float XOffsetPerSample = 1.0f / 64.0f; // in inches
    int ProfileCount = MAX_NUM_LASERS + 1;
    
    v3 Vertex;
    float ProfileOffset;
    float SampleOffset;
    float SampleY;
    
    int StartIndex = BoardData->last_board_data.implement_data.beg_ele;
    int EndIndex = BoardData->last_board_data.implement_data.end_ele + 1;
    MinX = BoardData->last_board_data.profile_data[StartIndex].lead_edge_x;
    std::vector<v3> Result;
    Result.reserve(EndIndex - StartIndex);
    for(int ProfileIndex = StartIndex; ProfileIndex < EndIndex; ++ProfileIndex)
    {
        if(Profiles[ProfileIndex].lead_edge_x)
        {
            ProfileOffset = ZOffsetPerProfile * ProfileIndex;
            int LeadEdgeX = Profiles[ProfileIndex].lead_edge_x;
            int LeadEdgeY = Profiles[ProfileIndex].lead_edge_y;
            for(int SampleIndex = 0; SampleIndex < 2300; ++SampleIndex)
            {
                SampleY = Profiles[ProfileIndex].thk_array[SampleIndex];
                if(SampleY)
                {
                    Vertex.x = LeadEdgeX + SampleIndex * XOffsetPerSample;
                    Vertex.y = LeadEdgeY + SampleY;
                    Vertex.z = ProfileOffset;
                    Result.push_back(Vertex);
                    
                    if(Vertex.x > MaxX) MaxX = Vertex.x;
                    if(Vertex.x < MinX) MinX = Vertex.x;
                    if(Vertex.y > MaxY) MaxY = Vertex.y;
                    if(Vertex.y < MinY) MinY = Vertex.y;
                }
            }
        }
    }
    
    float MaxZ = ZOffsetPerProfile * ProfileCount;
    for(int VertexIndex = 0; VertexIndex < Result.size(); ++VertexIndex)
    {
        Result[VertexIndex].x = Map(Result[VertexIndex].x, MinX, MaxX, -1, 1);
        Result[VertexIndex].y = Map(Result[VertexIndex].y, MinY, MaxY, -1, 1);
        Result[VertexIndex].z = Map(Result[VertexIndex].z, 0, MaxZ, -1, 1);
    }
    
    return Result;
}

static void
WriteRawVertexData(char *Filename, v3 *Data, int Count)
{
    FILE *FileHandle = fopen(Filename, "w+");
    
    fprintf(FileHandle, "TOP\n");
    fprintf(FileHandle, "%d\n", Count);
    for(int i = 0; i < Count; ++i)
    {
        fprintf(FileHandle, 
                "v %f %f %f\n", 
                Data[i].x, 
                Data[i].y, 
                Data[i].z);
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
