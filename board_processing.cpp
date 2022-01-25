#include "board_processing.h"

static u32 *
LoadVisionImagePixels(VISION_IMAGE_STRUCT *Image)
{
    u32 *Result = (u32 *)malloc(sizeof(u32) * Image->width * Image->height);
    
    for(u32 Row = 0; Row < (u32)Image->height; ++Row)
    {
        for(u32 Col = 0; Col < (u32)Image->width; ++Col)
        {
            Result[Row * Image->width + Col] = Image->Pixels[Row][Col];
        }
    }
    
    return Result;
}

static void
PushAttributes(vertex_buffer *Buffer, v3 Point, v3 Color, v2 TexCoords)
{
    Assert(Buffer->Count < Buffer->Capacity);
    vertex_attributes Attributes = {Point, Color, TexCoords};
    u32 Index = Buffer->Count++;
    *(Buffer->Attributes + Index) = Attributes;
}

static void
PushAttributes(vertex_buffer *Buffer, v3 Point, v3 Color)
{
    Assert(Buffer->Count < Buffer->Capacity);
    vertex_attributes Attributes = {Point, Color, V2(0.0f)};
    u32 Index = Buffer->Count++;
    *(Buffer->Attributes + Index) = Attributes;
}

static void
PushAttributes(vertex_buffer *Buffer, v3 Point)
{
    Assert(Buffer->Count < Buffer->Capacity);
    vertex_attributes Attributes = {Point, V3(0.0f), V2(0.0f)};
    u32 Index = Buffer->Count++;
    *(Buffer->Attributes + Index) = Attributes;
}

static void
ClearVertexBuffer(vertex_buffer *Buffer)
{
    int CapacityInBytes = Buffer->Capacity * sizeof(vertex_attributes);
    memset(Buffer->Attributes, 0, CapacityInBytes);
    Buffer->Count = 0;
}

static vertex_buffer
CreateVertexBuffer(u32 ElementCount)
{
    vertex_buffer Result = {};
    Result.Capacity = ElementCount;
    Result.Attributes = (vertex_attributes *)malloc(ElementCount * sizeof(vertex_attributes));
    return Result;
}

static void
UploadBufferedVertices(vertex_buffer *Buffer)
{
    glBindBuffer(GL_ARRAY_BUFFER, Buffer->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, Buffer->Count * sizeof(vertex_attributes), Buffer->Attributes);
}

static void
V3UpdateBufferObject(u32 BufferObj, size_t SizeInBytes, v3 *Data)
{
    glBindBuffer(GL_ARRAY_BUFFER, BufferObj);
    glBufferSubData(GL_ARRAY_BUFFER, 0, SizeInBytes, Data);
}

static void *
ReadFile(char *Filename, u64 *SizeOut)
{
    FILE *FileHandle = fopen(Filename, "rb");
    fseek(FileHandle, 0L, SEEK_END);
    long int FileSize = ftell(FileHandle);
    rewind(FileHandle);
    
    *SizeOut = FileSize;
    
    void *Result = malloc(FileSize);
    fread((void *)Result, FileSize, 1, FileHandle);
    fclose(FileHandle);
    
    return Result;
}

static void
ReadFile(void *DataOut, char *Filename, u64 SizeExpected)
{
    FILE *FileHandle = fopen(Filename, "rb");
    
    if(FileHandle)
    {
        fseek(FileHandle, 0L, SEEK_END);
        long int FileSize = ftell(FileHandle);
        rewind(FileHandle);
        Assert(FileSize <= SizeExpected);
        fread(DataOut, FileSize, 1, FileHandle);
        fclose(FileHandle);
    }
    else
    {
        fprintf(stderr, "unable to open file %s\n", Filename);
    }
}

inline f32
Map(f32 Value, f32 A, f32 B, f32 C, f32 D)
{
    
    //f32 Result = (Value - A) / (B - A) * (D - C) + C;
    f32 Result = Value / B * D;
    Assert((Result >= C) && (Result <= D));
    
    return Result;
}

// Smooth data plan size: MAX_NUM_FT_OF_LIGHT_CURTAIN * NUM_LIGHT_CURTAIN_ELEMENTS_FT
// MAX_NUM_FT_OF_LIGHT_CURTAIN = MAX_LEN_BRD + 1
// NUM_LIGHT_CURTAIN_ELEMENTS_FT = 48

static void
ProcessProfileData(vertex_buffer *Result, SBoardData *BoardData, s32 Grade, board_color_segments ColorSegments)
{
    PROFILE_DATA_STN *Profiles = &BoardData->last_board_data.profile_data[0];
    u32 ProfileCount = ArrayCount(BoardData->last_board_data.profile_data);
    u32 SampleCount = ArrayCount(Profiles[0].thk_array);
    
    Assert(Result->Capacity >= (ProfileCount * SampleCount));
    Result->Count = 0;
    Result->ProfileCount = ProfileCount;
    
    f32 ZOffsetPerProfile = 45.0f; // TODO: This is currently kind of arbitrary
    // image width / profile count
    
    f32 XOffsetPerSample = (1.0f / 64.0f) * 1000; // in 1000th's of an inch
    
    u32 StartIndex = BoardData->last_board_data.implement_data.beg_ele;
    u32 EndIndex = BoardData->last_board_data.implement_data.end_ele + 1;
    
    f32 MinX = FLT_MAX;
    f32 MaxX = FLT_MIN;
    
    f32 MinY = FLT_MAX;
    f32 MaxY = FLT_MIN;
    
    f32 MinZ = FLT_MAX;
    f32 MaxZ = FLT_MIN;
    
    v3 Vertex;
    v3 Color;
    v2 TexCoords;
    f32 ProfileOffset;
    for(int ProfileIndex = StartIndex; 
        ProfileIndex < EndIndex; 
        ++ProfileIndex)
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
            
            f32 SampleY;
            f32 MaxSampleY;
            for(int SampleIndex = 0; 
                SampleIndex < ArrayCount(Profiles[ProfileIndex].thk_array); 
                ++SampleIndex)
            {
                SampleY = (f32)Profiles[ProfileIndex].thk_array[SampleIndex];
                //SampleY = (f32)Profiles[ProfileIndex].max_thick;
                if(SampleY)
                {
#if 0
                    MaxSampleY = (f32)Profiles[ProfileIndex].max_thick;
                    
                    if(SampleY > MaxSampleY)
                    {
                        SampleY = MaxSampleY;
                    }
#endif
                    //SampleY = Profiles[ProfileIndex].sawn_thick;
                    // NOTE: By incrementing distance for every valid y-value instead of for every
                    // possible y-value the result looks more reasonable
                    Vertex.x = (SampleIndex * XOffsetPerSample);// + LeadEdgeX;
                    //Vertex.y = LeadEdgeY + SampleY;
                    Vertex.y = SampleY;
                    Vertex.z = ProfileOffset;
                    
                    if(Vertex.x < MinX) MinX = Vertex.x;
                    if(Vertex.x > MaxX) MaxX = Vertex.x;
                    if(Vertex.y < MinY) MinY = Vertex.y;
                    if(Vertex.y > MaxY) MaxY = Vertex.y;
                    if(Vertex.z < MinZ) MinZ = Vertex.z;
                    if(Vertex.z > MaxZ) MaxZ = Vertex.z;
                    
                    // NOTE: grade_thick for each grade (represented by color) can be unique
                    // per-profile.
                    if((Vertex.x >= ScantStartX) && (Vertex.x <= ScantEndX))
                    {
                        Color = V3(ColorSegments.scant.r, ColorSegments.scant.g, ColorSegments.scant.b);
                    }
                    else if((Vertex.x >= ThickStartX) && (Vertex.x <= ThickEndX))
                    {
                        Color = V3(ColorSegments.thick.r, ColorSegments.thick.g, ColorSegments.thick.b);
                    }
                    else if((Vertex.x >= VNE1StartX) && (Vertex.x <= VNE1EndX))
                    {
                        Color = V3(ColorSegments.vne_1.r, ColorSegments.vne_1.g, ColorSegments.vne_1.b);
                    }
                    else //if((Vertex.x >= VNEStartX) && (Vertex.x <= VNEEndX))
                    {
                        Color = V3(ColorSegments.vne.r, ColorSegments.vne.g, ColorSegments.vne.b);
                    }
                    
                    //TexCoords.x = Map(Vertex.x, 0, 288000, -1, 1);
                    //TexCoords.y = Map(Vertex.y, 0, 22000, -1, 1);
                    PushAttributes(Result, Vertex);
                }
            }
        }
    }
}

// CLineLzStruct::LinLzPoint
struct image_point
{
    u16 x;
    s16 y;
    char brightness;
};

static vertex_buffer
ExtractJS50Raw(SBoardData *BoardData)
{
    H_BufferStruct *TopData = &BoardData->H_Top_Raw;
    H_BufferStruct *BottomData = &BoardData->H_Bot_Raw;
    
    u32 LaserCount = ArrayCount(TopData->H_Buffer);
    u32 SamplesPerLaser ArrayCount(TopData->H_Buffer[0]);
    u32 PointsPerLaser = ArrayCount(TopData->H_Buffer[0][0].p);
    u32 ElementCount = LaserCount * SamplesPerLaser * PointsPerLaser * 2;
    vertex_buffer Result = CreateVertexBuffer(ElementCount);
    
    f32 ZOffsetPerLaserLine = (1.0f / 64.0f) * 1000;
    f32 AccumulatingZOffset = 0.0f;
    
    f32 YOffsetPerLaserHead = 24000.0f;
    f32 AccumulatingYOffset = 0.0f;
    
    v3 Vertex;
    v3 Color = V3(1.0f);
    
    for(u32 LaserHeadIndex = 0; 
        LaserHeadIndex < LaserCount; 
        ++LaserHeadIndex)
    {
        for(u32 SampleIndex = 0; 
            SampleIndex < SamplesPerLaser; 
            ++SampleIndex)
        {
            LineLzStorageStruct *LaserLine = &TopData->H_Buffer[LaserHeadIndex][SampleIndex];
            // NOTE: If points exist, they are laid out from index 0 ... numPoints - 1
            int PointCount = LaserLine->numPoints;
            if(PointCount)
            {
                
                LineLzPoint Point;
                for(u32 PointIndex = 0; PointIndex < PointCount; ++PointIndex)
                {
                    Point = LaserLine->p[PointIndex];
                    
                    Vertex.x = Point.y + YOffsetPerLaserHead * LaserHeadIndex;
                    Vertex.y = Point.x;
                    Vertex.z = ZOffsetPerLaserLine * SampleIndex;
                    Color = V3((f32)Point.brightness / 255.0f);
                    
                    PushAttributes(&Result, Vertex, Color);
                }
            }
        }
    }
    
    for(u32 LaserHeadIndex = 0; 
        LaserHeadIndex < LaserCount; 
        ++LaserHeadIndex)
    {
        for(u32 SampleIndex = 0; 
            SampleIndex < SamplesPerLaser; 
            ++SampleIndex)
        {
            
            LineLzStorageStruct *LaserLine = &BottomData->H_Buffer[LaserHeadIndex][SampleIndex];
            // NOTE: If points exist, they are laid out from index 0 ... numPoints - 1
            int PointCount = LaserLine->numPoints;
            if(PointCount)
            {
                LineLzPoint Point;
                
                for(u32 PointIndex = 0; PointIndex < PointCount; ++PointIndex)
                {
                    Point = LaserLine->p[PointIndex];
                    
                    Vertex.x = Point.x;
                    Vertex.y = Point.y + YOffsetPerLaserHead * LaserHeadIndex;
                    Vertex.z = ZOffsetPerLaserLine * SampleIndex;
                    Color = V3((f32)Point.brightness / 255.0f);
                    
                    PushAttributes(&Result, Vertex, Color);
                }
                
                AccumulatingZOffset += ZOffsetPerLaserLine;
            }
        }
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
