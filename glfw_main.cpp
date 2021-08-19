#include "windows.h"
#include "include/GL/glew.h"
#include "include/GLFW/glfw3.h"
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "include/stb/stb_image.h"
//#define FAST_OBJ_IMPLEMENTATION
//#include "include/fast_obj/fast_obj.h"
#include "include/assimp/Importer.hpp"
#include "include/assimp/scene.h"
#include "include/assimp/postprocess.h"
#include "include/qpc/qpc.h"
#include "stdio.h"
#include <vector>
#include "types.h"
#include "utility.h"
#include "opengl_code.h"
#include "renderer.h"
#include "vertices.h"

#include "opengl_code.cpp"
#include "renderer.cpp"

// NOTE: If the object's texture has an unexpected color or is black, verify that the in/out 
// varibles match across the shader program. This is NOT a link error; There are no error 
// messages for this bug.

void GLFW_FramebufferSizeCallback(GLFWwindow *, int, int);
void GLFW_MouseCallback(GLFWwindow *Window, double XPos, double YPos);
void GLFW_MouseScrollCallback(GLFWwindow *Window, double XOffset, double YOffset);
void TransparencyDepthSort(glm::vec3 *Array, int ArrayCount, glm::vec3 CameraPosition);
void RenderCube(void);
void RenderScene(unsigned int ShadowProgram);
void DebugRenderQuad(void);

struct global_input
{
    glm::vec2 PrevMousePos;
};

struct camera_orientation
{
    float Yaw;
    float Pitch;
    float Roll;
    float LookSpeed;
    float PanSpeed;
};

global_variable global_input GlobalInput;
global_variable camera_orientation CameraOrientation;
global_variable float FieldOfView;
global_variable int TransparentIndexBuffer[5] = {};
global_variable unsigned int CubeVAO;
global_variable unsigned int CubeVBO;

int WinMain(HINSTANCE hInstance,
            HINSTANCE hPrevInstance,
            LPSTR     lpCmdLine,
            int       nShowCmd)
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    SetConsoleTitle("Debug Console");
    HWND Console = GetConsoleWindow();
    SetWindowPos(Console, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    
    float WindowWidth = 1280.0f;
    float WindowHeight = 720.0f;
    if(glfwInit())
    {
        printf("GLFW Initialized\n");
        printf(glfwGetVersionString());
        printf("\n\n");
    }
    else
    {
        // TODO: Logging
        printf("ERROR: GLFW failed to initialize\n");
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4); // NOTE: Multsample buffer for MSAA, 4 samples per pixel
    GLFWwindow *Window = glfwCreateWindow(WindowWidth, WindowHeight, "Test window", NULL, NULL);
    glfwMakeContextCurrent(Window);
    glfwSetFramebufferSizeCallback(Window, GLFW_FramebufferSizeCallback);
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(Window, GLFW_MouseCallback);
    glfwSetScrollCallback(Window, GLFW_MouseScrollCallback);
    glfwSetCursorPos(Window, WindowWidth / 2.0f, WindowHeight / 2.0f);
    GLFWmonitor *Monitor = glfwGetPrimaryMonitor();
    const GLFWgammaramp *GammaRamp = glfwGetGammaRamp(Monitor);
    GLenum GlewError = glewInit();
    if(GlewError == GLEW_OK)
    {
        printf("OpenGL Initialized\n");
        printf("Vendor: "); printf((char *)glGetString(GL_VENDOR)); printf("\n");
        printf("Renderer: "); printf((char *)glGetString(GL_RENDERER)); printf("\n");
        printf("Version: "); printf((char *)glGetString(GL_VERSION)); printf("\n\n");
        
        // TODO: Do more initialization here?
        stbi_set_flip_vertically_on_load(true);
        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        // TODO: Logging
        printf("ERROR: Glew failed to initialize");
    }
    
    if(ASSERTIONS_ENABLED)
    {
        printf("INFO: Assertions turned ON\n");
    }
    else
    {
        printf("INFO: Assertions turned OFF\n");
    }
    
    offscreen_buffer OffscreenBuffer = CreateOffscreenBuffer(WindowWidth, WindowHeight);
    shadow_map ShadowCubeMap = CreateShadowCubeMap();
    float ShadowNearPlane = 1.0f;
    float ShadowFarPlane = 25.0f;
    float ShadowAspectRatio = ShadowCubeMap.DepthBufferWidth / ShadowCubeMap.DepthBufferHeight;
    glm::mat4 ShadowProjectionMatrix = glm::perspective(glm::radians(90.0f),
                                                        ShadowAspectRatio,
                                                        ShadowNearPlane,
                                                        ShadowFarPlane);
    
    // TODO: Can I streamline shader loading and compiltion?
    unsigned int VertexShaderID;
    unsigned int FragmentShaderID;
    unsigned int GeometryShaderID;
    
    VertexShaderID = LoadAndCompileShader("shaders/shadow_vertex.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/shadow_frag.c", GL_FRAGMENT_SHADER);
    opengl_shader_program ShadowProgram = CreateShaderProgram(VertexShaderID, FragmentShaderID, 0);
    SetUniform1i(ShadowProgram.Id, "diffuseMap", 0);
    SetUniform1i(ShadowProgram.Id, "normalMap", 1);
    
    VertexShaderID = LoadAndCompileShader("shaders/post_effects_vertex.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/post_effects_frag.c", GL_FRAGMENT_SHADER);
    opengl_shader_program PostEffectsProgram = CreateShaderProgram(VertexShaderID, FragmentShaderID, 0);
    
    VertexShaderID = LoadAndCompileShader("shaders/omnishadowmap_vertex.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/omnishadowmap_frag.c", GL_FRAGMENT_SHADER);
    GeometryShaderID = LoadAndCompileShader("shaders/omnishadowmap_geometry.c", GL_GEOMETRY_SHADER);
    opengl_shader_program OmniShadowProgram = CreateShaderProgram(VertexShaderID, FragmentShaderID, GeometryShaderID);
    
    texture_unit FloorTexture = LoadTexture("textures/brickwall.jpg");
    texture_unit FloorNormalMap = LoadTexture("textures/brickwall_normal.jpg");
    
    float SecondsElapsed;
    float PrevTime = 0;
    float FrameTime;
    int FPS;
    char FPS_OutputBuffer[20] = {};
    
    GlobalInput = {};
    GlobalInput.PrevMousePos.x = WindowWidth / 2;
    GlobalInput.PrevMousePos.y = WindowHeight / 2;
    
    // Camera
    // TODO: Set up a camera proper camera system
    glm::vec3 CameraPos(0.0f, 0.0f, 0.0f);
    glm::vec3 CameraFront(0.0f, 0.0f, -1.0f);
    glm::vec3 CameraUp(0.0f, 1.0f, 0.0f);
    CameraOrientation = {};
    CameraOrientation.Yaw = -90.0f;
    CameraOrientation.LookSpeed = 0.1f;
    CameraOrientation.PanSpeed = 10.0f;
    FieldOfView = 45.0f;
    
    point_light PointLight;
    PointLight.Position = glm::vec3(0.5f, 1.0f, 0.3f);
    
    glm::mat4 ModelMatrix;
    glm::mat4 ViewMatrix;
    glm::mat4 ViewSubMatrix;
    glm::mat4 ProjectionMatrix;
    glm::mat4 MVP;
    
    glm::vec3 CubePositions[] =
    {
        glm::vec3(0.0f, 1.5f, 3.0f),
        glm::vec3(2.0f, 0.01f, 1.0f),
        glm::vec3(-1.0f, 0.01f, 2.0f)
    };
    
    SetUniform1i(ShadowProgram.Id, "diffuseTexture", 0);
    
    while(!glfwWindowShouldClose(Window))
    {
        SecondsElapsed = glfwGetTime();
        FrameTime = SecondsElapsed - PrevTime;
        PrevTime = SecondsElapsed;
        
        if(glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
            CameraPos += CameraFront * CameraOrientation.PanSpeed * FrameTime;
        if(glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
            CameraPos -= CameraFront * CameraOrientation.PanSpeed * FrameTime;
        if(glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
            CameraPos -= glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraOrientation.PanSpeed * FrameTime;
        if(glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
            CameraPos += glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraOrientation.PanSpeed * FrameTime;
        if(glfwGetKey(Window, GLFW_KEY_Q) == GLFW_PRESS)
            CameraPos.y += CameraOrientation.PanSpeed * FrameTime;
        if(glfwGetKey(Window, GLFW_KEY_E) == GLFW_PRESS)
            CameraPos.y -= CameraOrientation.PanSpeed * FrameTime;
        
        glm::vec3 CameraAngle;
        CameraAngle.x = cos(glm::radians(CameraOrientation.Yaw)) * cos(glm::radians(CameraOrientation.Pitch));
        CameraAngle.y = sin(glm::radians(CameraOrientation.Pitch));
        CameraAngle.z = sin(glm::radians(CameraOrientation.Yaw)) * cos(glm::radians(CameraOrientation.Pitch));
        CameraFront = glm::normalize(CameraAngle);
        
        PointLight.Position = CameraPos;
        
        ViewMatrix = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
        ViewSubMatrix = glm::mat4(glm::mat3(ViewMatrix));
        
        ProjectionMatrix = glm::perspective(glm::radians(FieldOfView), 
                                            WindowWidth / WindowHeight, 
                                            0.1f, 
                                            1000.0f);
        
        
        glViewport(0, 0, WindowWidth, WindowHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        SetUniform3fv(ShadowProgram.Id, "lightPos", PointLight.Position);
        SetUniform3fv(ShadowProgram.Id, "viewPos", CameraPos);
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5.0f));
        SetUniformMatrix4fv(ShadowProgram.Id, "model", ModelMatrix);
        SetUniformMatrix4fv(ShadowProgram.Id, "projection", ProjectionMatrix);
        SetUniformMatrix4fv(ShadowProgram.Id, "view", ViewMatrix);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, FloorTexture.Id);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, FloorNormalMap.Id);
        DebugRenderQuad();
        
        
        glfwSwapBuffers(Window);
        glfwPollEvents();
        OutputErrorQueue();
    }
    
    return 0;
}

void DebugRenderQuad()
{
    static unsigned int quadVAO, quadVBO = 0;
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        };
        // configure plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    }
    
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void RenderCube()
{
    // initialize (if necessary)
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

struct transparent_sort_object
{
    float Length;
    int Index;
};

internal void TransparencyDepthSort(glm::vec3 *TransparentObjects, int ArrayCount, glm::vec3 CameraPosition)
{
    // The depth buffer is too small to hold the objects
    Assert(ArrayCount <= ArrayCount(TransparentIndexBuffer));
    memset(TransparentIndexBuffer, 0, sizeof(TransparentIndexBuffer));
    
    transparent_sort_object TempLengths[5] = {};
    
    float Length;
    for(int i = 0; i < ArrayCount; ++i)
    {
        Length = glm::length(CameraPosition - TransparentObjects[i]);
        TempLengths[i].Length = Length;
        TempLengths[i].Index = i;
    }
    
    while(true)
    {
        int SwapCount = 0;
        
        for(int i = 0; i < 4; i++)
        {
            if(TempLengths[i].Length < TempLengths[i + 1].Length)
            {
                transparent_sort_object Temp = TempLengths[i];
                TempLengths[i] = TempLengths[i + 1];
                TempLengths[i + 1] = Temp;
                SwapCount++;
            }
        }
        
        if(SwapCount == 0)
        {
            break;
        }
    }
    
    for(int i = 0; i < 5; ++i)
    {
        TransparentIndexBuffer[i] = TempLengths[i].Index;
    }
}

void GLFW_FramebufferSizeCallback(GLFWwindow *Window, int Width, int Height)
{
    glViewport(0, 0, Width, Height);
}

void GLFW_MouseScrollCallback(GLFWwindow *Window, double XOffset, double YOffset)
{
    FieldOfView -= (float)YOffset;
    if(FieldOfView < 1.0f)
        FieldOfView = 1;
    if(FieldOfView > 45.0f)
        FieldOfView = 45.0f;
}

void GLFW_MouseCallback(GLFWwindow *Window, double XPos, double YPos)
{
    float XOffset = (XPos - GlobalInput.PrevMousePos.x) * CameraOrientation.LookSpeed;
    float YOffset = (GlobalInput.PrevMousePos.y - YPos) * CameraOrientation.LookSpeed;
    
    GlobalInput.PrevMousePos.x = XPos;
    GlobalInput.PrevMousePos.y = YPos;
    
    CameraOrientation.Yaw += XOffset;
    CameraOrientation.Pitch += YOffset;
    
    if (CameraOrientation.Pitch > 89.0f) 
        CameraOrientation.Pitch = 89.0f;
    if (CameraOrientation.Pitch < -89.0f)
        CameraOrientation.Pitch = -89.0f;
}
