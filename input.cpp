// Input handling
#include "input.h"

internal void
UpdateClock(input_state *Input)
{
    float SecondsElapsed = glfwGetTime();
    Input->dt = SecondsElapsed - Input->SecondsElapsed;
    Input->SecondsElapsed = SecondsElapsed;
    Input->FPS = 1000.0f / Input->dt;
}

// Test inlining these functions
internal void
PushGesture(gesture_buffer *Buffer, gesture Gesture)
{
    Assert(Buffer->Count < ArrayCount(Buffer->Gestures));
    Buffer->Gestures[Buffer->Count++] = Gesture;
}

internal bool
PopGesture(gesture_buffer *Buffer, gesture *OutputGesture)
{
    if(Buffer->Count)
    {
        *OutputGesture = Buffer->Gestures[Buffer->Count - 1];
        Buffer->Count--;
        return true;
    }
    
    return false;
}

internal void
PushInputCommand(input_command_buffer *Buffer, input_command Command)
{
    Assert(Buffer->Count < ArrayCount(Buffer->Commands));
    Buffer->Commands[Buffer->Count++] = Command;
}

internal bool
PopInputCommand(key_table *KeyTable, input_command_buffer *Buffer, input_command *Output)
{
    if(Buffer->Count)
    {
        input_command Command = Buffer->Commands[Buffer->Count - 1];
        if(Command.Action == RELEASED)
        {
            KeyTable->Keys[Command.Key] = NEUTRAL;
        }
        
        Buffer->Count--;
        *Output = Command;
        
        return true;
    }
    
    return false;
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
            if(*KeyState == NEUTRAL)
            {
                *KeyState = PRESSED;
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
RegisterMouseButtonInput(input_state *Input, mouse_button_table *ButtonTable, int Button, int Action, int Mods)
{
    key_state *ButtonState = &ButtonTable->Buttons[Button];
    input_command Command;
    switch(Action)
    {
        // NOTE: GLFW mouse button actions can be GLFW_PRESS or GLFW_RELEASE
        case GLFW_PRESS:
        {
            *ButtonState = PRESSED;
            Command.Key = Button;
            Command.Action = PRESSED;
            Command.Mods = Mods;
            Command.Device = MOUSE;
            PushInputCommand(&Input->CommandBuffer, Command);
            
            if(Button == LEFT_MOUSE_BUTTON)
                Input->ClickPosition = Input->MousePosition;
        } break;
        
        case GLFW_RELEASE:
        {
            *ButtonState = RELEASED;
            Command.Key = Button;
            Command.Action = RELEASED;
            Command.Mods = Mods;
            Command.Device = MOUSE;
            PushInputCommand(&Input->CommandBuffer, Command);
        } break;
    }
}

internal void
RegisterMouseMovement(input_state *Input, mouse_button_table *Table, double XPos, double YPos)
{
    // NOTE: Idk if we need to track whether the mouse was moved as a boolean state
    // or if we can just keep track of the offset from the previous frame and that's enough
    glm::vec2 PrevMousePos = Input->MousePosition;
    Input->MousePosition.x = XPos;
    Input->MousePosition.y = YPos;
    
    gesture MouseGesture = {};
    MouseGesture.Type = MOVE;
    MouseGesture.Offset.Yaw = XPos - PrevMousePos.x;
    MouseGesture.Offset.Pitch = YPos - PrevMousePos.y;
    PushGesture(&Input->GestureBuffer, MouseGesture);
    
    if(Table->Buttons[LEFT_MOUSE_BUTTON] == PRESSED)
    {
        Table->Buttons[LEFT_MOUSE_BUTTON] = DRAG;
    }
}

internal void
RegisterMouseScroll(input_state *Input, double XOffset, double YOffset)
{
    gesture Gesture = {};
    Gesture.Type = SCROLL;
    Gesture.Offset.Yaw = XOffset;
    Gesture.Offset.Pitch = YOffset;
    PushGesture(&Input->GestureBuffer, Gesture);
}

internal void
FlushInputBuffers(input_state *Input)
{
    if(Input->CommandBuffer.Count)
        printf("%d inputs discarded\n",Input->CommandBuffer.Count);
    memset(&Input->CommandBuffer, 0, sizeof(input_command_buffer));
    
    if(Input->GestureBuffer.Count)
        printf("%d gestures discarded\n", Input->GestureBuffer.Count);
    memset(&Input->GestureBuffer, 0, sizeof(gesture_buffer));
}
