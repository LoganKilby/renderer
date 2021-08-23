#include "renderer.h"

// TODO: Move this somewhere else. Maybe initialize it at startup
global_variable texture_cache GlobalTextureCache;

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
CacheTexture(texture_unit Texture)
{
    if(GlobalTextureCache.Count >= GlobalTextureCache.Size)
    {
        AssertMsgBreak("ERROR: Texture cache overflow");
    }
    
    GlobalTextureCache.Textures[GlobalTextureCache.Count] = Texture;
    GlobalTextureCache.Count++;
}

internal texture_unit
GetCachedTexture(int TextureIndex)
{
    Assert(TextureIndex >= 0 && TextureIndex <= GlobalTextureCache.Count);
    return GlobalTextureCache.Textures[TextureIndex];
}

internal std::vector<texture_unit>
LoadMaterialTextures(model *Model, aiMaterial *Material, aiTextureType Type, texture_map_enum MapType)
{
    std::vector<texture_unit> TextureUnits;
    TextureUnits.reserve(Material->GetTextureCount(Type));
    
    char Path[256] = {};
    strcpy(Path, Model->Directory);
    
    int TextureCount = Material->GetTextureCount(Type);
    for(unsigned int i = 0; i < Material->GetTextureCount(Type); ++i)
    {
        aiString TextureName;
        Material->GetTexture(Type, i, &TextureName);
        strcat(Path, TextureName.C_Str());
        
        int TextureIndex = CheckTextureCache(Path);
        if(TextureIndex >= 0)
        {
            TextureUnits.push_back(GetCachedTexture(TextureIndex));
        }
        else
        {
            
            texture_unit TextureResult = UploadTextureFromFile(Path);
            TextureResult.Type = MapType;
            TextureUnits.push_back(TextureResult);
            CacheTexture(TextureResult);
            
        }
        
        memset(Path + Model->DirectoryStrLen, 0, TextureName.length);
    }
    
    return TextureUnits;
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
    
    std::vector<texture_unit> Textures;
    if(Mesh->mMaterialIndex >= 0)
    {
        aiMaterial *Material = Scene->mMaterials[Mesh->mMaterialIndex];
        
        std::vector<texture_unit> DiffuseMaps = LoadMaterialTextures(Model, Material, aiTextureType_DIFFUSE, DIFFUSE_MAP);
        Result.Textures.insert(Result.Textures.end(), DiffuseMaps.begin(), DiffuseMaps.end());
        
        std::vector<texture_unit> SpecularMaps = LoadMaterialTextures(Model, Material, aiTextureType_SPECULAR, SPECULAR_MAP);
        Result.Textures.insert(Result.Textures.end(), SpecularMaps.begin(), SpecularMaps.end());
        
        // NOTE: For the Assmip .obj loader, normal maps must be identified by "map_Kn" in the .mtl file
        // otherwise Assimp won't load them. TODO: Get rid of Assimp
        std::vector<texture_unit> NormalMaps = LoadMaterialTextures(Model, Material, aiTextureType_NORMALS, NORMAL_MAP);
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
    
    texture_unit TexUnit;
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
}

internal void
DrawModel(unsigned int ProgramID, model Model)
{
    glUseProgram(ProgramID);
    for(int MeshIndex = 0; MeshIndex < Model.Meshes.size(); ++MeshIndex)
    {
        DrawMesh(ProgramID, Model.Meshes[MeshIndex]);
    }
}
