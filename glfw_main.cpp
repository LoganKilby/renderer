#include "include/GL/glew.h"
#include "include/GLFW/glfw3.h"
#define IMGUI_IMPLEMENTATION
#include "include/imgui/imgui_single_file.h"
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
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

static void ToggleUI(GLFWwindow *Window, bool *StatusUI);
static void GLFW_ErrorCallback(int Error, const char *Description);
static void GLFW_KeyCallback(GLFWwindow *Window, int Key, int Scancode, int Action, int Mods);
static void GLFW_FramebufferSizeCallback(GLFWwindow *, int, int);
static void GLFW_MouseCallback(GLFWwindow *Window, double XPos, double YPos);
static void GLFW_MouseScrollCallback(GLFWwindow *Window, double XOffset, double YOffset);

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
    
    //board_data/Trimmer/Hermary/Vision/board1.brd
    SBoardData *BoardData = (SBoardData *)ReadFile("board_data/Trimmer/JS50/board44.brd");
    
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
    
    QPC_StartCounter();
    GLFWwindow *Window = glfwCreateWindow((int)WindowWidth, (int)WindowHeight, "Test window", NULL, NULL);
    glfwMakeContextCurrent(Window);
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
        glPointSize(1.5);
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
    
    
    
    // TODO: Use opengl to copy the sub-image?
    u32 MaxImageWidth = 5760;
    u32 MaxImageHeight = 440;
    
    u32 BottomWidth = BoardData->vs_image_data.sBottomImageData.width;
    u32 BottomHeight = BoardData->vs_image_data.sBottomImageData.height;
    texture_unit BottomBoardTexture = LoadTexture2D(&BoardData->vs_image_data.sBottomImageData.Pixels[0][0], MaxImageWidth, MaxImageHeight, 4, GL_BGRA, GL_UNSIGNED_BYTE);
    
    u32 TopWidth = BoardData->vs_image_data.sTopImageData.width;
    u32 TopHeight = BoardData->vs_image_data.sTopImageData.height;
    texture_unit TopBoardTexture = LoadTexture2D(&BoardData->vs_image_data.sTopImageData.Pixels[0][0], MaxImageWidth, MaxImageHeight, 4, GL_BGRA, GL_UNSIGNED_BYTE);
    
    board_color_segments ColorSegments = {};
    ColorSegments.vne = {1.0f, 0.0f, 0.0f};
    ColorSegments.vne_1 = {1.0f, 0.65f, 0.0f};
    ColorSegments.thick = {0.0f, 1.0f, 0.0f};
    ColorSegments.scant = {1.0f, 1.0f, 0.0f};
    
    QPC_StartCounter();
    // NOTE: Points are representative of the "top texture"
    vertex_buffer TopVertexData = ProcessProfileData(BoardData, 8, ColorSegments);
    f32 PixelsPerInch = 20;
    f32 XRangeInInches = (MaxImageWidth / 20) * 1000;
    f32 YRangeInInches = (MaxImageHeight / 20) * 1000;
    CreateTextureCoordinates(&TopVertexData, XRangeInInches, YRangeInInches);
    //vertex_buffer TopVertexData = ExtractJS50Raw(BoardData);
    QPC_EndCounterPrint("Vertices processed");
    printf("mb allocated: %.3f\n", (f32)(2 * TopVertexData.Capacity * sizeof(v3) + TopVertexData.Capacity * sizeof(v2)) / 1000000.0f);
    
    glGenVertexArrays(1, &TopVertexData.VAO);
    glGenBuffers(1, &TopVertexData.VBO);
    glBindVertexArray(TopVertexData.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, TopVertexData.VBO);
    glBufferData(GL_ARRAY_BUFFER, TopVertexData.Capacity * sizeof(vertex_attributes), TopVertexData.Attributes, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_attributes), (void*)offsetof(vertex_attributes, Point));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_attributes), (void*)offsetof(vertex_attributes, Color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_attributes), (void*)offsetof(vertex_attributes, TexCoords));
    glBindVertexArray(0);
    
    //UploadBufferedVertices(&TopVertexData);
    
    unsigned int VertexShaderID;
    unsigned int FragmentShaderID;
    VertexShaderID = LoadAndCompileShader("shaders/board_vertex.c", GL_VERTEX_SHADER);
    FragmentShaderID = LoadAndCompileShader("shaders/board_frag.c", GL_FRAGMENT_SHADER);
    opengl_shader_program BoardProgram = CreateShaderProgram(VertexShaderID, FragmentShaderID, 0);
    SetUniform1i(BoardProgram.Id, "scannerImage", 0);
    
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
    Camera.PanSpeed = 1.0f;
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
        
        SecondsElapsed = (f32)glfwGetTime();
        FrameTime = SecondsElapsed - PrevTime;
        PrevTime = SecondsElapsed;
        
        if(Camera.Mode == FLY_MODE)
            UpdateCamera(Window, &Camera, FrameTime);
        
        // TODO: Remove keyboard polling from UpdateCamera
        input_command *Command = PopCommand(&GlobalInputState);;
        while(Command)
        {
            // TODO: I could dispatch from here to different places. If we're in a fly camera view
            // I would want wasd keys to go to the move camera function or something...
            if(Command->Key == GLFW_KEY_GRAVE_ACCENT && Command->Action == GLFW_PRESS)
            {
                ToggleUI(Window, &DrawUI);
            }
            
            Command = PopCommand(&GlobalInputState);
        }
        
        ViewMatrix = GetViewMatrix(Camera);
        ViewSubMatrix = glm::mat4(glm::mat3(ViewMatrix));
        ProjectionMatrix = glm::perspective(glm::radians(Camera.FieldOfView), 
                                            WindowWidth / WindowHeight, 
                                            0.1f, 
                                            1000.0f);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.00005f, 0.00005f, 0.00005f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        SetUniformMatrix4fv(BoardProgram.Id, "mvp", MVP);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TopBoardTexture.Id);
        glBindVertexArray(TopVertexData.VAO);
        glDrawArrays(GL_POINTS, 0, TopVertexData.Count);
        glBindVertexArray(0);
        
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-5, 0, 0));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        SetUniformMatrix4fv(DefaultProgram.Id, "mvp", MVP);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TopBoardTexture.Id);
        RenderCube();
        
        ModelMatrix = glm::mat4(1.0f);
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        SetUniformMatrix4fv(DefaultProgram.Id, "mvp", MVP);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TopBoardTexture.Id);
        //RenderQuad();
        
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(5, 0, 0));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        SetUniformMatrix4fv(DefaultProgram.Id, "mvp", MVP);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, BottomBoardTexture.Id);
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
#if 0 // TODO Just clear color attribute and recalculate that
                    ClearVertexBuffer(&TopVertexData);
                    ProcessProfileData(&TopVertexData, BoardData, 8, ColorSegments);
                    V3UpdateBufferObject(TopColorsVBO, TopVertexData.ElementCount * sizeof(v3), TopVertexData.ColorAttribute);
#endif
                    f64 MilisecondsElapsed = QPC_EndCounter() / (f64)1000;
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
    
    float XOffset = (f32)(XPos - GlobalInputState.PrevMousePos.x) * Camera.LookSpeed;
    float YOffset = (f32)(GlobalInputState.PrevMousePos.y - YPos) * Camera.LookSpeed;
    
    GlobalInputState.PrevMousePos.x = (f32)XPos;
    GlobalInputState.PrevMousePos.y = (f32)YPos;
    
    Camera.Orientation.Yaw += XOffset;
    Camera.Orientation.Pitch += YOffset;
    
    if (Camera.Orientation.Pitch > 89.0f) 
        Camera.Orientation.Pitch = 89.0f;
    if (Camera.Orientation.Pitch < -89.0f)
        Camera.Orientation.Pitch = -89.0f;
}

static void
GLFW_ErrorCallback(int Error, const char *Description)
{
    fprintf(stderr, "ERROR: (GLFW: %d) %s.\n", Error, Description);
}

static void 
GLFW_KeyCallback(GLFWwindow *Window, int Key, int Scancode, int Action, int Mods)
{
    ProcessKeyboardInput(&GlobalInputState, Key, Scancode, Action, Mods);
}

static void
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