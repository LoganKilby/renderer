#include "renderer.h"

// TODO: Move this somewhere else. Maybe initialize it at startup
global_variable texture_cache GlobalTextureCache;

internal void
EnqueueDrawCommand(draw_buffer *Buffer, draw_command Command)
{
    Assert(Buffer->Count < Buffer->MaxCount);
    Buffer->Queue[Buffer->Count] = Command;
    ++Buffer->Count;
}

internal void
ClearDrawBuffer(draw_buffer *Buffer)
{
    memset(Buffer->Queue, 0, sizeof(draw_command) * Buffer->Count);
    Buffer->Count = 0;
}

internal int
CheckTextureCache(char *TexturePath)
{
    for(int i = 0; i < GlobalTextureCache.Count; ++i)
    {
        if(strcmp(TexturePath, GlobalTextureCache.Textures[i].Path) == 0)
        {
            return i;
        }
    }
    
    return -1;
}

internal void
CacheTexture(texture Texture)
{
    if(CheckTextureCache(Texture.Path) == -1)
    {
        if(GlobalTextureCache.Count >= ArrayCount(GlobalTextureCache.Textures))
            AssertMsgBreak("ERROR: Texture cache overflow");
        
        GlobalTextureCache.Textures[GlobalTextureCache.Count] = Texture;
        GlobalTextureCache.Count++;
    }
}

internal texture
GetCachedTexture(int TextureIndex)
{
    Assert(TextureIndex >= 0 && TextureIndex <= GlobalTextureCache.Count);
    return GlobalTextureCache.Textures[TextureIndex];
}

internal texture
LoadTextureToLinear(char *Filename)
{
    texture Result = {};
    Result.ColorSpace = SRGB;
    
    int TextureIndex = CheckTextureCache(Filename);
    if(TextureIndex >= 0)
    {
        return GetCachedTexture(TextureIndex);
    }
    
    unsigned char *Data = stbi_load(Filename, &Result.Width, &Result.Height, &Result.ColorChannels, 0);
    if(Data)
    {
        strcpy(Result.Path, Filename);
        
        GLenum CurrentPixelFormat;
        GLenum StorePixelFormat;
        GLenum TexParam;
        switch(Result.ColorChannels)
        {
            // NOTE: Specular maps loaded as PNGs should be RGBA (maybe other formats work too)
            case 4: 
            {
                StorePixelFormat = GL_SRGB_ALPHA;
                CurrentPixelFormat = GL_RGBA;
                TexParam = GL_CLAMP_TO_EDGE;
            } break;
            case 3:
            {
                StorePixelFormat = GL_SRGB;
                CurrentPixelFormat = GL_RGB;
                TexParam = GL_REPEAT;
            } break;
            default: 
            {
                printf("WARNING: Image %s has an unsupported internal pixel format. Aborting texture creation...\n", Filename);
                // TODO: Return a default texture
                return Result;
            }
        }
        
        glGenTextures(1, &Result.Id);
        glBindTexture(GL_TEXTURE_2D, Result.Id);
        glTexImage2D(GL_TEXTURE_2D, 0, StorePixelFormat, Result.Width, Result.Height, 0, CurrentPixelFormat, GL_UNSIGNED_BYTE, Data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TexParam);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TexParam);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(Data);
        printf("INFO: Texture loaded. (Path: %s, Texture ID: %d)\n", Filename, Result.Id);
        CacheTexture(Result);
    }
    else
    {
        printf("WARNING: Failed to load image %s\n", Filename);
    }
    
    return Result;
}

internal texture
LoadTexture(char *Filename)
{
    texture Result = {};
    
    int CachedIndex = CheckTextureCache(Filename);
    if(CachedIndex >= 0)
    {
        return GetCachedTexture(CachedIndex);
    }
    
    unsigned char *Data = stbi_load(Filename, &Result.Width, &Result.Height, &Result.ColorChannels, 0);
    if(Data)
    {
        strcpy(Result.Path, Filename);
        
        GLenum PixelFormat;
        GLenum TexParam;
        switch(Result.ColorChannels)
        {
            // NOTE: Specular maps loaded as PNGs should be RGBA (maybe other formats work too)
            case 4: 
            {
                PixelFormat = GL_RGBA; 
                TexParam = GL_CLAMP_TO_EDGE;
            } break;
            case 3:
            {
                PixelFormat = GL_RGB;
                TexParam = GL_REPEAT;
            } break;
            default: 
            {
                printf("WARNING: Image %s has an unsupported internal pixel format. Aborting texture creation...\n", Filename);
                // TODO: Return a default texture
                return Result;
            }
        }
        
        glGenTextures(1, &Result.Id);
        glBindTexture(GL_TEXTURE_2D, Result.Id);
        glTexImage2D(GL_TEXTURE_2D, 0, PixelFormat, Result.Width, Result.Height, 0, PixelFormat, GL_UNSIGNED_BYTE, Data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TexParam);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TexParam);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(Data);
        printf("INFO: Texture loaded. (Path: %s, Texture ID: %d)\n", Filename, Result.Id);
        CacheTexture(Result);
    }
    else
    {
        printf("WARNING: Failed to load image %s\n", Filename);
    }
    
    return Result;
}

internal std::vector<texture>
LoadMaterialTextures(model *Model, aiMaterial *Material, aiTextureType Type, texture_map_type MapType)
{
    std::vector<texture> Textures;
    Textures.reserve(Material->GetTextureCount(Type));
    
    char Path[256] = {};
    strcpy(Path, Model->Directory);
    
    int TextureCount = Material->GetTextureCount(Type);
    for(unsigned int i = 0; i < Material->GetTextureCount(Type); ++i)
    {
        aiString TextureName;
        Material->GetTexture(Type, i, &TextureName);
        strcat(Path, TextureName.C_Str());
        
        texture TextureResult;
        if(MapType == DIFFUSE_MAP)
        {
            TextureResult = LoadTextureToLinear(Path);
        }
        else
        {
            TextureResult = LoadTexture(Path);
        }
        
        TextureResult.Type = MapType;
        Textures.push_back(TextureResult);
        memset(Path + Model->DirectoryStrLen, 0, TextureName.length);
    }
    
    return Textures;
}

internal mesh
ProcessMesh(model *Model, aiMesh *Mesh, const aiScene *Scene)
{
    mesh Result = {};
    
    std::vector<vertex> Vertices;
    Vertices.reserve(Mesh->mNumVertices);
    
    vertex Vertex;
    for(unsigned int i = 0; i < Mesh->mNumVertices; ++i)
    {
        Vertex.Position.x = Mesh->mVertices[i].x;
        Vertex.Position.y = Mesh->mVertices[i].y;
        Vertex.Position.z = Mesh->mVertices[i].z;
        
        Vertex.Normal.x = Mesh->mNormals[i].x;
        Vertex.Normal.y = Mesh->mNormals[i].y;
        Vertex.Normal.z = Mesh->mNormals[i].z;
        
        if(Mesh->mTextureCoords[0])
        {
            Vertex.TextureCoordinates.x = Mesh->mTextureCoords[0][i].x;
            Vertex.TextureCoordinates.y = Mesh->mTextureCoords[0][i].y;
        }
        else
        {
            Vertex.TextureCoordinates = glm::vec2(0.0f);
        }
        
        Vertex.Tangent.x = Mesh->mTangents[i].x;
        Vertex.Tangent.y = Mesh->mTangents[i].y;
        Vertex.Tangent.z = Mesh->mTangents[i].z;
        
        Vertices.push_back(Vertex);
    }
    
    // NOTE: Assimp doesn't tell me the total number of indices
    std::vector<unsigned int> Indices;
    Indices.reserve(Mesh->mNumFaces * 3);
    for(unsigned int i = 0; i < Mesh->mNumFaces; ++i)
    {
        aiFace Face = Mesh->mFaces[i];
        for(unsigned int j = 0; j < Face.mNumIndices; ++j)
        {
            Indices.push_back(Face.mIndices[j]);
        }
    }
    
    Result.IndexCount = Indices.size();
    
    std::vector<texture> Textures;
    if(Mesh->mMaterialIndex >= 0)
    {
        aiMaterial *Material = Scene->mMaterials[Mesh->mMaterialIndex];
        
        std::vector<texture> DiffuseMaps = LoadMaterialTextures(Model, Material, aiTextureType_DIFFUSE, DIFFUSE_MAP);
        Result.Textures.insert(Result.Textures.end(), DiffuseMaps.begin(), DiffuseMaps.end());
        
        std::vector<texture> SpecularMaps = LoadMaterialTextures(Model, Material, aiTextureType_SPECULAR, SPECULAR_MAP);
        Result.Textures.insert(Result.Textures.end(), SpecularMaps.begin(), SpecularMaps.end());
        
        // NOTE: For the Assmip .obj loader, normal maps must be identified by "map_Kn" in the .mtl file
        // otherwise Assimp won't load them. TODO: Get rid of Assimp
        std::vector<texture> NormalMaps = LoadMaterialTextures(Model, Material, aiTextureType_NORMALS, NORMAL_MAP);
        Result.Textures.insert(Result.Textures.end(), NormalMaps.begin(), NormalMaps.end());
    }
    
    Assert(Indices.size() > 0);
    
    glGenVertexArrays(1, &Result.VAO);
    glGenBuffers(1, &Result.VBO);
    glGenBuffers(1, &Result.EBO);
    
    glBindVertexArray(Result.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, Result.VBO);
    
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(vertex), &Vertices[0], GL_STATIC_DRAW);  
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Result.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), 
                 &Indices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, Normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, TextureCoordinates));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, Tangent));
    
    glBindVertexArray(0);
    
    return Result;
}

internal void
ProcessNode(model *Model, aiNode *Node, const aiScene *Scene)
{
    for(unsigned int i = 0; i < Node->mNumMeshes; ++i)
    {
        aiMesh *Mesh = Scene->mMeshes[Node->mMeshes[i]];
        
        Model->Meshes.push_back(ProcessMesh(Model, Mesh, Scene));
    }
    
    for(unsigned int i = 0; i < Node->mNumChildren; ++i)
    {
        ProcessNode(Model, Node->mChildren[i], Scene);
    }
}

internal model
LoadModel(char *Path)
{
    char TempPath[256] = {};
    strcpy(TempPath, Path);
    
    model Model = {};
    
    QPC_StartCounter();
    Assimp::Importer Importer;
    const aiScene *Scene = Importer.ReadFile(Path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    printf("Assimp file read performance (%s): %Lf ms\n", Path, QPC_EndCounter() / 1000.0l);
    
    if(!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
    {
        printf("ERROR: (Assimp) %s\n", Importer.GetErrorString());
        return Model;
    }
    
    int PathIndex = 0;
    while(*Path)
    {
        Model.Directory[PathIndex] = *Path;
        
        PathIndex++;
        Path++;
    }
    
    while(Model.Directory[PathIndex] != '/')
    {
        Model.Directory[PathIndex] = 0;
        PathIndex--;
    }
    Model.DirectoryStrLen = PathIndex + 1;
    
    ProcessNode(&Model, Scene->mRootNode, Scene);
    
    printf("INFO: Model loaded (%s)\n", TempPath);
    return Model;
}

internal void
DrawMesh(unsigned int Program, mesh Mesh)
{
    // NOTE: Defined in shadow_frag.c (8/22/2021)
    // DiffuseUnit: 0
    // NormalUnit: 1
    // SpecularUnit: 2
    // DepthUnit: 3
    
    texture TexUnit;
    int TextureCount = Mesh.Textures.size();
    for(unsigned int i = 0; i < TextureCount; ++i)
    {
        TexUnit = Mesh.Textures[i];
        
        if(TexUnit.Type == DIFFUSE_MAP)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, TexUnit.Id);
        }
        else if(TexUnit.Type == SPECULAR_MAP)
        {
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, TexUnit.Id);
        }
        else if(TexUnit.Type == NORMAL_MAP)
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, TexUnit.Id);
        }
    }
    
    glBindVertexArray(Mesh.VAO);
    glDrawElements(GL_TRIANGLES, Mesh.IndexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
}

internal void
DrawModel(model Model, unsigned int ProgramID)
{
    glUseProgram(ProgramID);
    for(int MeshIndex = 0; MeshIndex < Model.Meshes.size(); ++MeshIndex)
    {
        DrawMesh(ProgramID, Model.Meshes[MeshIndex]);
    }
}

internal model
LoadObjModel(char *PathToDotObjFile)
{
    QPC_StartCounter();
    
    char *Ext = Extension(PathToDotObjFile);
    // TODO: Return a debug model so the program doesn't just crash
    AssertMsgBreakEx(strcmp(Ext, ".obj"), "ERROR: Invalid obj file (%s)", PathToDotObjFile);
    
    
    fast_obj_mesh *Mesh = fast_obj_read(PathToDotObjFile);
    // TODO: Return a debug model so the program doesn't just crash
    AssertMsgBreakEx(Mesh, "ERROR: Unable to open file (%s)", PathToDotObjFile);
    
    model Result;
    vertex FaceVertexBuffer[5]; // NOTE: I'm assuming there's no more than 5 vertices per face
    vertex Vertex;
    for(unsigned int GroupIndex = 0; GroupIndex < Mesh->group_count; ++GroupIndex)
    {
        fastObjGroup Group = Mesh->groups[GroupIndex];
        
        std::vector<mesh> Meshes;
        std::vector<vertex> Vertices;
        std::vector<unsigned int> Indices;
        mesh NewMesh;
        
        int idx = 0;
        for(unsigned int FaceIndex = 0; FaceIndex < Group.face_count; FaceIndex++)
        {
            unsigned int FaceVertCount = Mesh->face_vertices[FaceIndex + Group.face_offset];
            Assert(FaceVertCount <= ArrayCount(FaceVertexBuffer));
            
            // TODO: Will need to rewrite tangent calculation if > 3 vertices per face
            Assert(FaceVertCount == 3);
            
            for(unsigned int VertIndex = 0; VertIndex < FaceVertCount; VertIndex++)
            {
                fastObjIndex MeshIndex = Mesh->indices[Group.index_offset + idx];
                
                Vertex.Position.x = Mesh->positions[3 * MeshIndex.p + 0];
                Vertex.Position.y = Mesh->positions[3 * MeshIndex.p + 1];
                Vertex.Position.z = Mesh->positions[3 * MeshIndex.p + 2];
                
                Vertex.TextureCoordinates.x = Mesh->texcoords[2 * MeshIndex.t + 0];
                Vertex.TextureCoordinates.y = Mesh->texcoords[2 * MeshIndex.t + 1];
                
                Vertex.Normal.x = Mesh->normals[3 * MeshIndex.n + 0];
                Vertex.Normal.y = Mesh->normals[3 * MeshIndex.n + 1];
                Vertex.Normal.z = Mesh->normals[3 * MeshIndex.n + 2];
                
                Indices.push_back(MeshIndex.p);
                Indices.push_back(MeshIndex.n);
                Indices.push_back(MeshIndex.t);
                
                FaceVertexBuffer[VertIndex] = Vertex;
                idx++;
            }
            
            // Tangent calculation
            glm::vec3 Edge1 = FaceVertexBuffer[1].Position - FaceVertexBuffer[0].Position;
            glm::vec3 Edge2 = FaceVertexBuffer[2].Position - FaceVertexBuffer[0].Position;
            glm::vec2 DeltaUV1 = FaceVertexBuffer[1].TextureCoordinates - 
                FaceVertexBuffer[0].TextureCoordinates;
            glm::vec2 DeltaUV2 = FaceVertexBuffer[2].TextureCoordinates - 
                FaceVertexBuffer[0].TextureCoordinates;
            
            float f = 1.0f / (DeltaUV1.x * DeltaUV2.y - DeltaUV2.x * DeltaUV1.y);
            glm::vec3 Tangent(f * (DeltaUV2.y * Edge1.x - DeltaUV1.y * Edge2.x),
                              f * (DeltaUV2.y * Edge1.y - DeltaUV1.y * Edge2.y),
                              f * (DeltaUV2.y * Edge1.z - DeltaUV1.y * Edge2.z));
            
            for(int VertIndex = 0; VertIndex < FaceVertCount; ++VertIndex)
            {
                FaceVertexBuffer[VertIndex].Tangent = Tangent;
                Vertices.push_back(FaceVertexBuffer[VertIndex]);
            }
        }
        
        NewMesh.IndexCount = Indices.size();
        // TODO: Add per face texture support
        
        Assert(Mesh->material_count == 1);
        if(Mesh->materials[0].map_Kd.path)
        {
            NewMesh.Textures.push_back(LoadTextureToLinear(Mesh->materials[0].map_Kd.path));
        }
        
        if(Mesh->materials[0].map_Ks.path)
        {
            NewMesh.Textures.push_back(LoadTexture(Mesh->materials[0].map_Ks.path));
        }
        
        if(Mesh->materials[0].map_bump.path)
        {
            NewMesh.Textures.push_back(LoadTexture(Mesh->materials[0].map_bump.path));
        }
        
        glGenVertexArrays(1, &NewMesh.VAO);
        glGenBuffers(1, &NewMesh.VBO);
        glGenBuffers(1, &NewMesh.EBO);
        
        glBindVertexArray(NewMesh.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, NewMesh.VBO);
        
        glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(vertex), &Vertices[0], GL_STATIC_DRAW);  
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NewMesh.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), 
                     &Indices[0], GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, Normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, TextureCoordinates));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, Tangent));
        
        glBindVertexArray(0);
        
        Result.Meshes.push_back(NewMesh);
    }
    
    
    QPC_EndCounterPrint("INFO: Obj Load/Process complete");
    
    return Result;
}

internal void
RenderDrawBuffer(draw_buffer *Buffer, unsigned int Shader, glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix)
{
    
    glm::mat4 ModelMatrix;
    glm::mat4 MVP;
    for(draw_command *Command = Buffer->Queue;
        Command < Buffer->Queue + Buffer->Count;
        ++Command)
    {
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, Command->Position);
        ModelMatrix *= mat4_EncodeEulerAnglesZYX(Command->Rotation);
        ModelMatrix = glm::scale(ModelMatrix, Command->Scale);
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        SetUniformMatrix4fv(Command->Shader, "mvp", MVP);
        
        // check for textures?
        // shader uniforms?
        
        switch(Command->Primitive)
        {
            case CUBE:
            {
                RenderCube();
            } break;
            
            case QUAD:
            {
                RenderQuad();
            } break;
            
            case LINE_SEGMENT:
            {
                
            } break;
        }
    }
}