#include "windows.h"
#include "include/GL/glew.h"
#include "include/GLFW/glfw3.h"
#include "stdio.h"
#include "types.h"

#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "include/stb_image.h"
#include "utility.h"
#include "opengl_code.h"
#include "cube.h"

#include "opengl_code.cpp"
#include "math_functions.cpp"

void GLFW_FramebufferSizeCallback(GLFWwindow *, int, int);
void ProcessInput(GLFWwindow);
internal unsigned char *ReadEntireFile(char *, unsigned long int *);
internal char *ReadEntireFileToString(char *);
void ProcessKeyboardInput(GLFWwindow *, int, int, int, int);
void GLFW_MouseCallback(GLFWwindow *Window, double XPos, double YPos);
void GLFW_MouseScrollCallback(GLFWwindow *Window, double XOffset, double YOffset);

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
        glEnable(GL_DEPTH_TEST);
        // TODO: Do more initialization here?
    }
    else
    {
        // TODO: Logging
        printf("ERROR: Glew failed to initialize");
    }
    
    stbi_set_flip_vertically_on_load(true);
    
    float Vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    
    glm::vec3 CubePositions[] = {
        { 2.0f,  5.0f, -15.0f}, 
        {-1.5f, -2.2f, -2.5f},  
        {-3.8f, -2.0f, -12.3f},  
        { 2.4f, -0.4f, -3.5f},  
        {-1.7f,  3.0f, -7.5f},  
        { 1.3f, -2.0f, -2.5f},  
        { 1.5f,  2.0f, -2.5f}, 
        { 1.5f,  0.2f, -1.5f}, 
        {-1.3f,  1.0f, -1.5f}  
    };
    
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    unsigned int LightVAO;
    glGenVertexArrays(1, &LightVAO);
    glBindVertexArray(LightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    
    texture ContainerTexture = LoadTexture("textures/container2.png");
    texture ContainerSpecMap = LoadTexture("textures/container2_specularmap.png");
    texture ContainerEmissionMap = LoadTexture("textures/emission_map.jpg");
    
    char *VertexShaderFromFile;
    char *FragShaderFromFile;
    opengl_shader VertexShader;
    opengl_shader FragmentShader;
    
    VertexShaderFromFile = ReadEntireFileToString("shaders/vertex_shader.c");
    FragShaderFromFile = ReadEntireFileToString("shaders/frag_shader.c");
    VertexShader = CompileShader(VertexShaderFromFile, VERTEX);
    FragmentShader = CompileShader(FragShaderFromFile, FRAGMENT);
    opengl_shader_program Program = CreateShaderProgram(VertexShader, FragmentShader);
    free(VertexShaderFromFile);
    free(FragShaderFromFile);
    
    VertexShaderFromFile = ReadEntireFileToString("shaders/light_source_vertex_shader.c");
    FragShaderFromFile = ReadEntireFileToString("shaders/light_source_frag_shader.c");
    VertexShader = CompileShader(VertexShaderFromFile, VERTEX);
    FragmentShader = CompileShader(FragShaderFromFile, FRAGMENT);
    opengl_shader_program LightProgram = CreateShaderProgram(VertexShader, FragmentShader);
    free(VertexShaderFromFile);
    free(FragShaderFromFile);
    
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    
    float SecondsElapsed;
    float PrevTime = 0;
    float FrameTime;
    
    GlobalInput = {};
    GlobalInput.PrevMousePos.x = WindowWidth / 2;
    GlobalInput.PrevMousePos.y = WindowHeight / 2;
    
    // Camera
    glm::vec3 CameraPos(0.0f, 0.0f, 0.0f);
    glm::vec3 CameraFront(0.0f, 0.0f, -1.0f);
    glm::vec3 CameraUp(0.0f, 1.0f, 0.0f);
    CameraOrientation = {};
    CameraOrientation.Yaw = -90.0f;
    CameraOrientation.LookSpeed = 0.1f;
    CameraOrientation.PanSpeed = 2.0f;
    FieldOfView = 45.0f;
    
    OutputErrorQueue();
    
    // Light
    // NOTE: Consider using vec4(x, y, z, 0.0f) for directions, and vec4(x, y, z, 1.0f) for positions
    // Vectors shouldn't be effected by translations
    glm::vec3 LightColor(1.0f, 1.0f, 1.0f);
    light_source Light;
    Light.Position = glm::vec3(0.0, 0.0, -5.0f); // NOTE: Directional lights don't have a position
    Light.Ambient = glm::vec3(0.1f); // Can multiply by the light color
    Light.Diffuse = glm::vec3(0.8f); // Can multiply by the light color
    Light.Specular = glm::vec3(1.0f);
    float LightBoundary = glm::cos(glm::radians(12.5));
    float LightBoundaryEnd = glm::cos(glm::radians(17.5));
    
    glUseProgram(Program.Id);
    SetUniform1f(Program.Id, "constant", 1.0f);
    SetUniform1f(Program.Id, "linear", 0.09f);
    SetUniform1f(Program.Id, "quadratic", 0.032f);
    SetUniform1f(Program.Id, "spotlightBoundary", LightBoundary);
    SetUniform1f(Program.Id, "spotlightBoundaryEnd", LightBoundaryEnd);
    
    // Material
    material BasicMaterial = {};
    BasicMaterial.DiffuseMapTexUnit = 0;
    BasicMaterial.SpecularMapTexUnit = 1;
    BasicMaterial.Shininess = 32.0f;
    SetShaderMaterial(Program.Id, "material", BasicMaterial);
    
    // Light program uniform locations
    int LightMVPLocation = glGetUniformLocation(LightProgram.Id, "mvp");
    
    // Untitled program uniform locations (GLuint)
    int ModelMVPLocation = glGetUniformLocation(Program.Id, "mvp");
    int ModelViewLocation = glGetUniformLocation(Program.Id, "view");
    int ModelLocation = glGetUniformLocation(Program.Id, "model");
    int LightPosLocation = glGetUniformLocation(Program.Id, "lightPos");
    int ViewPosLocation = glGetUniformLocation(Program.Id, "viewPos");
    
    glm::mat4 ModelMatrix;
    glm::mat4 ViewMatrix;
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
            Light.Position.z += CameraOrientation.PanSpeed * FrameTime;
        if(glfwGetKey(Window, GLFW_KEY_E) == GLFW_PRESS)
            Light.Position.z -= CameraOrientation.PanSpeed * FrameTime;
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::vec3 CameraAngle;
        CameraAngle.x = cos(glm::radians(CameraOrientation.Yaw)) * cos(glm::radians(CameraOrientation.Pitch));
        CameraAngle.y = sin(glm::radians(CameraOrientation.Pitch));
        CameraAngle.z = sin(glm::radians(CameraOrientation.Yaw)) * cos(glm::radians(CameraOrientation.Pitch));
        CameraFront = glm::normalize(CameraAngle);
        
        ViewMatrix = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
        
        ProjectionMatrix = glm::perspective(glm::radians(FieldOfView), 
                                            WindowWidth / WindowHeight, 
                                            0.1f, 
                                            100.0f);
        Light.Position = CameraPos;
        Light.Direction = CameraFront;
        SetShaderLightSource(Program.Id, "light", Light);
#if 0
        // Draw light
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, Light.Position);
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.2f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUseProgram(LightProgram.Id);
        glUniformMatrix4fv(LightMVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
#endif
        // Draw world objects
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ContainerTexture.TextureId);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ContainerSpecMap.TextureId);
        
        glUseProgram(Program.Id);
        glUniform3fv(ViewPosLocation, 1, &CameraPos[0]);
        glUniformMatrix4fv(ModelViewLocation, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
        for(int CubeIndex = 0; CubeIndex < ArrayCount(CubePositions); ++CubeIndex)
        {
            ModelMatrix = glm::mat4(1.0f);
            ModelMatrix = glm::translate(ModelMatrix, CubePositions[CubeIndex]);
            MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
            glUniformMatrix4fv(ModelMVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));
            glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        glfwSwapBuffers(Window);
        glfwPollEvents();
        OutputErrorQueue();
    }
    
    return 0;
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
