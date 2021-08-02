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

void GLFW_FramebufferSizeCallback(GLFWwindow *, int, int);
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
    
    
    // TODO: Assimp is way too slow.
    
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
        glDepthFunc(GL_LESS);
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
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
    
    unsigned int CubeVAO, CubeVBO;
    glGenVertexArrays(1, &CubeVAO);
    glGenBuffers(1, &CubeVBO);
    glBindVertexArray(CubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), &CubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glBindVertexArray(0);
    
    unsigned int PlaneVAO, PlaneVBO;
    glGenVertexArrays(1, &PlaneVAO);
    glGenBuffers(1, &PlaneVBO);
    glBindVertexArray(PlaneVAO);
    glBindBuffer(GL_ARRAY_BUFFER, PlaneVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PlaneVertices), &PlaneVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glBindVertexArray(0);
    
    texture_unit CubeTexture = LoadTexture("textures/marble.jpg");
    texture_unit PlaneTexture = LoadTexture("textures/metal.jpg");
    
    unsigned int VertexShaderID;
    unsigned int FragmentShaderID;
    
    VertexShaderID = LoadAndCompileShader("shaders/vertex_shader.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/frag_shader.c", GL_FRAGMENT_SHADER);
    opengl_shader_program Program = CreateShaderProgram(VertexShaderID, FragmentShaderID);
    
    VertexShaderID = LoadAndCompileShader("shaders/outline_vertex.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/outline_frag.c", GL_FRAGMENT_SHADER);
    opengl_shader_program OutlineProgram = CreateShaderProgram(VertexShaderID, FragmentShaderID);
    
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
    
    glm::vec3 LightCubePosition = glm::vec3(0.0f);
    // Light
    // NOTE: Consider using vec4(x, y, z, 0.0f) for directions, and vec4(x, y, z, 1.0f) for positions
    // Vectors shouldn't be effected by translations
    glm::vec3 LightColor(1.0f, 1.0f, 1.0f);
    spot_light SpotLight = {};
    //SpotLight.Position is CameraPostion
    //SpotLight.Direction is CameraFront
    SpotLight.Ambient = glm::vec3(0.2f); // Can multiply by the light color
    SpotLight.Diffuse = glm::vec3(0.8f); // Can multiply by the light color
    SpotLight.Specular = glm::vec3(1.0f);
    SpotLight.NearRadius = glm::cos(glm::radians(12.5));
    SpotLight.FarRadius = glm::cos(glm::radians(17.5));
    SpotLight.Constant = 1.0f;
    SpotLight.Linear = 0.07f;
    SpotLight.Quadratic = 0.017f;
    
    directional_light DirectionalLight;
    DirectionalLight.Direction = glm::vec3(0.0f, 0.0f, -1.0f);
    DirectionalLight.Ambient = glm::vec3(0.5f);
    DirectionalLight.Diffuse = glm::vec3(0.5f);
    DirectionalLight.Specular = glm::vec3(1.0f);
    SetShaderDirectionalLight(Program.Id, "DirectionalLight", DirectionalLight);
    
    glm::vec3 PointLightPosOffset(0.0f);
    glm::vec3 PointLightPositions[] = 
    {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };
    
    point_light PointLight = {};
    PointLight.Ambient = glm::vec3(0.5f);
    PointLight.Diffuse = glm::vec3(0.5f);
    PointLight.Specular = glm::vec3(1.0f);
    PointLight.Constant = 1.0f;
    PointLight.Linear = 0.14f;
    PointLight.Quadratic = 0.07f;
    
    // Untitled program uniform locations (GLuint)
    int ModelMVPLocation = glGetUniformLocation(Program.Id, "mvp");
    int ModelViewLocation = glGetUniformLocation(Program.Id, "view");
    int ModelLocation = glGetUniformLocation(Program.Id, "model");
    
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
            CameraPos.y += CameraOrientation.PanSpeed * FrameTime;
        if(glfwGetKey(Window, GLFW_KEY_E) == GLFW_PRESS)
            CameraPos.y -= CameraOrientation.PanSpeed * FrameTime;
        
        glm::vec3 CameraAngle;
        CameraAngle.x = cos(glm::radians(CameraOrientation.Yaw)) * cos(glm::radians(CameraOrientation.Pitch));
        CameraAngle.y = sin(glm::radians(CameraOrientation.Pitch));
        CameraAngle.z = sin(glm::radians(CameraOrientation.Yaw)) * cos(glm::radians(CameraOrientation.Pitch));
        CameraFront = glm::normalize(CameraAngle);
        
        //SpotLight.Position = CameraPos;
        //SpotLight.Direction = CameraFront;
        
        ViewMatrix = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
        
        ProjectionMatrix = glm::perspective(glm::radians(FieldOfView), 
                                            WindowWidth / WindowHeight, 
                                            0.1f, 
                                            100.0f);
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        // Floor
        glUseProgram(Program.Id);
        glStencilMask(0x00);
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -0.01f, 0.0f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        SetUniformMatrix4fv(Program.Id, "model", ModelMatrix);
        SetUniformMatrix4fv(Program.Id, "mvp", MVP);
        glBindVertexArray(PlaneVAO);
        glBindTexture(GL_TEXTURE_2D, PlaneTexture.Id);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        
        // Cubes
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        
        glBindVertexArray(CubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, CubeTexture.Id);
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-1.0f, 0.0f, -1.0f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        SetUniformMatrix4fv(Program.Id, "model", ModelMatrix);
        SetUniformMatrix4fv(Program.Id, "mvp", MVP);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(2.0f, 0.0f, 0.0f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        SetUniformMatrix4fv(Program.Id, "model", ModelMatrix);
        SetUniformMatrix4fv(Program.Id, "mvp", MVP);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // Outline draw pass
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        
        glUseProgram(OutlineProgram.Id);
        glBindVertexArray(CubeVAO);
        glBindTexture(GL_TEXTURE_2D, CubeTexture.Id);
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-1.0f, 0.0f, -1.0f));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.1f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        SetUniformMatrix4fv(OutlineProgram.Id, "mvp", MVP);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(2.0f, 0.0f, 0.0f));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.1f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        SetUniformMatrix4fv(OutlineProgram.Id, "mvp", MVP);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);
        
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
