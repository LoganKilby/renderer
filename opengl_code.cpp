#include "opengl_code.h"

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
    glGetShaderiv(Result.Id, GL_COMPILE_STATUS, &CompilationStatus);
    if(!CompilationStatus)
    {
        int LogLength;
        glGetShaderiv(Result.Id, GL_INFO_LOG_LENGTH, &LogLength);
        char *LogBuffer = (char *)malloc(sizeof(LogLength));
        
        glGetShaderInfoLog(Result.Id, LogLength, NULL, LogBuffer);
        
        fprintf(stderr, "\nERROR: Shader compilation error (%s)\n", ShaderString);
        fprintf(stderr, "%s", LogBuffer);
        fprintf(stderr, "\n%s\n\n", ShaderSource);
    }
    
    return Result;
}

internal opengl_shader
LoadAndCompileShader(char *Filename, GLenum ShaderType)
{
    opengl_shader Result = {};
    char *ShaderTypeString;
    switch(ShaderType)
    {
        case GL_VERTEX_SHADER: 
        ShaderTypeString = "GL_VERTEX_SHADER";
        break;
        case TESS_CONTROL: 
        ShaderTypeString = "GL_TESS_CONTROL_SHADER";
        break;
        case TESS_EVALUATION: 
        ShaderTypeString = "GL_TESS_EVALUATION_SHADER";
        break;
        case GEOMETRY:
        ShaderTypeString = "GL_GEOMETRY_SHADER";
        break;
        case FRAGMENT:
        ShaderTypeString = "GL_FRAGMENT_SHADER";
        break;
        default:
        {
            printf("WARNING: Undefined shader type passed to CompileShader with filename %s\n", Filename);
        }
    }
    
    char *ShaderSource = ReadEntireFileToString(Filename);
    
    Result.Id = glCreateShader(Result.Type);
    glShaderSource(Result.Id, 1, &ShaderSource, NULL);
    glCompileShader(Result.Id);
    
    int CompilationStatus;
    glGetShaderiv(Result.Id, GL_COMPILE_STATUS, &CompilationStatus);
    if(CompilationStatus == GL_FALSE)
    {
        int LogLength;
        glGetShaderiv(Result.Id, GL_INFO_LOG_LENGTH, &LogLength);
        char *LogBuffer = (char *)malloc(sizeof(LogLength));
        
        glGetShaderInfoLog(Result.Id, LogLength, NULL, LogBuffer);
        
        fprintf(stderr, "\nERROR: Shader compilation error (%s)\n", ShaderTypeString);
        fprintf(stderr, "%s", LogBuffer);
        fprintf(stderr, "\n%s\n\n", ShaderSource);
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
SetShaderSpotLight(GLuint Program, char *StructName, spot_light Light)
{
    glUseProgram(Program);
    
    int MaxUniformLength, MaxAttributeLength;
    glGetProgramiv(Program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &MaxUniformLength);
    glGetProgramiv(Program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &MaxAttributeLength);
    int MinBufferSize = MaxUniformLength + MaxAttributeLength;
    char *UniformName = (char *)malloc(MinBufferSize);
    memset(UniformName, 0, MinBufferSize);
    
    char *ClearOffset = &UniformName[0] + strlen(StructName);
    
    strcpy(UniformName, StructName);
    
    strcat(UniformName, ".Position");
    SetUniform3fv(Program, UniformName, Light.Position);
    memset(ClearOffset, 0, sizeof(".Position"));
    
    strcat(UniformName, ".Direction");
    SetUniform3fv(Program, UniformName, Light.Direction);
    memset(ClearOffset, 0, sizeof(".Direction"));
    
    strcat(UniformName, ".Ambient");
    SetUniform3fv(Program, UniformName, Light.Ambient);
    memset(ClearOffset, 0, sizeof(".Ambient"));
    
    strcat(UniformName, ".Diffuse");
    SetUniform3fv(Program, UniformName, Light.Diffuse);
    memset(ClearOffset, 0, sizeof(".Diffuse"));
    
    strcat(UniformName, ".Specular");
    SetUniform3fv(Program, UniformName, Light.Specular);
    memset(ClearOffset, 0, sizeof(".Specular"));
    
    strcat(UniformName, ".NearRadius");
    SetUniform1f(Program, UniformName, Light.NearRadius);
    memset(ClearOffset, 0, sizeof(".NearRadius"));
    
    strcat(UniformName, ".FarRadius");
    SetUniform1f(Program, UniformName, Light.FarRadius);
    
    free(UniformName);
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

// NOTE: Saving the uniform locations is better than looking them up every time.
// Some uniforms are set every frame. So maybe in the future I can just assert that
// the uniform location passed as an argument is valid

internal void
SetUniform3fv(int Program, char *Name, glm::vec3 Data)
{
    GLint UniformLocation = glGetUniformLocation(Program, Name);
    AssertUniformLoc(UniformLocation);
    glUniform3fv(UniformLocation, 1, &Data[0]);
}

internal void
SetUniform1f(int Program, char *Name, float Data)
{
    GLint UniformLocation = glGetUniformLocation(Program, Name);
    AssertUniformLoc(UniformLocation);
    glUniform1f(UniformLocation, Data);
}

internal void
SetUniformMatrix4fv(int Program, char *Name, glm::mat4 Data)
{
    GLint UniformLocation = glGetUniformLocation(Program, Name);
    AssertUniformLoc(UniformLocation);
    glUniformMatrix4fv(UniformLocation, 1, GL_FALSE, &Data[0][0]);
}