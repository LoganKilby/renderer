global_variable std::vector<texture_unit> TextureCache;

internal int
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

internal std::vector<texture_unit>
LoadMaterialTextures(model *Model, aiMaterial *Material, aiTextureType Type, texture_map_enum MapType)
{
    std::vector<texture_unit> TextureUnits;
    
    char Path[256] = {};
    strcpy(Path, Model->Directory);
    
    for(unsigned int i = 0; i < Material->GetTextureCount(Type); ++i)
    {
        aiString TextureName;
        Material->GetTexture(Type, i, &TextureName);
        strcat(Path, TextureName.C_Str());
        
        int TextureIndex = CheckTextureCache(Path);
        if(TextureIndex >= 0)
        {
            TextureUnits.push_back(TextureCache[TextureIndex]);
        }
        else
        {
            texture_unit TextureResult = LoadTexture(Path);
            TextureResult.Type = MapType;
            TextureUnits.push_back(TextureResult);
            TextureCache.push_back(TextureResult);
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
    std::vector<unsigned int> Indices;
    std::vector<texture_unit> Textures;
    
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
        
        Vertices.push_back(Vertex);
    }
    
    for(unsigned int i = 0; i < Mesh->mNumFaces; ++i)
    {
        aiFace Face = Mesh->mFaces[i];
        for(unsigned int j = 0; j < Face.mNumIndices; ++j)
        {
            Indices.push_back(Face.mIndices[j]);
        }
    }
    
    Result.IndexCount = Indices.size();
    
    if(Mesh->mMaterialIndex >= 0)
    {
        aiMaterial *Material = Scene->mMaterials[Mesh->mMaterialIndex];
        
        std::vector<texture_unit> DiffuseMaps = LoadMaterialTextures(Model, Material, aiTextureType_DIFFUSE, DIFFUSE_MAP);
        Result.Textures.insert(Result.Textures.end(), DiffuseMaps.begin(), DiffuseMaps.end());
        
        std::vector<texture_unit> SpecularMaps = LoadMaterialTextures(Model, Material, aiTextureType_SPECULAR, SPECULAR_MAP);
        Result.Textures.insert(Result.Textures.end(), SpecularMaps.begin(), SpecularMaps.end());
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
    
    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, TextureCoordinates));
    
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
    model Model = {};
    
    // TODO: Assimp is sooo slow
    fprintf(stderr, "INFO: Loading model %s...\n", Path);
    
    
    Assimp::Importer Importer;
    const aiScene *Scene = Importer.ReadFile(Path, aiProcess_Triangulate | aiProcess_FlipUVs);
    
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
    
    return Model;
}

internal void
DrawMesh(unsigned int Program, mesh Mesh)
{
#if 1
    glUseProgram(Program);
    unsigned int DiffuseTexUnit = 1;
    unsigned int SpecularTexUnit = 1;
    
    char DifString[] = "CrateMaterial.DiffuseMaps[ ]";
    char SpecString[] = "CrateMaterial.SpecularMaps[ ]";
    
    texture_unit TexUnit;
    unsigned int TextureCount = Mesh.Textures.size();
    Assert(TextureCount == 2);
    for(unsigned int i = 0; i < TextureCount; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        
        TexUnit = Mesh.Textures[i];
        if(TexUnit.Type == DIFFUSE_MAP)
        {
            DifString[sizeof(DifString) - 3] = 48 + DiffuseTexUnit++;
            SetUniform1i(Program, DifString, TexUnit.Id);
        }
        else if(TexUnit.Type == SPECULAR_MAP)
        {
            SpecString[sizeof(SpecString) - 3] = 48 + SpecularTexUnit++;
            SetUniform1i(Program, SpecString, TexUnit.Id);
        }
        else
        {
            Assert(0);
        }
        
        glBindTexture(GL_TEXTURE_2D, TexUnit.Id);
    }
    
    glActiveTexture(GL_TEXTURE0);
#endif
    
    glBindVertexArray(Mesh.VAO);
    glDrawElements(GL_TRIANGLES, Mesh.IndexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}