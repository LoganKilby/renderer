#include "opengl_code.h"
#include "renderer.h"

internal GLuint
LoadAndCompileShader(char *Filename, GLenum ShaderType)
{
    GLuint ResultID = 0;
    char *ShaderTypeString;
    switch(ShaderType)
    {
        case GL_VERTEX_SHADER: 
        ShaderTypeString = "GL_VERTEX_SHADER";
        break;
        case GL_FRAGMENT_SHADER:
        ShaderTypeString = "GL_FRAGMENT_SHADER";
        break;
        default:
        {
            printf("WARNING: No Support for the shader type passed to CompileShader with filename %s\n", Filename);
        }
    }
    
    char *ShaderSource = ReadEntireFileToString(Filename);
    
    if(ShaderSource)
    {
        ResultID = glCreateShader(ShaderType);
        glShaderSource(ResultID, 1, &ShaderSource, NULL);
        glCompileShader(ResultID);
        
        int CompilationStatus;
        glGetShaderiv(ResultID, GL_COMPILE_STATUS, &CompilationStatus);
        if(CompilationStatus == GL_FALSE)
        {
            int LogLength;
            glGetShaderiv(ResultID, GL_INFO_LOG_LENGTH, &LogLength);
            char *LogBuffer = (char *)malloc(sizeof(LogLength));
            memset(LogBuffer, 0, LogLength);
            glGetShaderInfoLog(ResultID, LogLength, NULL, LogBuffer);
            
            fprintf(stderr, "\nERROR: Shader compilation error (%s): %s\n", ShaderTypeString, Filename);
            fprintf(stderr, "%s", LogBuffer);
            fprintf(stderr, "\n%s\n\n", ShaderSource);
            
            free(LogBuffer);
        }
        
        fprintf(stderr, "SHADER: %s compiled\n", Filename);
        free(ShaderSource);
    }
    
    return ResultID;
}

// TODO: Probably want to take an array of compiled shaders
// TODO: Probably don't need a program struct
internal opengl_shader_program
CreateShaderProgram(GLuint VertexShaderID, GLuint FragmentShaderID)
{
    opengl_shader_program Result;
    Result.Id = glCreateProgram();
    
    glAttachShader(Result.Id, VertexShaderID);
    glAttachShader(Result.Id, FragmentShaderID);
    glLinkProgram(Result.Id);
    
    // Shaders will only be flagged for deletion while attached to a program and not immediately deleted.
    glDeleteShader(VertexShaderID); 
    glDeleteShader(FragmentShaderID);
    
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
    int MaxUniformLength, MaxAttributeLength;
    glGetProgramiv(Program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &MaxUniformLength);
    glGetProgramiv(Program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &MaxAttributeLength);
    
    int MaxStringLength = MaxUniformLength + MaxAttributeLength;
    char *Location = (char *)malloc(MaxStringLength); // TODO: Stack allocate the buffer
    memset(Location, 0, MaxStringLength);
    char *ClearOffset = &Location[0] + strlen(UniformName);
    
    strcpy(Location, UniformName);
    
    strcat(Location, ".DiffuseMaps[0]");
    SetUniform1i(Program, Location, Material.DiffuseMapTexUnit);
    memset(ClearOffset, 0, sizeof(".DiffuseMap[0]"));
    
    strcat(Location, ".SpecularMaps[0]");
    SetUniform1i(Program, Location, Material.SpecularMapTexUnit);
    memset(ClearOffset, 0, sizeof(".SpecularMap[0]"));
    
    strcat(Location, ".Shininess");
    SetUniform1f(Program, Location, Material.Shininess);
    
    free(Location);
}

internal void
SetShaderSpotLight(GLuint Program, char *StructName, spot_light Light)
{
    int MaxUniformLength, MaxAttributeLength;
    glGetProgramiv(Program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &MaxUniformLength);
    glGetProgramiv(Program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &MaxAttributeLength);
    
    int MinBufferSize = MaxUniformLength + MaxAttributeLength;
    char *UniformName = (char *)malloc(MinBufferSize); // TODO: Stack allocate the buffer
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
    memset(ClearOffset, 0, sizeof(".FarRadius"));
    
    strcat(UniformName, ".Constant");
    SetUniform1f(Program, UniformName, Light.Constant);
    memset(ClearOffset, 0, sizeof(".Constant"));
    
    strcat(UniformName, ".Linear");
    SetUniform1f(Program, UniformName, Light.Linear);
    memset(ClearOffset, 0, sizeof(".Linear"));
    
    strcat(UniformName, ".Quadratic");
    SetUniform1f(Program, UniformName, Light.Quadratic);
    memset(ClearOffset, 0, sizeof(".Quadratic"));
    
    free(UniformName);
}

int GetMaxUniformLength(unsigned int Program)
{
    int Result;
    glGetProgramiv(Program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &Result);
    return Result;
}

int GetMaxAttributeLength(unsigned int Program)
{
    int Result;
    glGetProgramiv(Program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &Result);
    return Result;
}

internal void
SetShaderPointLight(GLuint Program, char *StructName, point_light Light)
{
    int MaxUniformLength, MaxAttributeLength;
    glGetProgramiv(Program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &MaxUniformLength);
    glGetProgramiv(Program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &MaxAttributeLength);
    
    int MinBufferSize = MaxUniformLength + MaxAttributeLength;
    char *UniformName = (char *)malloc(MinBufferSize); // TODO: Stack allocate the buffer
    memset(UniformName, 0, MinBufferSize);
    char *ClearOffset = &UniformName[0] + strlen(StructName);
    
    strcpy(UniformName, StructName);
    
    strcat(UniformName, ".Position");
    SetUniform3fv(Program, UniformName, Light.Position);
    memset(ClearOffset, 0, sizeof(".Position"));
    
    strcat(UniformName, ".Ambient");
    SetUniform3fv(Program, UniformName, Light.Ambient);
    memset(ClearOffset, 0, sizeof(".Ambient"));
    
    strcat(UniformName, ".Diffuse");
    SetUniform3fv(Program, UniformName, Light.Diffuse);
    memset(ClearOffset, 0, sizeof(".Diffuse"));
    
    strcat(UniformName, ".Specular");
    SetUniform3fv(Program, UniformName, Light.Specular);
    memset(ClearOffset, 0, sizeof(".Specular"));
    
    strcat(UniformName, ".Constant");
    SetUniform1f(Program, UniformName, Light.Constant);
    memset(ClearOffset, 0, sizeof(".Constant"));
    
    strcat(UniformName, ".Linear");
    SetUniform1f(Program, UniformName, Light.Linear);
    memset(ClearOffset, 0, sizeof(".Linear"));
    
    strcat(UniformName, ".Quadratic");
    SetUniform1f(Program, UniformName, Light.Quadratic);
    memset(ClearOffset, 0, sizeof(".Quadratic"));
    
    free(UniformName);
}

internal void
SetShaderDirectionalLight(GLuint Program, char *StructName, directional_light Light)
{
    int MaxUniformLength, MaxAttributeLength;
    glGetProgramiv(Program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &MaxUniformLength);
    glGetProgramiv(Program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &MaxAttributeLength);
    
    int MinBufferSize = MaxUniformLength + MaxAttributeLength;
    char *UniformName = (char *)malloc(MinBufferSize); // TODO: Stack allocate the buffer
    memset(UniformName, 0, MinBufferSize);
    char *ClearOffset = &UniformName[0] + strlen(StructName);
    
    strcpy(UniformName, StructName);
    
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
    
    free(UniformName);
}

internal texture_unit
LoadTexture(char *Filename)
{
    texture_unit Result = {};
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
                // TODO: Reminder: Maybe setting these params here is a bad idea. All pngs probably
                // don't need to be clamped to the edge
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
        printf("INFO: Texture %s loaded. (Texture ID: %d)\n", Filename, Result.Id);
    }
    else
    {
        printf("WARNING: Failed to load image %s\n", Filename);
    }
    
    return Result;
}

// NOTE: Saving the uniform locations is better than looking them up every time.
// Some uniforms are set every frame. So maybe in the future I can just assert that
// the uniform location passed as an argument is valid

internal void
SetUniform3fv(int Program, char *Name, glm::vec3 Data)
{
    glUseProgram(Program);
    GLint UniformLocation = glGetUniformLocation(Program, Name);
    AssertUniformLoc(UniformLocation);
    glUniform3fv(UniformLocation, 1, &Data[0]);
}

internal void
SetUniform1f(int Program, char *Name, float Data)
{
    glUseProgram(Program);
    GLint UniformLocation = glGetUniformLocation(Program, Name);
    AssertUniformLoc(UniformLocation);
    glUniform1f(UniformLocation, Data);
}

internal void
SetUniformMatrix4fv(int Program, char *Name, glm::mat4 Data)
{
    glUseProgram(Program);
    GLint UniformLocation = glGetUniformLocation(Program, Name);
    AssertUniformLoc(UniformLocation);
    glUniformMatrix4fv(UniformLocation, 1, GL_FALSE, &Data[0][0]);
}

internal void
SetUniform1i(int Program, char *Name, int Data)
{
    glUseProgram(Program);
    GLint UniformLocation = glGetUniformLocation(Program, Name);
    AssertUniformLoc(UniformLocation);
    glUniform1i(UniformLocation, Data);
}

internal void DebugPrintUniforms(GLuint ProgramID)
{
    int UniformCount;
    glGetProgramiv(ProgramID, GL_ACTIVE_UNIFORMS, &UniformCount);
    
    char Buffer[100] = {};
    
    GLsizei Length;
    int Size;
    GLenum Type;
    
    fprintf(stderr, "DEBUG INFO: Uniform Dump :: Program ID (%d)\n", ProgramID);
    for(unsigned int UniformID = 0; UniformID < UniformCount; ++UniformID)
    {
        glGetActiveUniform(ProgramID, UniformID, 100, &Length, &Size, &Type, &Buffer[0]);
        fprintf(stderr, "\t%s\n", Buffer);
        memset(Buffer, 0, Length);
    }
}
