#include "board_processing.h"

internal void
PushVertex(vertex_buffer *Buffer, v6 Element)
{
    Assert(Buffer->ElementCount < Buffer->ElementCapacity);
    Buffer->Memory[Buffer->ElementCount++] = Element;
}

internal void
ClearVertexBuffer(vertex_buffer *Buffer)
{
    int CapacityInBytes = Buffer->ElementCapacity * sizeof(v6);
    memset(Buffer->Memory, 0, CapacityInBytes);
    Buffer->ElementCount = 0;
}

internal vertex_buffer
CreateVertexBuffer(int ElementsToAllocate)
{
    vertex_buffer Result = {};
    Result.ElementCapacity = ElementsToAllocate;
    Result.Memory = (v6 *)malloc(Result.ElementCapacity * sizeof(v6));
    
    return Result;
}

internal void
UploadBufferedVertices(vertex_buffer Buffer, unsigned int VertexAttributeObject)
{
    glBindVertexArray(VertexAttributeObject);
    int BufferedBytes = Buffer.ElementCount * sizeof(v6);
    glBufferSubData(GL_ARRAY_BUFFER, 0, BufferedBytes, Buffer.Memory);
    glBindVertexArray(0);
}

internal void *ReadFile(char *Filename)
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

internal float
Map(float Value, float L1, float H1, float L2, float H2)
{
    float Result = L2 + (Value - L1) * (H2 - L2) / (H1 - L1);
    Assert(Result >= L2 && Result <= H2);
    return Result;
};

internal void
YValueTest(SBoardData *BoardData)
{
    int BeginIndex = BoardData->last_board_data.implement_data.beg_ele;
    int EndIndex = BoardData->last_board_data.implement_data.end_ele + 1;
    int MinThkArrayCount = 2300;
    for(int i = BeginIndex; i < EndIndex; ++i)
    {
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

internal raw_vertex_data
GenerateVertexDataFromRaw(SBoardData *BoardData)
{
    float ToInches = 1.0f / 1000.0f; // Raw laser data points are given in 1000ths of an inch
    float XOffsetPerSample = 1.0f / 64.0f; // in inches
    float ZOffsetPerProfile = 25.0f; // NOTE: For the hermary configuration, the laser map "dist_from_even_end" increments dist by 25 for each laser head. What is the unit of this distance? Likely 100th of an inch
    
    raw_vertex_data Result;
    
    v6 Vertex = {};
    
    float MaxX = 0;
    float MinX = 0;
    
    float MinY = 0;
    float MaxY = 0; // TODO: Use the maximum height of an edger board
    
    float MinZ = 0;
    float MaxZ = ZOffsetPerProfile * MAX_NUM_LASERS;
    
    for(int ProfileIndex = 0; ProfileIndex < MAX_NUM_LASERS; ++ProfileIndex)
    {
        if(BoardData->top_raw_laser_data[ProfileIndex].count == 0)
        {
            continue;
        }
        
        float ZProfileOffset = ProfileIndex * ZOffsetPerProfile;
        for(int SampleIndex = 0; SampleIndex < MAX_NUM_CNTS_PER_LUG; ++SampleIndex)
        {
            if(BoardData->top_raw_laser_data[ProfileIndex].range[SampleIndex])
            {
                Vertex.x = (SampleIndex + 1) * XOffsetPerSample;
                Vertex.y = BoardData->top_raw_laser_data[ProfileIndex].range[SampleIndex] * ToInches;
                Vertex.z = ZProfileOffset;
                Vertex.r = 0.5f;
                Vertex.g = 0.5f;
                Vertex.b = 0.5f;
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

internal void
ProcessProfileData(vertex_buffer *VertexBuffer, SBoardData *BoardData, int Grade, board_color_segments ColorSegments)
{
    PROFILE_DATA_STN *Profiles = &BoardData->last_board_data.profile_data[0];
    
    float ZOffsetPerProfile = 25.0f;
    float XOffsetPerSample = (1.0f / 64.0f) * 1000; // in 1000th's of an inch
    int ProfileCount = MAX_NUM_LASERS + 1;
    
    float ProfileOffset;
    float SampleOffset;
    float SampleY;
    
    int StartIndex = BoardData->last_board_data.implement_data.beg_ele;
    int EndIndex = BoardData->last_board_data.implement_data.end_ele + 1;
    
    float MinX = 0;
    float MaxX = 32000.0f;
    float MinY = -4000.0f;
    float MaxY = 4000.0f;
    float MaxZ = ZOffsetPerProfile * ProfileCount;
    float MinZ = 0.0f;
    
    v6 Vertex;
    for(int ProfileIndex = StartIndex; ProfileIndex < EndIndex; ++ProfileIndex) // profiles[]
    {
        if(Profiles[ProfileIndex].lead_edge_x)
        {
            ProfileOffset = ZOffsetPerProfile * ProfileIndex;
            int LeadEdgeX = Profiles[ProfileIndex].lead_edge_x;
            int LeadEdgeY = Profiles[ProfileIndex].lead_edge_y;
            int TrailEdgeX = Profiles[ProfileIndex].trail_edge_x;
            
            // NOTE: regions of coloration
            //       vne    red
            //       vne1   orange
            //       thick  yellow
            //       scant  (not implemented)
            int ScantStartX = Profiles[ProfileIndex].grade_thick[Grade].lead_scant_t;
            int ScantEndX = Profiles[ProfileIndex].grade_thick[Grade].trail_scant_t;
            int ThickStartX = Profiles[ProfileIndex].grade_thick[Grade].lead_thick_x;
            int ThickEndX = Profiles[ProfileIndex].grade_thick[Grade].trail_thick_x;
            int VNE1StartX = Profiles[ProfileIndex].grade_thick[Grade].lead_vne1_x;
            int VNE1EndX = Profiles[ProfileIndex].grade_thick[Grade].trail_vne1_x;
            int VNEStartX = Profiles[ProfileIndex].grade_thick[Grade].lead_vne_x;
            int VNEEndX = Profiles[ProfileIndex].grade_thick[Grade].trail_vne_x;
            
            int SampleCount = 0;
            for(int SampleIndex = 0; SampleIndex < 2300; ++SampleIndex) // thk_array[]
            {
                SampleY = Profiles[ProfileIndex].thk_array[SampleIndex];
                if(SampleY)
                {
                    // NOTE: By incrementing distance for every valid y-value instead of for every
                    // possible y-value the result looks more reasonable
                    Vertex.x = LeadEdgeX + (SampleCount++ * XOffsetPerSample);
                    Vertex.y = LeadEdgeY + SampleY;
                    Vertex.z = ProfileOffset;
                    
                    // NOTE: grade_thick for each grade (represented by color) can be unique
                    // per-profile.
                    if((Vertex.x >= ScantStartX) && (Vertex.x <= ScantEndX))
                    {
                        Vertex.r = ColorSegments.scant.r;
                        Vertex.g = ColorSegments.scant.g;
                        Vertex.b = ColorSegments.scant.b;
                    }
                    else if((Vertex.x >= ThickStartX) && (Vertex.x <= ThickEndX))
                    {
                        Vertex.r = ColorSegments.thick.r;
                        Vertex.g = ColorSegments.thick.g;
                        Vertex.b = ColorSegments.thick.b;
                    }
                    else if((Vertex.x >= VNE1StartX) && (Vertex.x <= VNE1EndX))
                    {
                        Vertex.r = ColorSegments.vne_1.r;
                        Vertex.g = ColorSegments.vne_1.g;
                        Vertex.b = ColorSegments.vne_1.b;
                    }
                    else if((Vertex.x >= VNEStartX) && (Vertex.x <= VNEEndX))
                    {
                        Vertex.r = ColorSegments.vne.r;
                        Vertex.g = ColorSegments.vne.g;
                        Vertex.b = ColorSegments.vne.b;
                    }
                    
                    Vertex.x = Map(Vertex.x, 0, 32000, -1, 1);
                    Vertex.y = Map(Vertex.y, -4000, 4000, -1, 1);
                    Vertex.z = Map(Vertex.z, 0, MaxZ, -1, 1);
                    PushVertex(VertexBuffer, Vertex);
                }
            }
        }
    }
}

internal void
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

internal bool
FloatLessThanEqual(float A, float B)
{
    return A <= B ? true : false;
}

internal bool
FloatGreaterThanEqual(float A, float B)
{
    return A >= B ? true : false;
}

internal std::vector<v3>
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
