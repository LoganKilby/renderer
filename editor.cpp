#include "editor.h"
#include "camera.h"
#include "entity.h"

internal void
ProcessInputForEditor(input_state *Input, key_table *KeyTable, editor *Editor, entity_group *EntityCache)
{
    input_command FrameInput;
    while(PopInputCommand(KeyTable, &Input->CommandBuffer, &FrameInput))
    {
        if(FrameInput.Device == MOUSE)
        {
            switch(FrameInput.Key)
            {
                case LEFT_MOUSE_BUTTON:
                {
                    
                    if(FrameInput.Action == PRESSED)
                    {
                        // TODO: Select something (UI/Entity)
                        Editor->Clicked = 1;
                        Editor->EntitySelected = SelectEntityAtScreenPoint(Input->MousePosition, EntityCache);
                    }
                    else if(FrameInput.Action == RELEASED)
                    {
                        Editor->Clicked = 0;
                        Editor->DrawSelectionRegion = 0;
                    }
                } break;
                
                case RIGHT_MOUSE_BUTTON:
                {
                    
                } break;
                
                case SCROLL_BUTTON:
                {
                    if(FrameInput.Action == PRESSED)
                    {
                        Editor->RotateCamera = 1;
                    }
                    else if(FrameInput.Action == RELEASED)
                    {
                        Editor->RotateCamera = 0;
                    }
                    
                } break;
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
                if(!Editor->EntitySelected && !Editor->DrawSelectionRegion)
                {
                    Editor->DrawSelectionRegion = 1;
                    Editor->SelectionRegionOrigin = Input->MousePosition;
                }
            }
            
            if(Editor->RotateCamera)
            {
                RotateCamera(&Editor->Camera, FrameGesture.Offset, Input->dt);
            }
        }
    }
    
    MoveCameraByKeyPressed(&Editor->Camera, KeyTable, Input->dt);
}