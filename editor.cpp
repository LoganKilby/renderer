#include "editor.h"
#include "camera.h"

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
                // TODO: Select something
                Editor->Clicked = 1;
                Editor->EntitySelected = SelectEntityAtMouse(Input->MousePosition);
            }
            else if(FrameInput.Action == RELEASED)
            {
                Editor->Clicked = 0;
                Editor->DrawSelectionRegion = 0;
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
            if(Editor->Clicked)
            {
                // if something was not selected with the 
                if(!Editor->EntitySelected && !Editor->DrawSelectionRegion)
                {
                    Editor->DrawSelectionRegion = 1;
                    Editor->SelectionRegionOrigin = Input->MousePosition;
                }
                
            }
            else
            {
                //RotateFreeCamera(&Editor->Camera, FrameGesture.Offset, Input->dt);
            }
        }
    }
    
    MoveCameraByKeyPressed(&Editor->Camera, KeyTable, Input->dt);
}