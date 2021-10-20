#include "windows.h"
#include "include/GL/glew.h"
#define IMGUI_IMPLEMENTATION
#include "include/imgui/imgui_single_file.h"
#include "include/GLFW/glfw3.h"
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "include/qpc/qpc.h"
#include "types.h"
#include "utility.h"
#include "opengl_code.h"
#include "renderer.h"

#include "input.cpp"
#include "camera.cpp"
#include "opengl_code.cpp"
#include "renderer.cpp"
#include "board_processing.cpp"

// NOTE: If the object's texture has an unexpected color or is black, verify that the in/out 
// varibles match across the shader program. This is NOT a link error; There are no error 
// messages for this bug.

internal void ToggleUI(GLFWwindow *Window, bool *StatusUI);
internal void GLFW_ErrorCallback(int Error, const char *Description);
internal void GLFW_KeyCallback(GLFWwindow *Window, int Key, int Scancode, int Action, int Mods);
internal void GLFW_FramebufferSizeCallback(GLFWwindow *, int, int);
internal void GLFW_MouseCallback(GLFWwindow *Window, double XPos, double YPos);
internal void GLFW_MouseScrollCallback(GLFWwindow *Window, double XOffset, double YOffset);
internal void TransparencyDepthSort(glm::vec3 *Array, int ArrayCount, glm::vec3 CameraPosition);
internal void DebugRenderCube(void);

global_variable input_state GlobalInputState;
global_variable board_render_config DrawConfig;
global_variable camera Camera;

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
    
    //glfwWindowHint(GLFW_SAMPLES, 4); // NOTE: Multsample buffer for MSAA, 4 samples per pixel
    QPC_StartCounter();
    GLFWwindow *Window = glfwCreateWindow(WindowWidth, WindowHeight, "Test window", NULL, NULL);
    glfwMakeContextCurrent(Window);
    //glfwSetFramebufferSizeCallback(Window, GLFW_FramebufferSizeCallback);
    //glfwSetScrollCallback(Window, GLFW_MouseScrollCallback);
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(Window, GLFW_KeyCallback);
    glfwSetCursorPosCallback(Window, GLFW_MouseCallback);
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
        
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
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
    
    // ImGui Initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& ImGuiIO = ImGui::GetIO(); (void)ImGuiIO;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(Window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGuiIO.Fonts->AddFontDefault();
    
    if(ASSERTIONS_ENABLED)
    {
        printf("INFO: Assertions turned ON\n");
    }
    else
    {
        printf("INFO: Assertions turned OFF\n");
    }
    
    int ProfileCount = MAX_NUM_LASERS + 1;
    int SamplesPerProfile = MAX_NUM_CNTS_PER_LUG;
    int MaxVertexCount = ProfileCount * SamplesPerProfile;
    vertex_buffer GlobalVertexStorage = CreateVertexBuffer(MaxVertexCount);
    
    int VertexBufferSize = GlobalVertexStorage.ElementCapacity * sizeof(v6);
    unsigned int ProfilePointsVAO, ProfilePointsVBO;
    glGenVertexArrays(1, &ProfilePointsVAO);
    glGenBuffers(1, &ProfilePointsVBO);
    glBindVertexArray(ProfilePointsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, ProfilePointsVBO);
    glBufferData(GL_ARRAY_BUFFER, VertexBufferSize, 0, GL_STATIC_DRAW);  
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    
    SBoardData *BoardData = (SBoardData *)ReadFile("board_data/Trimmer/Hermary/Vision/board1.brd");
    
    // TODO: Use opengl to copy the sub-image
    u32 TopImageWidth = BoardData->vs_image_data.sTopImageData.width;
    u32 TopImageHeight = BoardData->vs_image_data.sTopImageData.height;
    u32 *TopImagePixels = (u32 *)malloc(sizeof(u32) * TopImageWidth * TopImageHeight);
    for(u32 Row = 0; Row < TopImageHeight; ++Row)
    {
        for(u32 Col = 0; Col < TopImageWidth; ++Col)
        {
            TopImagePixels[Row * TopImageWidth + Col] = BoardData->vs_image_data.sTopImageData.Pixels[Row][Col];
        }
    }
    texture_unit TopBoardTexture = LoadTextureU32(TopImagePixels, TopImageWidth, TopImageHeight, 4);
    
    
    int BottomImageWidth = BoardData->vs_image_data.sBottomImageData.width;
    int BottomImageHeight = BoardData->vs_image_data.sBottomImageData.height;
    u32 *BottomImagePixels = (u32 *)malloc(sizeof(u32) * BottomImageWidth * BottomImageHeight);
    for(u32 Row = 0; Row < TopImageHeight; ++Row)
    {
        for(u32 Col = 0; Col < TopImageWidth; ++Col)
        {
            BottomImagePixels[Row * BottomImageWidth + Col] = BoardData->vs_image_data.sBottomImageData.Pixels[Row][Col];
        }
    }
    texture_unit BottomBoardTexture = LoadTextureU32(BottomImagePixels, BottomImageWidth, BottomImageHeight, 4);
    
    board_color_segments ColorSegments = {};
    ColorSegments.vne = {1.0f, 0.0f, 0.0f};
    ColorSegments.vne_1 = {1.0f, 0.65f, 0.0f};
    ColorSegments.thick = {0.0f, 1.0f, 0.0f};
    ColorSegments.scant = {1.0f, 1.0f, 0.0f};
    
#if 0
    QPC_StartCounter();
    ProcessProfileData(&GlobalVertexStorage, BoardData, 8, ColorSegments);
    UploadBufferedVertices(GlobalVertexStorage, ProfilePointsVAO);
    QPC_EndCounterPrint("Vertices processed");
#endif
    
    unsigned int VertexShaderID;
    unsigned int FragmentShaderID;
    unsigned int GeometryShaderID;
    VertexShaderID = LoadAndCompileShader("shaders/board_vertex.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/board_frag.c", GL_FRAGMENT_SHADER);
    opengl_shader_program BoardProgram = CreateShaderProgram(VertexShaderID, FragmentShaderID, 0);
    
    VertexShaderID = LoadAndCompileShader("shaders/default_vertex.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/default_frag.c", GL_FRAGMENT_SHADER);
    opengl_shader_program DefaultProgram = CreateShaderProgram(VertexShaderID, FragmentShaderID, 0);
    
    float SecondsElapsed;
    float PrevTime = 0;
    float FrameTime;
    
    GlobalInputState = {};
    GlobalInputState.PrevMousePos.x = WindowWidth / 2;
    GlobalInputState.PrevMousePos.y = WindowHeight / 2;
    
    // Camera
    // TODO: Set up a camera proper camera system
    Camera.Position = glm::vec3(1.49f, 1.40f, 2.80f);
    Camera.Front = glm::vec3(0.0f, 0.0f, -1.0f);
    Camera.Up = glm::vec3(0.0f, 1.0f, 0.0f);
    Camera.Orientation.Yaw = -90.0f;
    Camera.LookSpeed = 0.1f;
    Camera.PanSpeed = 0.3f;
    Camera.FieldOfView = 45.0f;
    Camera.Mode = FLY_MODE;
    
    point_light PointLight;
    PointLight.Position = glm::vec3(0.0f);
    
    glm::mat4 ModelMatrix;
    glm::mat4 ViewMatrix;
    glm::mat4 ViewSubMatrix;
    glm::mat4 ProjectionMatrix;
    glm::mat4 MVP;
    
    bool DrawUI = false; // Whether the UI will be drawn or not
    bool ShowDemoWindow = true;
    bool ShowAnotherWindow = false;
    float ClearColor[4] = {};
    while(!glfwWindowShouldClose(Window))
    {
        glfwPollEvents();
        
        SecondsElapsed = glfwGetTime();
        FrameTime = SecondsElapsed - PrevTime;
        PrevTime = SecondsElapsed;
        
        if(Camera.Mode == FLY_MODE)
            UpdateCamera(Window, &Camera, FrameTime);
        
        // TODO: Remove keyboard polling from UpdateCamera
        input_command *Command;
        while(Command = PopCommand(&GlobalInputState))
        {
            // TODO: I could dispatch from here to different places. If we're in a fly camera view
            // I would want wasd keys to go to the move camera function or something...
            if(Command->Key == GLFW_KEY_GRAVE_ACCENT && Command->Action == GLFW_PRESS)
            {
                ToggleUI(Window, &DrawUI);
            }
        }
        
        
        ViewMatrix = GetViewMatrix(Camera);
        ViewSubMatrix = glm::mat4(glm::mat3(ViewMatrix));
        ProjectionMatrix = glm::perspective(glm::radians(Camera.FieldOfView), 
                                            WindowWidth / WindowHeight, 
                                            0.1f, 
                                            1000.0f);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TopBoardTexture.Id);
        
        MVP = ProjectionMatrix * ViewMatrix;
        SetUniformMatrix4fv(DefaultProgram.Id, "mvp", MVP);
        RenderCube();
        
        glBindTexture(GL_TEXTURE_2D, BottomBoardTexture.Id);
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(3, 0, 0));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        SetUniformMatrix4fv(DefaultProgram.Id, "mvp", MVP);
        RenderCube();
        
        OutputErrorQueue();
        
        if(DrawUI)
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            
            if(ShowDemoWindow) ImGui::ShowDemoWindow(&ShowDemoWindow);
            
            {
                static int clicked;
                ImGui::Begin("Color Segments");
                ImGui::ColorEdit3("scant", &ColorSegments.scant.x);
                ImGui::ColorEdit3("thick", &ColorSegments.thick.x);
                ImGui::ColorEdit3("vne_1", &ColorSegments.vne_1.x);
                ImGui::ColorEdit3("vne", &ColorSegments.vne.x);
                if(ImGui::Button("Apply"))
                    clicked++;
                if(clicked & 1)
                {
                    QPC_StartCounter();
                    ClearVertexBuffer(&GlobalVertexStorage);
                    ProcessProfileData(&GlobalVertexStorage, BoardData, 8, ColorSegments);
                    UploadBufferedVertices(GlobalVertexStorage, ProfilePointsVAO);
                    long long MilisecondsElapsed = QPC_EndCounter() / 1000.0l;
                    ImGui::SameLine();
                    ImGui::Text("%Lf\n", MilisecondsElapsed);
                    clicked = 0;
                }
                
                ImGui::End();
            }
            
            ImGui::Render();
            int DisplayWidth, DisplayHeight;
            glfwGetFramebufferSize(Window, &DisplayWidth, &DisplayHeight);
            glViewport(0, 0, DisplayWidth, DisplayHeight);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        
        glfwSwapBuffers(Window);
    }
    
    return 0;
}

struct transparent_sort_object
{
    float Length;
    int Index;
};

void GLFW_FramebufferSizeCallback(GLFWwindow *Window, int Width, int Height)
{
    glViewport(0, 0, Width, Height);
}

void GLFW_MouseScrollCallback(GLFWwindow *Window, double XOffset, double YOffset)
{
    // TODO: Split this into another input handling system
    Camera.FieldOfView -= (float)YOffset;
    if(Camera.FieldOfView < 1.0f)
        Camera.FieldOfView = 1;
    if(Camera.FieldOfView > 45.0f)
        Camera.FieldOfView = 45.0f;
}

void GLFW_MouseCallback(GLFWwindow *Window, double XPos, double YPos)
{
    // TODO: Split this into another input handling system
    
    if(Camera.Mode == STATIC_MODE)
        return;
    
    float XOffset = (XPos - GlobalInputState.PrevMousePos.x) * Camera.LookSpeed;
    float YOffset = (GlobalInputState.PrevMousePos.y - YPos) * Camera.LookSpeed;
    
    GlobalInputState.PrevMousePos.x = XPos;
    GlobalInputState.PrevMousePos.y = YPos;
    
    Camera.Orientation.Yaw += XOffset;
    Camera.Orientation.Pitch += YOffset;
    
    if (Camera.Orientation.Pitch > 89.0f) 
        Camera.Orientation.Pitch = 89.0f;
    if (Camera.Orientation.Pitch < -89.0f)
        Camera.Orientation.Pitch = -89.0f;
}

internal void
GLFW_ErrorCallback(int Error, const char *Description)
{
    fprintf(stderr, "ERROR: (GLFW: %d) %s.\n", Error, Description);
}

internal void 
GLFW_KeyCallback(GLFWwindow *Window, int Key, int Scancode, int Action, int Mods)
{
    ProcessKeyboardInput(&GlobalInputState, Key, Scancode, Action, Mods);
}

internal void
ToggleUI(GLFWwindow *Window, bool *DrawUI)
{
    *DrawUI ^= 1;
    if(*DrawUI)
    {
        glfwSetCursorPosCallback(Window, 0);
        glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        
        int WindowWidth = 0, WindowHeight = 0;
        glfwGetWindowSize(Window, &WindowWidth, &WindowHeight);
        glfwSetCursorPos(Window, WindowWidth / 2.0f, WindowHeight / 2.0f);
    }
    else
    {
        glfwSetCursorPosCallback(Window, GLFW_MouseCallback);
        glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPos(Window, GlobalInputState.PrevMousePos.x, GlobalInputState.PrevMousePos.y);
    }
}