#include "editor.h"

internal void
ProcessInputForEditor(input_state *Input, key_table *KeyTable, editor *Editor)
{
    input_command FrameInput;
    while(PopInputCommand(KeyTable, &Input->CommandBuffer, &FrameInput))
    {
        if(FrameInput.Device == MOUSE)
        {
            if(FrameInput.Action == PRESSED)
            {
                // TODO: Select something?
                Input->Clicked = 1;
            }
            else if(FrameInput.Action == RELEASED)
            {
                Input->Clicked = 0;
            }
        }
    }
    
    
    gesture FrameGesture;
    while(PopGesture(&Input->GestureBuffer, &FrameGesture))
    {
        if(FrameGesture.Type == SCROLL)
        {
            ZoomCamera(&Editor->Camera, FrameGesture.Offset.Pitch);
        }
        
        if(FrameGesture.Type == MOVE)
        {
            if(Input->Clicked)
            {
                // Draw selection region, no entity selected
                Input->DrawSelectionRegion = 1;
            }
            else
            {
                Input->DrawSelectionRegion = 0;
            }
        }
    }
    
    MoveCameraByKeyPressed(&Editor->Camera, KeyTable, Input->dt);
}