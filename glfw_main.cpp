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
#include "vertices.h"

#include "opengl_code.cpp"
#include "renderer.cpp"
#include "input.cpp"
#include "camera.cpp"


// NOTE: (On frustrating shader bugs)
// If the object's texture has an unexpected color or is black, verify that the in/out 
// varible names match across the shader program. This is NOT a link error; There are no error 
// messages for this bug.
// If the textured geometry is a solid color, look at the vertex data and confirm that it aligns
// with the vertex attributes. Also, the default value for unspecified texture coordinate
// attributes is a value that results in the bottom left corner of the bound texture being sampled;
// This would result in the geometry being textured as a solid color, the color of the bottom-left
// texel.

internal void GLFW_FramebufferSizeCallback(GLFWwindow *Window, int, int);
internal void GLFW_MouseCallback(GLFWwindow *Window, double XPos, double YPos);
internal void GLFW_MouseScrollCallback(GLFWwindow *Window, double XOffset, double YOffset);
internal void GLFW_MouseButtonCallback(GLFWwindow *Window, int Button, int Action, int Mods);
internal void GLFW_KeyCallback(GLFWwindow *Window, int Key, int Scancode, int Action, int Mods);
internal void TransparencyDepthSort(glm::vec3 *Array, int ArrayCount, glm::vec3 CameraPosition);

// TODO: I would like to support multiple cameras.
global_variable camera PrimaryCamera;
global_variable input_state GlobalInputState;
global_variable key_table GlobalKeyState;
global_variable mouse_button_table GlobalMouseButtonState;
global_variable unsigned int PrimitiveShader;

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
    
    int WindowWidth = 1280.0f;
    int WindowHeight = 720.0f;
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
    
    // TODO: With O2 optimizations turned on, GLFW isn't AS slow, but still bad.
    glfwWindowHint(GLFW_SAMPLES, 4); // NOTE: Multsample buffer for MSAA, 4 samples per pixel
    GLFWwindow *Window = glfwCreateWindow(WindowWidth, WindowHeight, "Test window", NULL, NULL);
    glfwMakeContextCurrent(Window);
    glfwSetFramebufferSizeCallback(Window, GLFW_FramebufferSizeCallback);
    glfwSetCursorPosCallback(Window, GLFW_MouseCallback);
    glfwSetScrollCallback(Window, GLFW_MouseScrollCallback);
    glfwSetKeyCallback(Window, GLFW_KeyCallback);
    
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(Window, WindowWidth / 2.0f, WindowHeight / 2.0f);
    
    GLenum GlewError = glewInit();
    if(GlewError == GLEW_OK)
    {
        printf("OpenGL Initialized\n");
        printf("Vendor: "); printf((char *)glGetString(GL_VENDOR)); printf("\n");
        printf("Renderer: "); printf((char *)glGetString(GL_RENDERER)); printf("\n");
        printf("Version: "); printf((char *)glGetString(GL_VERSION)); printf("\n\n");
        
        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glEnable(GL_DEPTH_TEST);
        
        // NOTE: GLFW provides the multisample buffers by calling glfwWindowHint with
        // GLFW_SAMPLES, 4 (# of samples). "Anti-aliasing" is enabled here, but I want
        // to enable my own custom version eventually
        glEnable(GL_MULTISAMPLE);
        
        GlobalTextureCache = {};
        PrimaryCamera = {};
        GlobalInputState = {};
        GlobalKeyState = {};
        GlobalMouseButtonState = {};
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
    
    render_target HDR_RenderTarget = HDR_CreateRenderTarget(WindowWidth, WindowHeight);
    render_target PFX_RenderTarget = PFX_CreateRenderTarget(WindowWidth, WindowHeight);
    shadow_map ShadowCubeMap = CreateShadowCubeMap();
    float ShadowNearPlane = 1.0f;
    float ShadowFarPlane = 25.0f;
    float ShadowAspectRatio = ShadowCubeMap.DepthBufferWidth / ShadowCubeMap.DepthBufferHeight;
    glm::mat4 ShadowProjectionMatrix = glm::perspective(glm::radians(90.0f),
                                                        ShadowAspectRatio,
                                                        ShadowNearPlane,
                                                        ShadowFarPlane);
    
    unsigned int ScreenVAO = CreateScreenRenderQuad();
    
    // TODO: Can I streamline shader loading and compiltion?
    unsigned int VertexShaderID;
    unsigned int FragmentShaderID;
    unsigned int GeometryShaderID;
    
    VertexShaderID = LoadAndCompileShader("shaders/shadow_vertex.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/shadow_frag.c", GL_FRAGMENT_SHADER);
    opengl_shader_program ShadowProgram = CreateShaderProgram(VertexShaderID, FragmentShaderID, 0);
    SetUniform1i(ShadowProgram.Id, "diffuseMap", 0);
    SetUniform1i(ShadowProgram.Id, "normalMap", 1);
    SetUniform1i(ShadowProgram.Id, "specularMap", 2);
    //SetUniform1i(ShadowProgram.Id, "depthMap", 3);
    
    VertexShaderID = LoadAndCompileShader("shaders/blit_screen_vertex.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/blit_screen_frag.c", GL_FRAGMENT_SHADER);
    opengl_shader_program BlitShader = CreateShaderProgram(VertexShaderID, FragmentShaderID, 0);
    
    VertexShaderID = LoadAndCompileShader("shaders/hdr_pass_vertex.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/hdr_pass_frag.c", GL_FRAGMENT_SHADER);
    opengl_shader_program GammaProgram = CreateShaderProgram(VertexShaderID, FragmentShaderID, 0);
    
    VertexShaderID = LoadAndCompileShader("shaders/blit_texture_vertex.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/blit_texture_frag.c", GL_FRAGMENT_SHADER);
    opengl_shader_program BlitTextureProgram = CreateShaderProgram(VertexShaderID, FragmentShaderID, 0);
    
    texture FloorTexture = LoadTextureToLinear("textures/brickwall.jpg");
    texture FloorNormalMap = LoadTexture("textures/brickwall_normal.jpg");
    texture FloorSpecTexture = LoadTexture("textures/brickwall_grayscale.jpg");
    texture LinearTexture = LoadTextureToLinear("textures/container.jpg");
    texture GammaTexture = LoadTexture("textures/container.jpg");
    
    GlobalInputState = {};
    GlobalInputState.MousePos.x = WindowWidth / 2;
    GlobalInputState.MousePos.y = WindowHeight / 2;
    
    // Camera
    // TODO: Set up a camera proper camera system
    PrimaryCamera.Position = glm::vec3(0.015581f, 1.017382f, 14.225583f); // DEBUG VALUES
    PrimaryCamera.Orientation.Yaw = -90.0f;
    PrimaryCamera.Orientation.Pitch = -2.8f; // DEBUG VALUE
    PrimaryCamera.LookSpeed = 7.0f;
    PrimaryCamera.PanSpeed = 35.0f;
    PrimaryCamera.FieldOfView = 45.0f;
    
    glm::mat4 ModelMatrix;
    glm::mat4 ViewMatrix;
    glm::mat3 NormalMatrix;
    glm::mat4 ProjectionMatrix;
    glm::mat4 MVP;
    
    float Exposure = 0.5f;
    float Gamma = 2.2f;
    
    while(!glfwWindowShouldClose(Window))
    {
        UpdateClock(&GlobalInputState);
        
        // UpdateAndRender
        
        input_command FrameInput;
        while(PopInputCommand(&GlobalKeyState, &GlobalInputState.CommandBuffer, &FrameInput))
        {
            if(FrameInput.Device == MOUSE && PrimaryCamera.Mode == PAN)
            {
                if(FrameInput.Action == PRESSED)
                {
                    // TODO: Select something
                    GlobalInputState.Clicked = 1;
                }
                else if(FrameInput.Action == RELEASED)
                {
                    GlobalInputState.Clicked = 0;
                }
            }
        }
        
        gesture FrameGesture;
        while(PopGesture(&GlobalInputState.GestureBuffer, &FrameGesture))
        {
            if(FrameGesture.Type == MOVE && PrimaryCamera.Mode == FREE)
            {
                
            }
            else if(FrameGesture.Type == SCROLL)
            {
                ZoomCamera(&PrimaryCamera, FrameGesture.Offset.Pitch);
            }
            
            if(PrimaryCamera.Mode == FREE)
            {
                if(FrameGesture.Type == MOVE)
                {
                    RotateFreeCamera(&PrimaryCamera, FrameGesture.Offset, GlobalInputState.dt);
                }
                
            }
            else if(PrimaryCamera.Mode == PAN)
            {
                if(FrameGesture.Type == MOVE)
                {
                    // TODO: Check if we're selecting something with a mouse click
                    // If we didn't select anything, start drawing a selection region
                    // TODO: I need some notion of an entity soon
                }
            }
        }
        
        MoveCameraByKeyPressed(&PrimaryCamera, GlobalKeyState, GlobalInputState.dt);
        
        ViewMatrix = GetCameraViewMatrix(PrimaryCamera);
        ProjectionMatrix = glm::perspective(glm::radians(PrimaryCamera.FieldOfView), 
                                            (float)WindowWidth / (float)WindowHeight, 
                                            0.1f, 
                                            1000.0f);
        
        glClearColor(0.01f, 0.01f, 0.01f, 0.01f);
        
        glBindFramebuffer(GL_FRAMEBUFFER, HDR_RenderTarget.FrameBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        SetUniform3fv(ShadowProgram.Id, "lightPos", PrimaryCamera.Position);
        SetUniform3fv(ShadowProgram.Id, "viewPos", PrimaryCamera.Position);
        SetUniformMatrix4fv(ShadowProgram.Id, "projection", ProjectionMatrix);
        SetUniformMatrix4fv(ShadowProgram.Id, "view", ViewMatrix);
        
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5.0f));
        NormalMatrix = glm::transpose(glm::inverse(glm::mat3(ModelMatrix)));
        SetUniformMatrix3fv(ShadowProgram.Id, "normalMatrix", NormalMatrix);
        SetUniformMatrix4fv(ShadowProgram.Id, "model", ModelMatrix);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, FloorTexture.Id);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, FloorNormalMap.Id);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, FloorSpecTexture.Id);
        DebugRenderQuad();
        
        // HDR pass: HDR and Gamma Correction
        glBindFramebuffer(GL_FRAMEBUFFER, PFX_RenderTarget.FrameBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(GammaProgram.Id);
        SetUniform1f(GammaProgram.Id, "exposure", Exposure);
        SetUniform1f(GammaProgram.Id, "gamma", Gamma);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, HDR_RenderTarget.ColorBuffer);
        glBindVertexArray(ScreenVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
        
        // PFX pass
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(BlitShader.Id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, PFX_RenderTarget.ColorBuffer);
        glBindVertexArray(ScreenVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
        
        // UI pass ?
        
        glfwSwapBuffers(Window);
        glfwPollEvents();
        OpenGL_OutputErrorQueue();
    }
    
    return 0;
}

internal void 
GLFW_FramebufferSizeCallback(GLFWwindow *Window, int Width, int Height)
{
    // TODO: Resize rendering buffers
    glViewport(0, 0, Width, Height);
}

internal void 
GLFW_MouseScrollCallback(GLFWwindow *Window, double XOffset, double YOffset)
{
    RegisterMouseScroll(&GlobalInputState, XOffset, YOffset);
}

internal void 
GLFW_MouseCallback(GLFWwindow *Window, double XPos, double YPos)
{
    RegisterMouseMovement(&GlobalInputState, GlobalMouseButtonState, XPos, YPos);
}

internal void
GLFW_KeyCallback(GLFWwindow *Window, int Key, int Scancode, int Action, int Mods)
{
    RegisterKeyboardInput(&GlobalInputState, &GlobalKeyState, Key, Scancode, Action, Mods);
}

internal void
GLFW_MouseButtonCallback(GLFWwindow *Window, int Button, int Action, int Mods)
{
    RegisterMouseButtonInput(&GlobalInputState, &GlobalMouseButtonState, Button, Action, Mods);
}