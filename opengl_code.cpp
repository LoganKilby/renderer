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
CreateShaderProgram(GLuint VertexShaderID, GLuint FragmentShaderID, GLuint GeometryShaderID)
{
    opengl_shader_program Result;
    Result.Id = glCreateProgram();
    
    glAttachShader(Result.Id, VertexShaderID);
    glAttachShader(Result.Id, FragmentShaderID);
    if(GeometryShaderID)
    {
        glAttachShader(Result.Id, GeometryShaderID);
    }
    
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
OpenGL_PrintErrorCode(int ErrorCode)
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
        OpenGL_PrintErrorCode(ErrorCode);
        Result = true;
    }
    
    return Result;
}

internal void
OpenGL_OutputErrorQueue()
{
    GLenum ErrorCode = glGetError();
    while(ErrorCode != GL_NO_ERROR)
    {
        OpenGL_PrintErrorCode(ErrorCode);
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
SetUniform4fv(int Program, char *Name, glm::vec3 Data)
{
    glUseProgram(Program);
    GLint UniformLocation = glGetUniformLocation(Program, Name);
    AssertUniformLoc(UniformLocation);
    glUniform4fv(UniformLocation, 1, &Data[0]);
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
DebugPrintUniforms(GLuint ProgramID, char *ProgramName = "")
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
OpenGL_LoadCubemap(char *Right, char *Left, char *Top, char *Bottom, char *Back, char *Front)
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

internal render_target
HDR_CreateRenderTarget(int ScreenWidth, int ScreenHeight)
{
    render_target Result = {};
    Result.Type = HDR;
    glGenFramebuffers(1, &Result.FrameBuffer);
    
    glGenTextures(1, &Result.ColorBuffer);
    glBindTexture(GL_TEXTURE_2D, Result.ColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, ScreenWidth, ScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glGenRenderbuffers(1, &Result.RenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, Result.RenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, ScreenWidth, ScreenHeight);
    
    glBindFramebuffer(GL_FRAMEBUFFER, Result.RenderBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Result.ColorBuffer, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, Result.RenderBuffer);
    
    GLenum FramebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(FramebufferStatus != GL_FRAMEBUFFER_COMPLETE)
        AssertMsgBreak("OpenGL Error: Render buffer creation error");
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return Result;
}

internal render_target
PFX_CreateRenderTarget(int ScreenWidth, int ScreenHeight)
{
    render_target Result;
    Result.Type = PFX;
    
    glGenFramebuffers(1, &Result.FrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, Result.FrameBuffer);
    
    glGenTextures(1, &Result.ColorBuffer);
    glBindTexture(GL_TEXTURE_2D, Result.ColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ScreenWidth, ScreenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Result.ColorBuffer, 0);
    
    glGenRenderbuffers(1, &Result.RenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, Result.RenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ScreenWidth, ScreenHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, Result.RenderBuffer);
    
    GLenum FramebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    AssertMsgBreakEx(FramebufferStatus == GL_FRAMEBUFFER_COMPLETE,
                     "ERROR: (OpenGL) Render buffer creation error: %s",
                     GetFramebufferStatusMsg(FramebufferStatus));
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return Result;
}

internal shadow_map
CreateShadowMap()
{
    shadow_map Result;
    
    glGenFramebuffers(1, &Result.FrameBuffer);
    
    // Depth map resolution
    Result.DepthBufferWidth = 1024;
    Result.DepthBufferHeight = 1024;
    
    glGenTextures(1, &Result.DepthBuffer);
    glBindTexture(GL_TEXTURE_2D, Result.DepthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Result.DepthBufferWidth, Result.DepthBufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glBindFramebuffer(GL_FRAMEBUFFER, Result.FrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Result.DepthBuffer, 0);
    glDrawBuffer(GL_NONE); // NOTE: To specifiy that we're not using a color buffer
    glReadBuffer(GL_NONE); // NOTE: To specifiy that we're not using a color buffer
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return Result;
}

internal shadow_map
CreateShadowCubeMap()
{
    shadow_map Result;
    Result.DepthBufferWidth = 1024;
    Result.DepthBufferHeight = 1024;
    
    glGenFramebuffers(1, &Result.FrameBuffer);
    
    glGenTextures(1, &Result.DepthBuffer);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Result.DepthBuffer);
    for(unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 
                     Result.DepthBufferWidth, Result.DepthBufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    glBindFramebuffer(GL_FRAMEBUFFER, Result.FrameBuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, Result.DepthBuffer, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    return Result;
}

// NOTE: Example of calculating tangent and bitanent vectors
internal void 
RenderQuad()
{
    static unsigned int quadVAO, quadVBO = 0;
    if (quadVAO == 0)
    {
        // Quad without tangent/bitangent
        float quadVertices[] =
        {
            -1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f, // 1
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // 2
            1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f, // 3
            
            -1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f, // 1
            1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f, // 3
            1.0f, 1.0f, 0.0f,    0.0f, 0.0f, 1.0f,  1.0f, 1.0f  // 4
        };
        
        //                              ^
        // These values represent those |
        glm::vec3 pos1(-1.0,  1.0, 0.0); // unique vertices
        glm::vec3 pos2(-1.0, -1.0, 0.0);
        glm::vec3 pos3( 1.0, -1.0, 0.0);
        glm::vec3 pos4( 1.0,  1.0, 0.0);
        glm::vec2 uv1(0.0, 1.0); // texture coordinates
        glm::vec2 uv2(0.0, 0.0);
        glm::vec2 uv3(1.0, 0.0);
        glm::vec2 uv4(1.0, 1.0);
        glm::vec3 nm(0.0, 0.0, 1.0); // normal
        
        // I assume this is how we would calculate tangent/bitangent space vectors when processing
        // model files like .obj
        glm::vec3 tangent1, tangent2;
        glm::vec3 bitangent1, bitangent2;
        
        // First triangle
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;
        
        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        
        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        
        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        
        // Second triangle
        edge1 = pos3 - pos1;
        edge2 = pos4 - pos1;
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;
        
        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        
        tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        
        bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        
        float quadVerticesEx[] =
        {
            -1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 
            tangent1.x, tangent1.y, tangent1.z,  bitangent1.x, bitangent1.y, bitangent1.z, // 1
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
            tangent1.x, tangent1.y, tangent1.z,  bitangent1.x, bitangent1.y, bitangent1.z, // 2
            1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
            tangent1.x, tangent1.y, tangent1.z,  bitangent1.x, bitangent1.y, bitangent1.z, // 3
            
            -1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
            tangent2.x, tangent2.y, tangent2.z,  bitangent2.x, bitangent2.y, bitangent2.z, // 1
            1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
            tangent2.x, tangent2.y, tangent2.z,  bitangent2.x, bitangent2.y, bitangent2.z, // 3
            1.0f, 1.0f, 0.0f,    0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
            tangent2.x, tangent2.y, tangent2.z,  bitangent2.x, bitangent2.y, bitangent2.z  // 4
        };
        
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerticesEx), &quadVerticesEx, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)(8 * sizeof(float)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)(11 * sizeof(float)));
    }
    
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void DebugRenderCube()
{
    static unsigned int CubeVAO, CubeVBO = 0;
    if (CubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
            1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
            1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
            1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
            1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
            1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
            1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
            1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        
        glGenVertexArrays(1, &CubeVAO);
        glGenBuffers(1, &CubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(CubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    
    // render Cube
    glBindVertexArray(CubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

internal unsigned int
CreateScreenRenderQuad()
{
    float ScreenRenderQuad[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ScreenRenderQuad), &ScreenRenderQuad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    
    return VAO;
}

internal char *
GetFramebufferStatusMsg(GLenum FramebufferStatusCode)
{
    char *Message;
    switch(FramebufferStatusCode)
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
        
        case GL_FRAMEBUFFER_COMPLETE:
        Message = "GL_FRAMEBUFFER_COMPLETE"; break;
        
        default:
        Message = "Unkown frame buffer status code.";
    }
    
    return Message;
}

internal void
PFX_ResizeTarget(render_target Target, int NewWidth, int NewHeight)
{
    Assert(NewWidth <= GL_MAX_RENDERBUFFER_SIZE && NewHeight <= GL_MAX_RENDERBUFFER_SIZE);
    Assert(NewWidth <= GL_MAX_TEXTURE_SIZE && NewHeight <= GL_MAX_TEXTURE_SIZE);
    glBindFramebuffer(GL_FRAMEBUFFER, Target.FrameBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, NewWidth, NewHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, NewWidth, NewHeight);
    
    GLenum FramebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    AssertMsgBreakEx(FramebufferStatus == GL_FRAMEBUFFER_COMPLETE,
                     "ERROR: (OpenGL) Render buffer creation error: %s",
                     GetFramebufferStatusMsg(FramebufferStatus));
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

internal void
HDR_ResizeTarget(render_target *Target)
{
    glDeleteFramebuffers(2, &Target->FrameBuffer);
}

internal void
ResizeRenderTargets(render_target *Targets, int Count, int NewWidth, int NewHeight)
{
    for(int i = 0; i < Count; ++i)
    {
        switch(Targets[i].Type)
        {
            case HDR:
            {
                HDR_ResizeTarget(&Targets[i]);
            } break;
            
            case PFX:
            {
                PFX_ResizeTarget(Targets[i], NewWidth, NewHeight);
            } break;
            
            default:
            {
                // NOTE: SHADOW_MAP does not need to be resized as far as I can tell
                AssertMsgBreak("ERROR: Undefined framebuffer type resizing");
            }
        }
    }
}

internal void 
DrawSelectionRegion(unsigned int Shader,
                    float BeginX, float BeginY, 
                    float EndX, float EndY)
{
    static unsigned int QuadVAO, QuadVBO, QuadEBO = 0;
    if(QuadVAO == 0)
    {
        float Indices[] = 
        {
            0, 1, 3,
            1, 2, 3
        };
        
        glGenVertexArrays(1, &QuadVAO);
        glGenBuffers(1, &QuadEBO);
        glGenBuffers(1, &QuadVBO);
        glBindVertexArray(QuadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4, 0, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, QuadEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    }
    
    gl_viewport Viewport;
    glGetFloatv(GL_VIEWPORT, (float *)&Viewport);
    
    glm::mat4 OrthoMatrix = glm::ortho(Viewport.Left, Viewport.Right, Viewport.Bottom, Viewport.Top,
                                       0.0f, 1.0f);
    
    glm::vec3 RectVerts[] = 
    {
        glm::vec3(OrthoMatrix * glm::vec4(BeginX, BeginY, 0.0f, 1.0f)), // top right
        glm::vec3(OrthoMatrix * glm::vec4(EndX, EndY, 0.0f, 1.0f)),     // bottom right
        glm::vec3(OrthoMatrix * glm::vec4(BeginX, EndY, 0.0f, 1.0f)),   // bottom left
        glm::vec3(OrthoMatrix * glm::vec4(EndX, BeginY, 0.0f, 1.0f)),   // top left
    };
    
    glm::vec4 Color(1.0f, 0.0f, 0.0f, 1.0f);
    SetUniform4fv(Shader, "color", Color);
    glBindVertexArray(QuadVAO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(RectVerts), RectVerts);
    glUseProgram(Shader);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

internal void
DrawWorldGrid(unsigned int Shader, glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, float NearPlane, float FarPlane)
{
    // This function really depends on the shader. Maybe I should add the shader id
    // to the editor struct? Or idk. The shader id only makes sense in the context
    // of this function, so I'm not sure if it makes sense to always have the user remember
    // to pass it to the function. Maybe the shader gets compiled during the call to the
    // constructor of the editor (if I add a constructor)
    SetUniformMatrix4fv(Shader, "projection", ProjectionMatrix);
    SetUniformMatrix4fv(Shader, "view", ViewMatrix);
    RenderQuad();
}
