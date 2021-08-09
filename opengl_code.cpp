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
        case GL_GEOMETRY_SHADER:
        ShaderTypeString = "GL_GEOMETRY_SHADER";
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
            char *LogBuffer = (char *)malloc(LogLength);
            memset(LogBuffer, 0, LogLength);
            glGetShaderInfoLog(ResultID, LogLength, NULL, LogBuffer);
            
            fprintf(stderr, "\nERROR: Shader compilation error (%s): %s\n", ShaderTypeString, Filename);
            fprintf(stderr, "%s", LogBuffer);
            fprintf(stderr, "\n%s\n\n", ShaderSource);
            
            free(LogBuffer);
        }
        
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
    
    int LinkStatus;
    glGetProgramiv(Result.Id, GL_LINK_STATUS, &LinkStatus);
    if(LinkStatus == GL_FALSE)
    {
        fprintf(stderr, "ERROR: Shader link error (Program ID: %d)\n", Result.Id);
        int LogLength;
        glGetProgramiv(Result.Id, GL_INFO_LOG_LENGTH, &LogLength);
        if(LogLength)
        {
            char *LogBuffer = (char *)malloc(LogLength);
            memset(LogBuffer, 0, LogLength);
            glGetProgramInfoLog(Result.Id, LogLength, NULL, LogBuffer);
            fprintf(stderr, "%s\n\n", LogBuffer);
            free(LogBuffer);
        }
    }
    
    // NOTE: glDeleteShader sets a flag for deletion. A shader won't be deleted while attached to a program.
    
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
        Assert(0);
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

internal void
SetUniformMatrix3fv(int Program, char *Name, glm::mat3 Data)
{
    glUseProgram(Program);
    GLint UniformLocation = glGetUniformLocation(Program, Name);
    AssertUniformLoc(UniformLocation);
    glUniformMatrix3fv(UniformLocation, 1, GL_FALSE, &Data[0][0]);
}

internal void 
DebugPrintUniforms(GLuint ProgramID, char *ProgramName)
{
    int UniformCount;
    glGetProgramiv(ProgramID, GL_ACTIVE_UNIFORMS, &UniformCount);
    
    char Buffer[100] = {};
    
    GLsizei Length;
    int Size;
    GLenum Type;
    
    fprintf(stderr, "DEBUG INFO: Uniform Dump :: Program(\"%s\", %d)\n", ProgramName, ProgramID);
    for(unsigned int UniformID = 0; UniformID < UniformCount; ++UniformID)
    {
        glGetActiveUniform(ProgramID, UniformID, 100, &Length, &Size, &Type, &Buffer[0]);
        fprintf(stderr, "\t%s\n", Buffer);
        memset(Buffer, 0, Length);
    }
}

internal unsigned int
LoadCubemap(char *Right, char *Left, char *Top, char *Bottom, char *Back, char *Front)
{
    char *Paths[6] = 
    {
        Right, Left, 
        Top, Bottom, 
        Back, Front
    };
    
    unsigned int Result = 0;
    glGenTextures(1, &Result);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Result);
    
    int Width, Height, ColorChannels;
    unsigned char *Data;
    GLenum PixelFormat;
    
    // NOTE: Since we're looking from inside the cubemap, the textures will be inverted.
    // Letting the texture load inverted creates the effect I want. I'm not sure that this is
    // robust, but it at least works for my first cubemap (skybox). 
    stbi_set_flip_vertically_on_load(false);
    for(int PathIndex = 0; PathIndex < 6; ++PathIndex)
    {
        Data = stbi_load(Paths[PathIndex], &Width, &Height, &ColorChannels, 0);
        if(Data)
        {
            switch(ColorChannels)
            {
                // NOTE: Specular maps loaded as PNGs should be RGBA (maybe other formats work too)
                case 4: 
                {
                    PixelFormat = GL_RGBA; 
                } break;
                case 3:
                {
                    PixelFormat = GL_RGB;
                } break;
                default: 
                {
                    printf("WARNING: Image %s has an unsupported internal pixel format. Aborting texture creation...\n", Paths[PathIndex]);
                    // TODO: Apply a default texture
                    return Result;
                }
            }
            
            // NOTE: The cubemap enums enumerate from POSITIVE_X to NEGATIVE_Z
            // Order starting with POSITIVE_X:
            // Right, Left, Top, Bottom, Back, Front
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + PathIndex, 0, 
                         PixelFormat, Width, Height, 0, 
                         PixelFormat, GL_UNSIGNED_BYTE, Data);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            
            printf("INFO: Cubemap texture %s loaded. (Texture ID: %d)\n",
                   Paths[PathIndex], Result);
            
            stbi_image_free(Data);
        }
        else
        {
            // TODO: Apply default texture
            printf("WARNING: Unable to load cubemap texture (%s).\n", Paths[PathIndex]);
        }
    }
    
    stbi_set_flip_vertically_on_load(true);
    
    return Result;
}

internal offscreen_buffer
CreateOffscreenBuffer(int WindowWidth, int WindowHeight)
{
    offscreen_buffer Result;
    
    glGenFramebuffers(1, &Result.FrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, Result.FrameBuffer);
    
    glGenTextures(1, &Result.ColorBuffer);
    glBindTexture(GL_TEXTURE_2D, Result.ColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WindowWidth, WindowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Result.ColorBuffer, 0);
    
    glGenRenderbuffers(1, &Result.RenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, Result.RenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WindowWidth, WindowHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, Result.RenderBuffer);
    
    GLenum FramebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(FramebufferStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        char *Message;
        switch(FramebufferStatus)
        {
            case GL_FRAMEBUFFER_UNDEFINED: 
            Message = "GL_FRAMEBUFFER_UNDEFINED"; break;
            
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: 
            Message = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
            
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            Message = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
            
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            Message = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"; break;
            
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            Message = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"; break;
            
            case GL_FRAMEBUFFER_UNSUPPORTED:
            Message = "GL_FRAMEBUFFER_UNSUPPORTED"; break;
            
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            Message = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break;
            
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            Message = "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"; break;
            
            default: Message = "Check OpenGL error queue";
        }
        
        fprintf(stderr, "ERROR: Off-screen buffer creation failure (%s)\n", Message);
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    unsigned int VBO; // NOTE: I decided to not save this because I don't need it anywhere atm.
    glGenVertexArrays(1, &Result.VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(Result.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(DefaultScreenVertices), &DefaultScreenVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
    
    return Result;
}

internal void
DrawOffscreenBuffer(unsigned int PostEffectsShader, offscreen_buffer OffScreen)
{
    AssertFrameBuf(OffScreen.FrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(PostEffectsShader);
    glBindVertexArray(OffScreen.VAO);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, OffScreen.ColorBuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_FRAMEBUFFER_SRGB);
}