struct mtl
{
    float Ns; // Specular highlights
    glm::vec3 Ka; // Ambient color
    glm::vec3 Kd; // Diffuse color
    glm::vec3 Ks; // Specular color
    glm::vec3 Ke; // Emissive color
    float Ni; // Optical density (index of refraction)
    float d; // Dissolve. 1 is completely opaque, 0 is completely transparent
    int illum; // Lightning model (see link above)
    char *map_Kd; // Diffuse map (color texture)
    char *map_Bump; // Normal map
    char *map_Ks; // Specular map
    char *map_Ke; // Emissive map
};

enum obj_token_type
{
    HEADER,
    VERTEX,
    MESH,
    SMOOTH_GROUP,
    FACE,
    TEXTURE_COORDINATE,
    VERTEX_NORMAL,
    USE_MATERIAL,
    MATERIAL_LIB,
    
    SMOOTH_GROUP_INFO,
    INVALID_TOKEN
};


char *ObjReserved[] = 
{
    "#", // Header
    "v", // Vertex
    "o", // Mesh
    "s", // Smooth group, followed by off/on; on will be an integer
    "f", // Face (vertex indices for EBO)
    "vt", // Texture coordinates
    "vn", // Surface normal
    "usemtl", // Use the following material
    "mtllib", // Material library name decl
    
};

struct token
{
    char *LexemeAddress;
    int LexemeLength;
    obj_token_type Type = INVALID_TOKEN;
};

internal int
BytesInCommon(char *String1, char *String2)
{
    int Result = 0;
    
    while((String1 && String2) && (*String1 == *String2))
    {
        Result++;
        String1++;
        String2++;
    }
    
    return Result;
}

internal token *
LexObjFile(char *Bytes, unsigned long ByteCount, int *TokensCreated, int *MeshesInModel, int *MaxVertices, int *MaxFaces)
{
    token *Tokens = (token *)malloc(sizeof(token) * ByteCount); // Way more memory than required
    memset(Tokens, 0, sizeof(token) * ByteCount);
    int TokenCount = 0;
    
    token CurrentToken;
    int LexemeLength;
    for(int ByteIndex = 0; ByteIndex < ByteCount; ++ByteIndex)
    {
        CurrentToken = {};
        
        CurrentToken.LexemeAddress = &Bytes[ByteIndex];
        
        if(*CurrentToken.LexemeAddress == '#') // Header; Scan to next line.
        {
            CurrentToken.Type = HEADER;
            LexemeLength = 1;
            ++ByteIndex;
            while((ByteIndex < ByteCount) && (Bytes[ByteIndex] != '\n'))
            {
                ++LexemeLength;
                ++ByteIndex;
            }
        }
        else
        {
            // Scan to white space
            LexemeLength = 1;
            ++ByteIndex;
            while((ByteIndex < ByteCount) && 
                  ((Bytes[ByteIndex] != ' ') &&
                   (Bytes[ByteIndex] != '\n')))
            {
                ++LexemeLength;
                ++ByteIndex;
            }
        }
        
        CurrentToken.LexemeLength = LexemeLength;
        Tokens[TokenCount++] = CurrentToken;
    }
    
    
    int MeshCount = 0;
    int MaxVertexCount = 0;
    int MaxFaceCount = 0;
    
    int TempVertexCount = 0;
    int TempFaceCount = 0;
    
    // Assign token types
    for(int TokenIndex = 0; TokenIndex < TokenCount; ++TokenIndex)
    {
        switch(Tokens[TokenIndex].LexemeLength)
        {
            case 1:
            {
                // "f" appears more often
                if(BytesInCommon(Tokens[TokenIndex].LexemeAddress, "f") == 1)
                {
                    Tokens[TokenIndex].Type = FACE;
                    ++TempFaceCount;
                }
                else if(BytesInCommon(Tokens[TokenIndex].LexemeAddress, "v") == 1)
                {
                    Tokens[TokenIndex].Type = VERTEX;
                    ++TempVertexCount;
                }
                else if(BytesInCommon(Tokens[TokenIndex].LexemeAddress, "s") == 1)
                {
                    Tokens[TokenIndex].Type = SMOOTH_GROUP;
                }
                else if(BytesInCommon(Tokens[TokenIndex].LexemeAddress, "o") == 1)
                {
                    Tokens[TokenIndex].Type = MESH;
                    ++MeshCount;
                    
                    if(TempVertexCount > MaxVertexCount)
                    {
                        MaxVertexCount = TempVertexCount;
                    }
                    
                    if(TempFaceCount > MaxFaceCount)
                    {
                        MaxFaceCount = TempFaceCount;
                    }
                    
                    TempVertexCount = 0;
                    TempFaceCount = 0;
                }
            } break;
            case 2:
            {
                if(BytesInCommon(Tokens[TokenIndex].LexemeAddress, "vt") == 2)
                {
                    Tokens[TokenIndex].Type = TEXTURE_COORDINATE;
                }
                else if(BytesInCommon(Tokens[TokenIndex].LexemeAddress, "vn") == 2)
                {
                    Tokens[TokenIndex].Type = VERTEX_NORMAL;
                }
            } break;
            case 6:
            {
                // usemtl will appear more often
                if(BytesInCommon(Tokens[TokenIndex].LexemeAddress, "usemtl") == 6)
                {
                    Tokens[TokenIndex].Type = USE_MATERIAL;
                }
                else if (BytesInCommon(Tokens[TokenIndex].LexemeAddress, "mtllib") == 6)
                {
                    Tokens[TokenIndex].Type = MATERIAL_LIB;
                }
            }
        }
    }
    
    *MeshesInModel = MeshCount;
    *TokensCreated = TokenCount;
    *MaxVertices = MaxVertexCount;
    *MaxFaces = MaxFaceCount;
    return Tokens;
}

internal model
LoadModel(char *Filename)
{
    Assert(0); // NOTE: Stopped work on the obj loader. Verify what has changed
    
    model Result = {};
    
    unsigned long BytesRead;
    char *FileContents = ReadEntireFile(Filename, &BytesRead);
    
    if(!FileContents)
    {
        return Result;
    }
    
    int TokenCount, MeshCount, MaxVertexCount, MaxFaceCount;
    token *Tokens = LexObjFile(FileContents, BytesRead, &TokenCount, &MeshCount, &MaxVertexCount, &MaxFaceCount);
    
    //Result.Meshes  = (mesh *)malloc(sizeof(mesh) * MeshCount);
    //memset(Result.Meshes, 0, sizeof(mesh) * MeshCount);
    
    /*
    struct vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TextureCoordinates;
    };
    */
    
    vertex *Vertices = (vertex *)malloc(sizeof(vertex) * MaxVertexCount);
    memset(Vertices, 0.0, sizeof(vertex) * MaxVertexCount);
    
    unsigned int *Indices = (unsigned int *)malloc(sizeof(unsigned int) * MaxFaceCount * 3);
    memset(Indices, 0, sizeof(unsigned int) * MaxFaceCount);
    
    int TokenIndex = 0;
    Assert(Tokens[TokenIndex++].Type == HEADER);
    Assert(Tokens[TokenIndex++].Type == HEADER);
    Assert(Tokens[TokenIndex++].Type == MATERIAL_LIB);
    TokenIndex++; // TODO: This is the file name. Process the material lib
    
    
    for(int MeshIndex = 0; MeshIndex < MeshCount; ++MeshIndex)
    {
        // Obj Parsing Order
        // - Mesh
        // - Vertices
        // - TextureCoordinates
        // - Normals
        // - Use material (scene name)
        // - Indices (check for smooth group - two consecutive tokens)
        
        Assert(Tokens[TokenIndex++].Type == MESH);
        ++TokenIndex; // This is the mesh name. I don't care about that
        Assert(Tokens[TokenIndex].Type == VERTEX);
        
        size_t PositionOffset = 0;
        size_t NormalOffset = offsetof(vertex, Normal);
        size_t TextureCoordOffset = offsetof(vertex, TextureCoordinates);
        
        int RunningVertexCount;
        
        int VertexOffset = 0;
        while(Tokens[TokenIndex].Type == VERTEX)
        {
            (Vertices + VertexOffset)->Position = glm::vec3(1.0f);
            ++VertexOffset;
            TokenIndex += 4;
        }
        
        RunningVertexCount = VertexOffset;
        
        int RunningTexCoordCount;
        
        VertexOffset = 0;
        Assert(Tokens[TokenIndex].Type == TEXTURE_COORDINATE);
        while(Tokens[TokenIndex].Type == TEXTURE_COORDINATE)
        {
            (Vertices + VertexOffset)->TextureCoordinates =
                glm::vec2(2.0f);
            ++VertexOffset;
            TokenIndex += 3;
        }
        
        RunningTexCoordCount = VertexOffset;
        Assert(RunningTexCoordCount == RunningVertexCount);
        
        int RunningNormalCount;
        
        VertexOffset = 0;
        Assert(Tokens[TokenIndex].Type == VERTEX_NORMAL);
        while(Tokens[TokenIndex].Type == VERTEX_NORMAL)
        {
            (Vertices + VertexOffset)->Normal = glm::vec3(3.0f);
            ++VertexOffset;
            TokenIndex += 4;
        }
        
        RunningNormalCount = VertexOffset;
        //Assert(RunningNormalCount == RunningVertexCount); 
        
        Assert(Tokens[TokenIndex++].Type == USE_MATERIAL);
        // NOTE: The current token is the name of the material to use located int
        // <material_lib>.mtl. Example obj file: "Scene_-_Root"
        ++TokenIndex;
        
        if(Tokens[TokenIndex].Type == SMOOTH_GROUP)
        {
            TokenIndex += 2; // NOTE: Idk what to do with smooth groups yet
        }
        
        Assert(Tokens[TokenIndex].Type == FACE);
        int IndexOffset = 0;
        while(Tokens[TokenIndex].Type == FACE)
        {
            // vertex_index | texture_index | normal_index
            // I think I'll just use the vertex indices?
            Indices[IndexOffset++] = 1;
            Indices[IndexOffset++] = 1;
            Indices[IndexOffset++] = 1;
            TokenIndex += 4;
        }
        
        if(Tokens[TokenIndex].Type == SMOOTH_GROUP)
        {
            // NOTE: Smooth group info change, continue processing faces
            TokenIndex+= 2;
            while(Tokens[TokenIndex].Type == FACE)
            {
                Indices[IndexOffset++] = 1;
                Indices[IndexOffset++] = 1;
                Indices[IndexOffset++] = 1;
                TokenIndex += 4;
            }
        }
        
        int RunningIndexCount = IndexOffset;
        // TODO: LOOK AT INDEX OFFSET. IS IT EQUAL TO THE # OF VERTICES?
        
        Assert(Tokens[TokenIndex].Type == MESH); // NOTE: Next mesh is ready to process;
        
        // SetupMesh(Vertices, RunningVertexCount, Indices, RunningIndexCount, &Mesh[MeshIndex]);
    }
    
    return Result;
}
