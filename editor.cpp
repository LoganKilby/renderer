#include "editor.h"
#include "camera.h"
#include "entity.h"

internal void
ProcessEditorInput(editor *Editor, input_state *Input, key_table *KeyTable)
{
    Editor->MouseButtons[0] = 0;
    Editor->MouseButtons[1] = 0;
    
    input_command FrameInput;
    while(PopInputCommand(KeyTable, &Input->CommandBuffer, &FrameInput))
    {
        if(FrameInput.Device == MOUSE)
        {
            switch(FrameInput.Key)
            {
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
                
                case LEFT_MOUSE_BUTTON:
                {
                    Editor->MouseButtons[LEFT_MOUSE_BUTTON] = (FrameInput.Action == PRESSED) ? 1 : 0;
                    
                } break;
                
                case RIGHT_MOUSE_BUTTON:
                {
                    Editor->MouseButtons[RIGHT_MOUSE_BUTTON] = (FrameInput.Action == PRESSED) ? 1 : 0;
                    
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
            if(Editor->RotateCamera)
            {
                Editor->Camera = RotateCamera(Editor->Camera, FrameGesture.Offset, Input->Clock.dt);
            }
        }
    }
    
    Editor->Camera = TranslateCamera(Editor->Camera, KeyTable, Input->Clock.dt);
}