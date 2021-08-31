/* date = August 27th 2021 1:33 pm */

#ifndef INPUT_H
#define INPUT_H

enum key_state
{
    PRESSED,
    RELEASED,
    REPEAT,
    NEUTRAL
};

enum device
{
    MOUSE,
    KEYBOARD,
    GAMEPAD
};

struct input_command
{
    int Key;
    key_state Action;
    int Mods;
    device Device;
};

struct input_command_buffer
{
    input_command Commands[15];
    int Count;
};

// Key state lookup table for GLFW key enumerations
struct key_table
{
    key_state Keys[359] = { NEUTRAL };
};

struct input_state
{
    float dt;
    float FPS;
    float SecondsElapsed;
    
    // TODO: I need a gesture command buffer / syste that's not integrated with the keyboard input
    // Raw mouse input
    glm::vec2 PrevMousePos;
    glm::vec2 MouseFrameMovement;
    bool32 MouseMovedThisFrame;
    float MouseFrameScrollOffset; // FOV will be FOV -= MouseFrameScrollOffset this frame
    bool32 MouseScrolledThisFrame;
    
    input_command_buffer CommandBuffer;
};

#endif //INPUT_H
