#include "windows.h"
#include "include/GL/glew.h"
#include "include/GLFW/glfw3.h"
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "include/qpc/qpc.h"
#include "stdio.h"
#include "types.h"
#include "utility.h"
#include "opengl_code.h"
#include "renderer.h"
#include "vertices.h"
#include "include/board_processing/board_processing.h"

#include "opengl_code.cpp"
#include "renderer.cpp"

// NOTE: If the object's texture has an unexpected color or is black, verify that the in/out 
// varibles match across the shader program. This is NOT a link error; There are no error 
// messages for this bug.

void GLFW_FramebufferSizeCallback(GLFWwindow *, int, int);
void GLFW_MouseCallback(GLFWwindow *Window, double XPos, double YPos);
void GLFW_MouseScrollCallback(GLFWwindow *Window, double XOffset, double YOffset);
void TransparencyDepthSort(glm::vec3 *Array, int ArrayCount, glm::vec3 CameraPosition);
void DebugRenderCube(void);

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

// TODO: Write camera system

global_variable global_input GlobalInput;
global_variable camera_orientation CameraOrientation;
global_variable float FieldOfView;
global_variable int TransparentIndexBuffer[5] = {};

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
    QPC_StartCounter();
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
        printf("INFO: OpenGL Initialized. %Lf ms\n", QPC_EndCounter() / 1000.0l);
        printf("Vendor: "); printf((char *)glGetString(GL_VENDOR)); printf("\n");
        printf("Renderer: "); printf((char *)glGetString(GL_RENDERER)); printf("\n");
        printf("OpenGL Version: "); printf((char *)glGetString(GL_VERSION)); printf("\n\n");
        
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
    
    QPC_StartCounter();
    SBoardData *BoardData = (SBoardData *)ReadFile("board_data/Edger/Hermary/board36.brd");
    
    std::vector<v3> ProfileVerts = ProcessProfileData(BoardData);
    raw_vertex_data BrdVertexData = GenerateVertexDataFromRaw(BoardData);
    printf("INFO: Board data generated. %Lf ms\n", QPC_EndCounter() / 1000.0l);
    int Size = BrdVertexData.BottomData.size();
    offscreen_buffer OffscreenBuffer = CreateOffscreenBuffer(WindowWidth, WindowHeight);
    
    unsigned int BottomDataVAO, BottomDataVBO;
    glGenVertexArrays(1, &BottomDataVAO);
    glGenBuffers(1, &BottomDataVBO);
    glBindVertexArray(BottomDataVAO);
    glBindBuffer(GL_ARRAY_BUFFER, BottomDataVBO);
    glBufferData(GL_ARRAY_BUFFER, BrdVertexData.BottomData.size() * sizeof(v3), &BrdVertexData.BottomData[0], GL_STATIC_DRAW);  
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glBindVertexArray(0);
    
    unsigned int ProfilePointsVAO, ProfilePointsVBO;
    glGenVertexArrays(1, &ProfilePointsVAO);
    glGenBuffers(1, &ProfilePointsVBO);
    glBindVertexArray(ProfilePointsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, ProfilePointsVBO);
    glBufferData(GL_ARRAY_BUFFER, ProfileVerts.size() * sizeof(v3), &ProfileVerts[0], GL_STATIC_DRAW);  
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glBindVertexArray(0);
    
    // TODO: Can I streamline shader loading and compiltion?
    unsigned int VertexShaderID;
    unsigned int FragmentShaderID;
    unsigned int GeometryShaderID;
    VertexShaderID = LoadAndCompileShader("shaders/board_vertex.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/board_frag.c", GL_FRAGMENT_SHADER);
    opengl_shader_program BoardProgram = CreateShaderProgram(VertexShaderID, FragmentShaderID, 0);
    
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
    CameraOrientation.PanSpeed = 0.3f;
    FieldOfView = 45.0f;
    
    point_light PointLight;
    PointLight.Position = glm::vec3(0.0f);
    
    glm::mat4 ModelMatrix;
    glm::mat4 ViewMatrix;
    glm::mat4 ViewSubMatrix;
    glm::mat4 ProjectionMatrix;
    glm::mat4 MVP;
    
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
        if(glfwGetKey(Window, GLFW_KEY_SPACE) == GLFW_PRESS)
            CameraPos.y += CameraOrientation.PanSpeed * FrameTime;
        
        glm::vec3 CameraAngle;
        CameraAngle.x = cos(glm::radians(CameraOrientation.Yaw)) * cos(glm::radians(CameraOrientation.Pitch));
        CameraAngle.y = sin(glm::radians(CameraOrientation.Pitch));
        CameraAngle.z = sin(glm::radians(CameraOrientation.Yaw)) * cos(glm::radians(CameraOrientation.Pitch));
        CameraFront = glm::normalize(CameraAngle);
        
        ViewMatrix = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
        ViewSubMatrix = glm::mat4(glm::mat3(ViewMatrix));
        
        ProjectionMatrix = glm::perspective(glm::radians(FieldOfView), 
                                            WindowWidth / WindowHeight, 
                                            0.1f, 
                                            1000.0f);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.0f, 0.05f, 3.0f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        SetUniformMatrix4fv(BoardProgram.Id, "mvp", MVP);
        SetUniform3fv(BoardProgram.Id, "color", glm::vec3(0.0f, 0.0f, 1.0f));
        glBindVertexArray(BottomDataVAO);
        glDrawArrays(GL_POINTS, 0, BrdVertexData.BottomData.size());
        
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(1.0f, 0.0f, 0.0f));
        //ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.0f, 0.05f, 3.0f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        SetUniformMatrix4fv(BoardProgram.Id, "mvp", MVP);
        SetUniform3fv(BoardProgram.Id, "color", glm::vec3(0.0f, 1.0f, 0.0f));
        glBindVertexArray(ProfilePointsVAO);
        glDrawArrays(GL_POINTS, 0, ProfileVerts.size());
        
        
        OutputErrorQueue();
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }
    
    return 0;
}

void DebugRenderCube()
{
    local_persist unsigned int CubeVAO;
    local_persist unsigned int CubeVBO;
    
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
        glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
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
    
    glBindVertexArray(CubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

struct transparent_sort_object
{
    float Length;
    int Index;
};

internal void 
TransparencyDepthSort(glm::vec3 *TransparentObjects, int ArrayCount, glm::vec3 CameraPosition)
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
