#include "input.h"

inline void
PushInputCommand(input_state *Input, input_command Command)
{
    Assert(Input->CommandsBuffered < ArrayCount(Input->CommandBuffer));
    Input->CommandBuffer[Input->CommandsBuffered++] = Command;
}

internal void
ProcessKeyboardInput(input_state *Input, int Key, int Scancode, int Action, int Mods)
{
    button_state *KeyState = &Input->KeyTable[Key];
    input_command Command;
    switch(Action)
    {
        case GLFW_PRESS:
        {
            // TODO: I would like to have the option of maintaining the "pressed" state in
            // circumstances where I want to continually do something while the button
            // is pressed
            if(*KeyState == NEUTRAL)
            {
                *KeyState = PRESSED;
                Command.Key = Key;
                Command.Action = PRESSED;
                PushInputCommand(Input, Command);
            }
        } break;
        
        case GLFW_RELEASE:
        {
            if(*KeyState == PRESSED)
            {
                *KeyState = NEUTRAL;
                Command.Key = Key;
                Command.Action = RELEASED;
                PushInputCommand(Input, Command);
                // Issue command?
            }
        } break;
        
        case GLFW_REPEAT:
        {
            // ?
        } break;
    }
}

internal input_command *
PopCommand(input_state *Input)
{
    input_command *Result;
    if(Input->CommandsBuffered)
    {
        Result = &Input->CommandBuffer[Input->CommandsBuffered - 1];
        Input->CommandsBuffered--;
    }
    else
    {
        Result = 0;
    }
    
    return Result;
}