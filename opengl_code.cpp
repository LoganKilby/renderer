#include "opengl_code.h"

internal opengl_buffer
InitializeOpenglBuffer(float *Vertices, int VertexCount, unsigned int *Indices, int IndexCount)
{
    // The resulting draw call would either be glDrawArrays or glDrawElements
    // It may make more sense to cluster together each of these different types of draw calls,
    // I'm not sure. 
    
    opengl_buffer Result = {};
    
    Assert(Vertices);
    Assert(VertexCount);
    
    if(Indices)
    {
        Assert(IndexCount);
        
        glGenVertexArrays(1, &Result.VAO);
        glGenBuffers(1, &Result.VBO);
        glGenBuffers(1, &Result.EBO);
        
        glBindVertexArray(Result.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, Result.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices[0]) * VertexCount, Vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Result.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * IndexCount, Indices, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
        glEnableVertexAttribArray(0);
    }
    else
    {
        glGenVertexArrays(1, &Result.VAO);
        glGenBuffers(1, &Result.VBO);
        
        glBindVertexArray(Result.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, Result.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * VertexCount, Vertices, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
        glEnableVertexAttribArray(0);
    }
    
    glBindVertexArray(0);
    
    return Result;
}

internal opengl_shader
CompileShader(char *ShaderSource, shader_type Type)
{
    opengl_shader Result = {};
    char *ShaderString;
    switch(Type)
    {
        case COMPUTE: 
        Result.Type = GL_COMPUTE_SHADER; 
        ShaderString = "GL_COMPUTE_SHADER"; 
        break;
        case VERTEX: 
        Result.Type = GL_VERTEX_SHADER;
        ShaderString = "GL_VERTEX_SHADER";
        break;
        case TESS_CONTROL: 
        Result.Type =  GL_TESS_CONTROL_SHADER; 
        ShaderString = "GL_TESS_CONTROL_SHADER";
        break;
        case TESS_EVALUATION: 
        Result.Type = GL_TESS_EVALUATION_SHADER;
        ShaderString = "GL_TESS_EVALUATION_SHADER";
        break;
        case GEOMETRY:
        Result.Type = GL_GEOMETRY_SHADER;
        ShaderString = "GL_GEOMETRY_SHADER";
        break;
        case FRAGMENT:
        Result.Type = GL_FRAGMENT_SHADER;
        ShaderString = "GL_FRAGMENT_SHADER";
        break;
        default:
        {
            printf("WARNING: Undefined shader type passed to CompileShader\n");
        }
    }
    
    Result.Id = glCreateShader(Result.Type);
    glShaderSource(Result.Id, 1, &ShaderSource, NULL);
    glCompileShader(Result.Id);
    
    int CompilationStatus;
    char ErrorInfo[512] = {};
    glGetShaderiv(Result.Id, GL_COMPILE_STATUS, &CompilationStatus);
    if(!CompilationStatus)
    {
        char *Divider = "-------------------------------------";
        glGetShaderInfoLog(Result.Id, 512, NULL, ErrorInfo);
        fprintf(stderr, "ERROR: Shader compilation (%s)\n", ShaderString);
        fprintf(stderr, "%s", ErrorInfo);
        fprintf(stderr, "\n%s\n", Divider);
        fprintf(stderr, "\n%s\n", ShaderSource);
        fprintf(stderr, "\n%s\n\n", Divider);
    }
    
    return Result;
}

internal opengl_shader_program
CreateShaderProgram(opengl_shader VertexShader, opengl_shader FragmentShader)
{
    opengl_shader_program Result;
    Result.Id = glCreateProgram();
    
    glAttachShader(Result.Id, VertexShader.Id);
    glAttachShader(Result.Id, FragmentShader.Id);
    glLinkProgram(Result.Id);
    
    // Shaders will only be flagged for deletion while attached to a program and not immediately deleted.
    glDeleteShader(VertexShader.Id); 
    glDeleteShader(FragmentShader.Id);
    
    return Result;
}

internal void
OutputOpenglError(int ErrorCode)
{
    char *Message;
    
    switch(ErrorCode)
    {
        case GL_INVALID_ENUM:
        {
            Message = "GL_INVALID_ENUM";
        } break;
        case GL_INVALID_VALUE:
        {
            Message = "GL_INVALID_VALUE";
        } break;
        case GL_INVALID_OPERATION:
        {
            Message = "GL_INVALID_OPERATION";
        } break;
        case GL_STACK_OVERFLOW:
        {
            Message = "GL_STACK_OVERFLOW";
        } break;
        case GL_STACK_UNDERFLOW:
        {
            Message = "GL_STACK_UNDERFLOW";
        } break;
        case GL_OUT_OF_MEMORY:
        {
            Message = "GL_OUT_OF_MEMORY";
        } break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
        {
            Message = "GL_INVALID_FRAMEBUFFER_OPERATION";
        } break;
        case GL_CONTEXT_LOST:
        {
            Message = "GL_CONTEXT_LOST";
        } break;
    }
    
    fprintf(stderr, "WARNING: OpenGL error (%s)\n", Message);
}

internal bool
CheckErrorCode()
{
    bool Result = false;
    
    int ErrorCode = glGetError();
    if(ErrorCode != GL_NO_ERROR)
    {
        OutputOpenglError(ErrorCode);
        Result = true;
    }
    
    return Result;
}

internal void
OutputErrorQueue()
{
    GLenum ErrorCode = glGetError();
    while(ErrorCode != GL_NO_ERROR)
    {
        OutputOpenglError(ErrorCode);
        ErrorCode = glGetError();
    }
}

internal void
SetShaderMaterial(GLuint Program,
                  char *UniformName,
                  material Material)
{
    glUseProgram(Program);
    
    int MaxUniformLength, MaxAttributeLength;
    glGetProgramiv(Program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &MaxUniformLength);
    glGetProgramiv(Program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &MaxAttributeLength);
    
    int MaxStringLength = MaxUniformLength + MaxAttributeLength;
    char *Location = (char *)malloc(MaxStringLength);
    memset(Location, 0, MaxStringLength);
    char *ClearOffset = &Location[0] + strlen(UniformName);
    
    strcpy(Location, UniformName);
    
    strcat(Location, ".diffuseMap");
    glUniform1i(glGetUniformLocation(Program, Location), Material.DiffuseMapTexUnit);
    memset(ClearOffset, 0, sizeof(".diffuseMap"));
    
    strcat(Location, ".specularMap");
    glUniform1i(glGetUniformLocation(Program, Location), Material.SpecularMapTexUnit);
    memset(ClearOffset, 0, sizeof(".specularMap"));
    
    strcat(Location, ".shininess");
    glUniform1f(glGetUniformLocation(Program, Location), Material.Shininess);
    
    free(Location);
}

internal void
SetShaderLightSource(GLuint Program,
                     char *UniformName,
                     light_source Light)
{
    glUseProgram(Program);
    
    int MaxUniformLength, MaxAttributeLength;
    glGetProgramiv(Program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &MaxUniformLength);
    glGetProgramiv(Program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &MaxAttributeLength);
    
    int MaxStringLength = MaxUniformLength + MaxAttributeLength;
    char *Location = (char *)malloc(MaxStringLength);
    memset(Location, 0, MaxStringLength);
    char *ClearOffset = &Location[0] + strlen(UniformName);
    
    strcpy(Location, UniformName);
    
    strcat(Location, ".position");
    glUniform3fv(glGetUniformLocation(Program, Location), 1, &Light.Position[0]);
    memset(ClearOffset, 0, sizeof(".position"));
    
    strcat(Location, ".ambient");
    glUniform3fv(glGetUniformLocation(Program, Location), 1, &Light.Ambient[0]);
    memset(ClearOffset, 0, sizeof(".ambient"));
    
    strcat(Location, ".diffuse");
    glUniform3fv(glGetUniformLocation(Program, Location), 1, &Light.Diffuse[0]);
    memset(ClearOffset, 0, sizeof(".diffuse"));
    
    strcat(Location, ".specular");
    glUniform3fv(glGetUniformLocation(Program, Location), 1, &Light.Specular[0]);
    
    free(Location);
}

internal texture
LoadTexture(char *Filename)
{
    texture Result = {};
    Result.Data = stbi_load(Filename, &Result.Width, &Result.Height, &Result.ColorChannels, 0);
    if(Result.Data)
    {
        GLenum PixelFormat;
        switch(Result.ColorChannels)
        {
            // NOTE: Specular maps loaded as PNGs should be RGBA (maybe other formats work too)
            case 4: PixelFormat = GL_RGBA; break;
            case 3: PixelFormat = GL_RGB; break;
            default: 
            {
                printf("WARNING: An unsupported image format attempted to load. Aborting...\n");
                stbi_image_free(Result.Data);
                Result.Data = 0;
                return Result;
            }
        }
        
        glGenTextures(1, &Result.TextureId);
        glBindTexture(GL_TEXTURE_2D, Result.TextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, PixelFormat, Result.Width, Result.Height, 0, PixelFormat, GL_UNSIGNED_BYTE, Result.Data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        printf("INFO: Texture %d loaded.\n", Result.TextureId);
    }
    else
    {
        printf("WARNING: Failed to load image from at %s\n", Filename);
    }
    
    return Result;
}