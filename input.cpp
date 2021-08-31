// Input handling
#include "input.h"

inline void
PushInputCommand(input_command_buffer *Buffer, input_command Command)
{
    Assert(Buffer->Count < ArrayCount(Buffer->Commands));
    Buffer->Commands[Buffer->Count++] = Command;
}

internal void
RegisterKeyboardInput(input_state *Input, key_table *KeyTable, 
                      int Key, int Scancode, int Action, int Mods)
{
    if(Action == GLFW_KEY_UNKNOWN)
        return;
    
    key_state *KeyState = &KeyTable->Keys[Key];
    input_command Command;
    switch(Action)
    {
        // NOTE: GLFW key actions can be GLFW_PRESS, GLFW_RELEASE, or GLFW_REPEAT
        case GLFW_PRESS:
        {
            *KeyState = PRESSED;
            if(*KeyState == NEUTRAL)
            {
                Command.Key = Key;
                Command.Action = PRESSED;
                Command.Mods = Mods;
                Command.Device = KEYBOARD;
                PushInputCommand(&Input->CommandBuffer, Command);
            }
        } break;
        
        case GLFW_RELEASE:
        {
            *KeyState = RELEASED;
            Command.Key = Key;
            Command.Action = RELEASED;
            Command.Mods = Mods;
            Command.Device = KEYBOARD;
            PushInputCommand(&Input->CommandBuffer, Command);
        } break;
        
        case GLFW_REPEAT:
        {
            *KeyState = REPEAT;
            Command.Key = Key;
            Command.Action = REPEAT;
            Command.Mods = Mods;
            Command.Device = KEYBOARD;
            PushInputCommand(&Input->CommandBuffer, Command);
        } break;
    }
}

internal void
RegisterMouseButtonInput(input_state *Input, key_table *KeyTable, int Button, int Action, int Mods)
{
    key_state *KeyState = &KeyTable->Keys[Button];
    input_command Command;
    switch(Action)
    {
        // NOTE: GLFW mouse button actions can be GLFW_PRESS or GLFW_RELEASE
        case GLFW_PRESS:
        {
            *KeyState = PRESSED;
            if(*KeyState == NEUTRAL)
            {
                Command.Key = Button;
                Command.Action = PRESSED;
                Command.Mods = Mods;
                Command.Device = MOUSE;
                PushInputCommand(&Input->CommandBuffer, Command);
            }
        } break;
        
        case GLFW_RELEASE:
        {
            *KeyState = RELEASED;
            Command.Key = Button;
            Command.Action = RELEASED;
            Command.Mods = Mods;
            Command.Device = MOUSE;
            PushInputCommand(&Input->CommandBuffer, Command);
        } break;
    }
}

internal void
RegisterMouseMovement(input_state *Input, double XPos, double YPos)
{
    // NOTE: Idk if we need to track whether the mouse was moved as a boolean state
    // or if we can just keep track of the offset from the previous frame and that's enough
    glm::vec2 PrevMousePos = Input->PrevMousePos;
    if(XPos != PrevMousePos.x || YPos != PrevMousePos.y)
        Input->MouseMovedThisFrame = 1;
    else
        Input->MouseMovedThisFrame = 0;
    
    float XOffset = XPos - PrevMousePos.x;
    float YOffset = YPos - PrevMousePos.y;
    
    Input->PrevMousePos.x = XPos;
    Input->PrevMousePos.y = YPos;
    Input->MouseFrameMovement.x = XOffset;
    Input->MouseFrameMovement.y = YOffset;
    
    // Compute new camera yaw and pitch 
    //float YawOffset = (XPos - GlobalInputState.PrevMousePos.x) * PrimaryCamera.LookSpeed;
    //float PitchOffset = (GlobalInputState.PrevMousePos.y - YPos) * PrimaryCamera.LookSpeed;
}

internal void
UpdateClock(input_state *Input)
{
    float SecondsElapsed = glfwGetTime();
    Input->dt = SecondsElapsed - Input->SecondsElapsed;
    Input->SecondsElapsed = SecondsElapsed;
    Input->FPS = 1000.0f / Input->dt;
}

internal void
PopInputCommand(key_table *KeyTable, input_command_buffer *Buffer)
{
    if(Buffer->Count)
    {
        input_command KeyCommand = Buffer->Commands[Buffer->Count - 1];
        Buffer->Count--;
        if(KeyCommand.Action == RELEASED)
        {
            KeyTable->Keys[KeyCommand.Key] = NEUTRAL;
        }
    }
}

internal void
RegisterMouseScroll(input_state *Input, double XOffset, double YOffset)
{
    
}