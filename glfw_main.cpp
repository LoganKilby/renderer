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

// NOTE: If the object's texture has an unexpected color, verify that the texture coordinate
// varibles match across the shader program. This is NOT a link error; There are no error
// messages for this bug.

void GLFW_FramebufferSizeCallback(GLFWwindow *, int, int);
void GLFW_MouseCallback(GLFWwindow *Window, double XPos, double YPos);
void GLFW_MouseScrollCallback(GLFWwindow *Window, double XOffset, double YOffset);
void TransparencyDepthSort(glm::vec3 *Array, int ArrayCount, glm::vec3 CameraPosition);

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
    
    GLFWwindow *Window = glfwCreateWindow(WindowWidth, WindowHeight, "Test window", NULL, NULL);
    glfwMakeContextCurrent(Window);
    glfwSetFramebufferSizeCallback(Window, GLFW_FramebufferSizeCallback);
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(Window, GLFW_MouseCallback);
    glfwSetScrollCallback(Window, GLFW_MouseScrollCallback);
    glfwSetCursorPos(Window, WindowWidth / 2.0f, WindowHeight / 2.0f);
    
    GLenum GlewError = glewInit();
    if(GlewError == GLEW_OK)
    {
        printf("OpenGL Initialized\n");
        printf("Vendor: "); printf((char *)glGetString(GL_VENDOR)); printf("\n");
        printf("Renderer: "); printf((char *)glGetString(GL_RENDERER)); printf("\n");
        printf("Version: "); printf((char *)glGetString(GL_VERSION)); printf("\n\n");
        // TODO: Do more initialization here?
        
        glEnable(GL_DEPTH_TEST);
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
    
    stbi_set_flip_vertically_on_load(true);
    model Asteroid = LoadModel("models/rock/rock.obj");
    model Planet = LoadModel("models/planet/planet.obj");
    
    unsigned int VAO, VBO;
    // CubeVerticies /3v, 2st, 3vn
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), &CubeVertices, GL_STATIC_DRAW);  
    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 5));
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
    glBindVertexArray(0);
    
    
    // TODO: Can I streamline shader loading and compiltion?
    unsigned int VertexShaderID;
    unsigned int FragmentShaderID;
    unsigned int GeometryShaderID;
    
    VertexShaderID = LoadAndCompileShader("shaders/model_vertex.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/model_frag.c", GL_FRAGMENT_SHADER);
    opengl_shader_program Program = CreateShaderProgram(VertexShaderID, FragmentShaderID);
    DebugPrintUniforms(Program.Id);
    
    VertexShaderID = LoadAndCompileShader("shaders/instanced_vertex.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/instanced_frag.c", GL_FRAGMENT_SHADER);
    opengl_shader_program InstancedProgram = CreateShaderProgram(VertexShaderID, FragmentShaderID);
    
    
    float SecondsElapsed;
    float PrevTime = 0;
    float FrameTime;
    int FPS;
    char FPS_OutputBuffer[20] = {};
    
    // Asteroid position setup
    unsigned int amount = 100;
    glm::mat4 *ModelMatricies = (glm::mat4 *)malloc(sizeof(glm::mat4) * amount);
    memset(ModelMatricies, 0, sizeof(glm::mat4) * amount);
    srand(glfwGetTime()); // initialize random seed	
    float radius = 100.0;
    float offset = 10.0f;
    glm::mat4 model;
    for(unsigned int i = 0; i < amount; i++)
    {
        model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));
        
        // 2. scale: scale between 0.05 and 0.25f
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));
        
        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
        
        // 4. now add to list of matrices
        ModelMatricies[i] = model;
    }
    
    unsigned int InstancedMatrixBuffer;
    glGenBuffers(1, &InstancedMatrixBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, InstancedMatrixBuffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &ModelMatricies[0], GL_STATIC_DRAW);
    
    size_t Vec4Size = sizeof(glm::vec4);
    for(unsigned int MeshIndex = 0; MeshIndex < Asteroid.Meshes.size(); ++MeshIndex)
    {
        glBindVertexArray(Asteroid.Meshes[MeshIndex].VAO);
        
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * Vec4Size, (void *)0);
        
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * Vec4Size, (void *)Vec4Size);
        
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * Vec4Size, (void *)(2 * Vec4Size));
        
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * Vec4Size, (void *)(3 * Vec4Size));
        
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        
        glBindVertexArray(0);
    }
    
    GlobalInput = {};
    GlobalInput.PrevMousePos.x = WindowWidth / 2;
    GlobalInput.PrevMousePos.y = WindowHeight / 2;
    
    // Camera
    // TODO: Set up a camera proper camera system
    glm::vec3 CameraPos(0.0f, 0.0f, 45.0f);
    glm::vec3 CameraFront(0.0f, 0.0f, -1.0f);
    glm::vec3 CameraUp(0.0f, 1.0f, 0.0f);
    CameraOrientation = {};
    CameraOrientation.Yaw = -90.0f;
    CameraOrientation.LookSpeed = 0.1f;
    CameraOrientation.PanSpeed = 10.0f;
    FieldOfView = 45.0f;
    
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
        
        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -3.0f, 0.0f));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(6.5f, 6.5f, 6.5f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        SetUniformMatrix4fv(Program.Id, "mvp", MVP);
        DrawModel(Program.Id, Planet);
        
        SetUniformMatrix4fv(InstancedProgram.Id, "view", ViewMatrix);
        SetUniformMatrix4fv(InstancedProgram.Id, "projection", ProjectionMatrix);
        for(int MeshIndex = 0; MeshIndex < Asteroid.Meshes.size(); ++MeshIndex)
        {
            mesh Mesh = Asteroid.Meshes[MeshIndex];
            glBindVertexArray(Mesh.VAO);
            glBindTexture(GL_TEXTURE_2D, Mesh.Textures[0].Id);
            glDrawElementsInstanced(GL_TRIANGLES, Mesh.IndexCount, GL_UNSIGNED_INT, 0, amount);
        }
        
        glfwSwapBuffers(Window);
        glfwPollEvents();
        OutputErrorQueue();
        FPS = 1 / FrameTime;
        sprintf(FPS_OutputBuffer, "FPS: %d\n", FPS);
        OutputDebugStringA(FPS_OutputBuffer);
        memset(FPS_OutputBuffer, 0, sizeof(FPS_OutputBuffer));
    }
    
    return 0;
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
