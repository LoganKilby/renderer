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
#include "math_util.h"

#include "opengl_code.cpp"
#include "renderer.cpp"
#include "input.cpp"
#include "camera.cpp"
#include "editor.cpp"
#include "entity.cpp"
#include "collision.cpp"

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
// TODO: Add these as part of the game memory (allocated up front)
global_variable camera GameCamera;
global_variable camera EditorCamera;
global_variable input_state GlobalInputState;
global_variable key_table GlobalKeyState;
global_variable mouse_button_table GlobalMouseButtonState;

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
    //glfwSetFramebufferSizeCallback(Window, GLFW_FramebufferSizeCallback);
    glfwSetCursorPosCallback(Window, GLFW_MouseCallback);
    glfwSetScrollCallback(Window, GLFW_MouseScrollCallback);
    glfwSetKeyCallback(Window, GLFW_KeyCallback);
    glfwSetMouseButtonCallback(Window,GLFW_MouseButtonCallback);
    
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPos(Window, WindowWidth / 2.0f, WindowHeight / 2.0f);
    
    GLenum GlewError = glewInit();
    if(GlewError == GLEW_OK)
    {
        printf("OpenGL Initialized\n");
        printf("Vendor: "); printf((char *)glGetString(GL_VENDOR)); printf("\n");
        printf("Renderer: "); printf((char *)glGetString(GL_RENDERER)); printf("\n");
        printf("Version: "); printf((char *)glGetString(GL_VERSION)); printf("\n\n");
        
        // NOTE: GLFW provides the multisample buffers by calling 
        // glfwWindowHint with (GLFW_SAMPLES, 4). Together with GL_MULTISAMPLE
        // anti-aliasing is enabled.
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        GlobalTextureCache = {};
        GameCamera = {};
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
    
    // TODO: Do I really need a shader program struct still?
    VertexShaderID = LoadAndCompileShader("shaders/blit_screen_vertex.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/blit_screen_frag.c", GL_FRAGMENT_SHADER);
    opengl_shader_program BlitShader = CreateShaderProgram(VertexShaderID, FragmentShaderID, 0);
    
    VertexShaderID = LoadAndCompileShader("shaders/hdr_pass_vertex.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/hdr_pass_frag.c", GL_FRAGMENT_SHADER);
    opengl_shader_program GammaProgram = CreateShaderProgram(VertexShaderID, FragmentShaderID, 0);
    
    VertexShaderID = LoadAndCompileShader("shaders/blit_texture_vertex.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/blit_texture_frag.c", GL_FRAGMENT_SHADER);
    opengl_shader_program BlitTextureProgram = CreateShaderProgram(VertexShaderID, FragmentShaderID, 0);
    
    VertexShaderID = LoadAndCompileShader("shaders/selection_vertex.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/selection_frag.c", GL_FRAGMENT_SHADER);
    opengl_shader_program SelectionProgram = CreateShaderProgram(VertexShaderID, FragmentShaderID, 0);
    
    VertexShaderID = LoadAndCompileShader("shaders/grid_vertex.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/grid_frag.c", GL_FRAGMENT_SHADER);
    opengl_shader_program GridShaderProgram = CreateShaderProgram(VertexShaderID, FragmentShaderID, 0);
    
    VertexShaderID = LoadAndCompileShader("shaders/default_vertex.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/default_frag.c", GL_FRAGMENT_SHADER);
    opengl_shader_program DefaultProgram = CreateShaderProgram(VertexShaderID, FragmentShaderID, 0);
    
    VertexShaderID = LoadAndCompileShader("shaders/stencil_vertex.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/stencil_frag.c", GL_FRAGMENT_SHADER);
    opengl_shader_program StencilProgram = CreateShaderProgram(VertexShaderID, FragmentShaderID, 0);
    
    texture FloorTexture = LoadTextureToLinear("textures/brickwall.jpg");
    texture FloorNormalMap = LoadTexture("textures/brickwall_normal.jpg");
    texture FloorSpecTexture = LoadTexture("textures/brickwall_grayscale.jpg");
    texture LinearTexture = LoadTextureToLinear("textures/container.jpg");
    texture GammaTexture = LoadTexture("textures/container.jpg");
    
    GlobalInputState = {};
    GlobalInputState.MousePosition.x = WindowWidth / 2;
    GlobalInputState.MousePosition.y = WindowHeight / 2;
    
    // Camera
    // TODO: Set up a camera proper camera system
    GameCamera.Position = glm::vec3(0.015581f, 1.017382f, 14.225583f); // DEBUG VALUES
    GameCamera.Orientation.Yaw = -90.0f;
    GameCamera.Orientation.Pitch = -2.8f; // DEBUG VALUE
    GameCamera.LookSpeed = 7.0f;
    GameCamera.PanSpeed = 35.0f;
    GameCamera.FieldOfView = 45.0f;
    
    editor Editor = {};
    Editor.Active = 1;
    Editor.Camera.Position = glm::vec3(0.0f); // DEBUG VALUES
    Editor.Camera.Orientation.Yaw = -90.0f;
    Editor.Camera.Orientation.Pitch = -2.8f; // DEBUG VALUE
    Editor.Camera.LookSpeed = 7.0f;
    Editor.Camera.PanSpeed = 1.0f;
    Editor.Camera.FieldOfView = 45.0f;
    
    // TODO:
    // I could go through, do the matrix multiplications, then send the MVPs to the draw buffer?
    // Instead of copying all of the entity data
    draw_buffer DrawBuffer = {};
    draw_buffer StencilDrawBuffer = {};
    
    glm::mat4 ModelMatrix;
    glm::mat4 ViewMatrix;
    glm::mat3 NormalMatrix;
    glm::mat4 ProjectionMatrix;
    glm::mat4 MVP;
    
    float Exposure = 0.5f;
    float Gamma = 2.2f;
    float NearPlane = 0.01f;
    float FarPlane = 100.0f;
    
    entity Plane;
    Plane.Position = glm::vec3(0.0f);
    Plane.Scale = glm::vec3(1.0f);
    Plane.Rotation = {0.0f, 0.0f, 0.0f};
    Plane.Primitive = QUAD;
    
    entity_group WorldEntities = {};
    PushEntityToGroup(&WorldEntities, Plane);
    
    entity_group SelectedEntities = {};
    entity_group RayCastEntities = {};
    
    while(!glfwWindowShouldClose(Window))
    {
        PollEvents(&GlobalInputState);
        UpdateClock(&GlobalInputState.Clock);
        
        if(Editor.Active)
        {
            ProcessEditorInput(&Editor, &GlobalInputState, &GlobalKeyState);
            ViewMatrix = GetCameraViewMatrix(Editor.Camera);
            ProjectionMatrix = glm::perspective(glm::radians(Editor.Camera.FieldOfView), 
                                                (float)WindowWidth / (float)WindowHeight, 
                                                NearPlane, 
                                                FarPlane);
            
            key_state LMouseButton = GlobalMouseButtonState.Buttons[LEFT_MOUSE_BUTTON];
            key_state RMouseButton = GlobalMouseButtonState.Buttons[RIGHT_MOUSE_BUTTON];
            if(Editor.MouseButtons[LEFT_MOUSE_BUTTON])
            {
                ClearEntityGroup(&SelectedEntities);
                // NOTE: Check intersection of mouse cursor and world objects
                // NOTE: The mouse button state changes to DRAG if there's any movement while
                // pressing down the LMouseButton. This makes it feel like we're losing clicks
                // TODO: Maybe we should keep around a frustum culled group of visible entities
                ray MouseRay = {};
                MouseRay.Origin = Editor.Camera.Position;
                MouseRay.Direction = ScreenToWorldRay(GlobalInputState.ClickPosition, ProjectionMatrix, ViewMatrix);
                
                // TODO: Check all (visible) entities
                entity EntityInView;
                EntityInView.Position = glm::vec3(0.0f);
                EntityInView.Scale = glm::vec3(1.0f);
                EntityInView.Rotation = {0.0f, 0.0f, 0.0f};
                EntityInView.Primitive = QUAD;
                
                plane Plane = CreatePlane(EntityInView.Position, EntityInView.Rotation);
                
                glm::vec3 Intersection;
                bool Collision = RayPlaneIntersection(MouseRay, Plane, &Intersection);
                if(Collision)
                {
                    // TODO: Check boundary of plane
                    entity RayCastEntity = {};
                    RayCastEntity.Primitive = CUBE;
                    RayCastEntity.Position = Intersection;
                    RayCastEntity.Scale = glm::vec3(0.10);
                    PushEntityToGroup(&RayCastEntities, RayCastEntity);
                }
                
            }
            else if(GlobalMouseButtonState.Buttons[LEFT_MOUSE_BUTTON] == DRAG)
            {
                rect SelectionRegion = CreateRectFromDiagonalPoints(GlobalInputState.ClickPosition,
                                                                    GlobalInputState.MousePosition);
                
                // Should we point to the entities or copy their data entirely?
                SelectEntitiesInScreenRegion(&SelectedEntities, &WorldEntities, ViewMatrix,
                                             ProjectionMatrix, SelectionRegion);
            }
            
            if(Editor.MouseButtons[RIGHT_MOUSE_BUTTON])
            {
                ClearEntityGroup(&RayCastEntities);
            }
        }
        else
        {
            // ProcessGameInputAndSimulate
            ViewMatrix = GetCameraViewMatrix(GameCamera);
            ProjectionMatrix = glm::perspective(glm::radians(GameCamera.FieldOfView), 
                                                (float)WindowWidth / (float)WindowHeight, 
                                                NearPlane, 
                                                FarPlane);
        }
        
        QueueEntityGroupForRendering(&SelectedEntities, &StencilDrawBuffer);
        QueueEntityGroupForRendering(&RayCastEntities, &DrawBuffer);
        
        glBindFramebuffer(GL_FRAMEBUFFER, HDR_RenderTarget.FrameBuffer);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        // TODO: Implement stencil buffer drawing correctly
        RenderDrawBuffer(&StencilDrawBuffer, StencilProgram.Id, ProjectionMatrix, ViewMatrix, true);
        RenderDrawBuffer(&DrawBuffer, DefaultProgram.Id, ProjectionMatrix, ViewMatrix, false);
        
#if 1
        // Scene
        SetUniform3fv(ShadowProgram.Id, "lightPos", Editor.Camera.Position);
        SetUniform3fv(ShadowProgram.Id, "viewPos", Editor.Camera.Position);
        SetUniformMatrix4fv(ShadowProgram.Id, "projection", ProjectionMatrix);
        SetUniformMatrix4fv(ShadowProgram.Id, "view", ViewMatrix);
        
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.0f));
        NormalMatrix = glm::transpose(glm::inverse(glm::mat3(ModelMatrix)));
        SetUniformMatrix3fv(ShadowProgram.Id, "normalMatrix", NormalMatrix);
        SetUniformMatrix4fv(ShadowProgram.Id, "model", ModelMatrix);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, FloorTexture.Id);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, FloorNormalMap.Id);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, FloorSpecTexture.Id);
        RenderQuad();
#endif
        
        DrawWorldGrid(GridShaderProgram.Id, ProjectionMatrix, ViewMatrix, NearPlane, FarPlane);
        
        // HDR and Gamma Correction pass
        glBindFramebuffer(GL_FRAMEBUFFER, PFX_RenderTarget.FrameBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(BlitShader.Id);
        //SetUniform1f(GammaProgram.Id, "exposure", Exposure);
        //SetUniform1f(GammaProgram.Id, "gamma", Gamma);
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
        
        // UI
        if(Editor.Active && GlobalMouseButtonState.Buttons[LEFT_MOUSE_BUTTON] == DRAG)
        {
            // TODO: Add manipulators.
            rect SelectionRegion = CreateRectFromDiagonalPoints(GlobalInputState.MousePosition, GlobalInputState.ClickPosition);
            
            DrawSelectionRegion(SelectionProgram.Id,
                                SelectionRegion.X,
                                SelectionRegion.Y,
                                SelectionRegion.Width,
                                SelectionRegion.Height);
        }
        
        
        glfwSwapBuffers(Window);
        OpenGL_OutputErrorQueue();
    }
    
    return 0;
}

internal void 
GLFW_FramebufferSizeCallback(GLFWwindow *Window, int Width, int Height)
{
    // TODO: Resize framebuffers
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
    int WindowWidth, WindowHeight;
    glfwGetWindowSize(Window, &WindowWidth, &WindowHeight);
    RegisterMouseMovement(&GlobalInputState, &GlobalMouseButtonState, XPos, WindowHeight - YPos);
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
