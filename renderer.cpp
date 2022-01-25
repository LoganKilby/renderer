global_variable std::vector<texture_unit> TextureCache;

static int
CheckTextureCache(char *TexturePath)
{
    for(int i = 0; i < TextureCache.size(); ++i)
    {
        if(strcmp(TexturePath, TextureCache[i].Path) == 0)
        {
            return i;
        }
    }
    
    return -1;
}

#define not_newline_or_space(c) (c != '\n' && c != ' ')

internal f32
next_float(char *data, u32 *offset)
{
    u32 index = 0;
    u32 bytes_skipped = 0;
    u32 count = 0;
    char float_buffer[24] = {};
    
    while(data + index)
    {
        if(not_newline_or_space(*(data + index)))
        {
            while(not_newline_or_space(*(data + index)))
            {
                float_buffer[count++] = *(data + index);
                ++index;
            }
            
            break;
        }
        else
        {
            ++bytes_skipped;
            ++index;
        }
    }
    
    f32 result = atof(float_buffer);
    *offset = bytes_skipped + count;
    return result;
}

#define extract_f32(a, i) ((f32 *)&a)[i]

static bounds
Compute3DPointSetBounds(vertex_attributes *Attributes, u32 Count)
{
    bounds Result = {};
    
    if(Attributes)
    {
        Result.Min = { FLT_MAX, FLT_MAX, FLT_MAX };
        Result.Max = { FLT_MIN, FLT_MIN, FLT_MIN };
        
        v3 Point;
        for(u32 i = 0; i < Count; ++i)
        {
            Point = Attributes[i].Point;
            
            if(Point.x < Result.Min.x) Result.Min.x = Point.x;
            if(Point.y < Result.Min.y) Result.Min.y = Point.y;
            if(Point.z < Result.Min.z) Result.Min.z = Point.z;
            
            if(Point.x > Result.Max.x) Result.Max.x = Point.x;
            if(Point.y > Result.Max.y) Result.Max.y = Point.y;
            if(Point.z > Result.Max.z) Result.Max.z = Point.z;
        }
    }
    
    return Result;
}

static void
CreateTextureCoordinates(vertex_attributes *Attributes, u32 AttributeCount, bounds Bounds, f32 TextureWidth, f32 TextureHeight)
{
    f32 WidthRange = (TextureWidth / 20) * 1000;
    f32 HeightRange = (TextureHeight / 20) * 1000;
    f32 MaxZ = Bounds.Max.z;
    f32 MaxX = Bounds.Max.x;
    
    f32 zMap = 1 / MaxZ;
    f32 xMap = 1 / MaxX;
    
    // NOTE: 20 px per inch
    v2 TexCoords;
	for (u32 i = 0; i < AttributeCount; ++i)
	{
        
#if 0
        
        f32 ZRemap = Map(Attributes[i].Point.z, 0, MaxZ, 0, WidthRange);
        f32 XRemap = Map(Attributes[i].Point.x, 0, MaxX, 0, HeightRange);
        TexCoords.x = Map(ZRemap, 0, WidthRange, 0, 1);
        TexCoords.y = Map(XRemap, 0, HeightRange, 0, 1);
        
#else
        
        TexCoords.x = Attributes[i].Point.z * zMap;
        TexCoords.y = Attributes[i].Point.x * xMap;
        
#endif
        
        Attributes[i].TexCoords = TexCoords;
	}
}

internal point_render_set
AllocPointRenderSet(u32 MaxPoints)
{
    point_render_set Result = {};
    Result.Attributes = (vertex_attributes *)malloc(sizeof(vertex_attributes) * MaxPoints);
    Result.Count = 0;
    Result.MaxCount = MaxPoints;
    return Result;
}

internal point_render_set
PointsFromFile(char *Filename)
{
    u64 BytesRead;
    char *Data = ReadEntireFile(Filename, &BytesRead);
    
    u32 MaxPoints = 300000;
    point_render_set Result = AllocPointRenderSet(MaxPoints);
    
    vertex_attributes *Attrib;
    
    u32 offset;
    for(int i = 0; i < BytesRead; ++i)
    {
        Assert(Result.Count < Result.MaxCount);
        
        Attrib = Result.Attributes + Result.Count++;
        
        Attrib->Point.x = next_float(Data + i, &offset);
        i += offset;
        Attrib->Point.y = next_float(Data + i, &offset);
        i += offset;
        Attrib->Point.z = next_float(Data + i, &offset);
        i += offset;
    }
    
    free(Data);
    
    Result.Bounds = Compute3DPointSetBounds(Result.Attributes, Result.Count);
    CreateTextureCoordinates(Result.Attributes, Result.Count, Result.Bounds, 0, 0);
    
    glGenVertexArrays(1, &Result.VAO);
    glGenBuffers(1, &Result.VBO);
    glBindVertexArray(Result.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, Result.VBO);
    glBufferData(GL_ARRAY_BUFFER, Result.Count * sizeof(vertex_attributes), Result.Attributes, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_attributes), (void*)offsetof(vertex_attributes, Point));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_attributes), (void*)offsetof(vertex_attributes, Color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_attributes), (void*)offsetof(vertex_attributes, TexCoords));
    glBindVertexArray(0);
    
    return Result;
}

internal void
DrawPointSet(point_render_set PointSet)
{
    glBindVertexArray(PointSet.VAO);
    glDrawArrays(GL_POINTS, 0, PointSet.Count);
    glBindVertexArray(0);
}