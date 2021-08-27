/* date = August 27th 2021 1:33 pm */

#ifndef INPUT_H
#define INPUT_H

struct global_input
{
    glm::vec2 PrevMousePos;
    float FrameTime;
    
    // TODO: Add multiple camera modes
    //       Modelling Camera
    //       Fly Camera
    bool32 CameraMode;
    
    
    // I want to be able to manage frame time and process keyboard input in intervals and not
    // every single frame.
};


#endif //INPUT_H
