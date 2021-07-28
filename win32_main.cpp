#define GLEW_STATIC
#include "include/GL/glew.h"
#include <windows.h>
#include <stdint.h>
#include "types.h"
#include "win32_main.h"

#include "include/GL/glew.c"
#include "opengl_code.cpp"

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

global_variable bool GlobalRunning;
global_variable win32_offscreen_buffer GlobalBackbuffer;
global_variable win32_offscreen_buffer GlobalZBuffer;

internal void
ProcessKeyboardInput(uint8 *GameInputButton, bool IsDown, bool WasDown)
{
    if(WasDown && !IsDown)
    {
        *GameInputButton = 0;
        return;
    }
    else if(!WasDown && IsDown)
    {
        *GameInputButton = 1;
    }
}

internal void
ProcessMessages(game_input *GameInput)
{
    MSG Message;
    
    while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
    {
        if(Message.message == WM_QUIT)
        {
            GlobalRunning = false;
        }
        
        switch(Message.message)
        {
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP:
            {
                uint32 VKCode = Message.wParam;
                bool WasDown = ((Message.lParam & (1 << 30)) != 0);
                bool IsDown = ((Message.lParam & (1 << 31)) == 0);
                if(WasDown != IsDown)
                {
                    if(VKCode == 'W')
                    {
                        // up
                        ProcessKeyboardInput(&(*GameInput).Up, IsDown, WasDown);
                    }
                    else if(VKCode == 'A')
                    {
                        // left
                        ProcessKeyboardInput(&(*GameInput).Left, IsDown, WasDown); 
                    }
                    else if(VKCode == 'S')
                    {
                        // down
                        ProcessKeyboardInput(&(*GameInput).Down, IsDown, WasDown); 
                    }
                    else if(VKCode == 'D')
                    {
                        // right
                        ProcessKeyboardInput(&(*GameInput).Right, IsDown, WasDown); 
                    }
                    else if(VKCode == VK_SPACE)
                    {
                        // shoot
                        ProcessKeyboardInput(&(*GameInput).Shoot, IsDown, WasDown); 
                    }
                    else if(VKCode == 'Q')
                    {
                    }
                    else if(VKCode == 'E')
                    {
                    }
                    else if(VKCode == VK_UP)
                    {
                        ProcessKeyboardInput(&(*GameInput).Up, IsDown, WasDown);
                    }
                    else if(VKCode == VK_LEFT)
                    {
                        ProcessKeyboardInput(&(*GameInput).Left, IsDown, WasDown);
                    }
                    else if(VKCode == VK_DOWN)
                    {
                        ProcessKeyboardInput(&(*GameInput).Down, IsDown, WasDown);
                    }
                    else if(VKCode == VK_RIGHT)
                    {
                        ProcessKeyboardInput(&(*GameInput).Right, IsDown, WasDown);
                    }
                    else if(VKCode == VK_ESCAPE)
                    {
                    }
                    else if((GetKeyState(VK_MENU) & 0x8000) && VKCode == VK_F4)
                    {
                        GlobalRunning = false;
                    }
                }
            } break;
        }
        
        TranslateMessage(&Message);
        DispatchMessageA(&Message);
    }
    
}

internal win32_window_dimension
Win32GetWindowDimension(HWND Window)
{
    win32_window_dimension Result;
    
    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    Result.Width = ClientRect.right - ClientRect.left;
    Result.Height = ClientRect.bottom - ClientRect.top;
    
    return(Result);
}

internal void
Win32ResizeDIBSection(win32_offscreen_buffer *Buffer, int Width, int Height)
{
    // TODO(casey): Bulletproof this.
    // Maybe don't free first, free after, then free first if that fails.
    
    if(Buffer->Memory)
    {
        VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
    }
    
    Buffer->Width = Width;
    Buffer->Height = Height;
    
    int BytesPerPixel = 4;
    
    // NOTE(casey): When the biHeight field is negative, this is the clue to
    // Windows to treat this bitmap as top-down, not bottom-up, meaning that
    // the first three bytes of the image are the color for the top left pixel
    // in the bitmap, not the bottom left!
    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biWidth = Buffer->Width;
    Buffer->Info.bmiHeader.biHeight = -Buffer->Height;
    Buffer->Info.bmiHeader.biPlanes = 1;
    Buffer->Info.bmiHeader.biBitCount = 32;
    Buffer->Info.bmiHeader.biCompression = BI_RGB;
    
    // NOTE(casey): Thank you to Chris Hecker of Spy Party fame
    // for clarifying the deal with StretchDIBits and BitBlt!
    // No more DC for us.
    int BitmapMemorySize = (Buffer->Width*Buffer->Height)*BytesPerPixel;
    Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
    Buffer->Pitch = Width*BytesPerPixel;
}

internal void
Win32DisplayBufferInWindow(win32_offscreen_buffer *Buffer,
                           HDC DeviceContext, int WindowWidth, int WindowHeight)
{
    // TODO(casey): Aspect ratio correction
    // TODO(casey): Play with stretch modes
    StretchDIBits(DeviceContext,
                  /*
                  X, Y, Width, Height,
                  X, Y, Width, Height,
                  */
                  0, 0, WindowWidth, WindowHeight,
                  0, 0, Buffer->Width, Buffer->Height,
                  Buffer->Memory,
                  &Buffer->Info,
                  DIB_RGB_COLORS, SRCCOPY);
    
    int BytesPerPixel = 4;
    int BitmapMemorySize = (Buffer->Width * Buffer->Height) * BytesPerPixel;
    
    memset(Buffer->Memory, 0, BitmapMemorySize);
}

internal LRESULT CALLBACK
Win32MainWindowCallback(HWND Window,
                        UINT Message,
                        WPARAM WParam,
                        LPARAM LParam)
{       
    LRESULT Result = 0;
    
    switch(Message)
    {
        case WM_CLOSE:
        {
            // TODO(casey): Handle this with a message to the user?
            GlobalRunning = false;
        } break;
        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;
        case WM_DESTROY:
        {
            // TODO(casey): Handle this as an error - recreate window?
            GlobalRunning = false;
        } break;
        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            win32_window_dimension Dimension = Win32GetWindowDimension(Window);
            Win32DisplayBufferInWindow(&GlobalBackbuffer, DeviceContext,
                                       Dimension.Width, Dimension.Height);
            EndPaint(Window, &Paint);
        } break;
        default:
        {
            Result = DefWindowProc(Window, Message, WParam, LParam);
        } break;
    }
    
    return(Result);
}


int CALLBACK
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR CommandLine,
        int ShowCode)
{
    WNDCLASSA WindowClass = {};
    
    Win32ResizeDIBSection(&GlobalBackbuffer, 1280, 720);
    //Win32ResizeDIBSection(&GlobalZBuffer, 1280, 720);
    
    
    WindowClass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = Instance;
    //    WindowClass.hIcon;
    WindowClass.lpszClassName = "HandmadeHeroWindowClass";
    
    if(RegisterClassA(&WindowClass))
    {
        
        HWND Window =
            CreateWindowExA(0,
                            WindowClass.lpszClassName,
                            "Handmade Hero",
                            WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            0,
                            0,
                            Instance,
                            0);
        
        
        if(Window)
        {
            // NOTE(casey): Since we specified CS_OWNDC, we can just
            // get one device context and use it forever because we
            // are not sharing it with anyone.
            HDC DeviceContext = GetDC(Window);
            
            
            game_memory GameMemory = {};
            GameMemory.PermanentStorageSize = Megabytes(64);
            GameMemory.PermanentStorage = VirtualAlloc(0, GameMemory.PermanentStorageSize,
                                                       MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
            
            game_input GameInput = {};
            
            opengl_buffer VertexBuffer = InitializeOpenglBuffer(Vertices, ArrayCount(Vertices), 0, 0);
            opengl_shader VertexShader = CompileShaderSource(VertexShaderSource, VERTEX);
            opengl_shader FragmentShader = CompileShaderSource(FragmentShaderSource, FRAGMENT);
            opengl_shader_program Program = CreateShaderProgram(VertexShader, FragmentShader);
            glViewport(0, 0, 1280, 720);
            
            glUseProgram(Program.Id);
            
            GlobalRunning = true;
            
            while(GlobalRunning)
            {
                ProcessMessages(&GameInput);
            }
        }
        else
        {
            // TODO(casey): Logging
        }
    }
    else
    {
        // TODO(casey): Logging
    }
    
    return(0);
}
