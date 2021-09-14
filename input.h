/* date = August 27th 2021 1:33 pm */

#ifndef INPUT_H
#define INPUT_H

#include "math_util.h"

enum key_state
{
    NEUTRAL = 0,
    PRESSED,
    RELEASED,
    REPEAT,
    DRAG
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

// NOTE: Keyboard input
struct input_command_buffer
{
    input_command Commands[15]; // TODO: Idk how many commands I need
    int Count;
};

// NOTE: Key state lookup table for GLFW keyboard-specific key enumerations
// You would look in the table by saying Keys[GLFW_KEY_A] for example
struct key_table
{
    key_state Keys[359] = { NEUTRAL };
};

// NOTE: Key state lookup table for GLFW mouse-specific key enumerations
// You would look in the table by saying Keys[GLFW_MOUSE_BUTTON_1] for example
struct mouse_button_table
{
    key_state Buttons[8] = { NEUTRAL };
};

// NOTE: Mouse gestures
enum gesture_type
{
    MOVE,
    SCROLL,
};

// TODO: gestures could be a lot more sophisticated
struct gesture
{
    gesture_type Type;
    euler_angles Offset;
};

struct gesture_buffer
{
    gesture Gestures[10];
    int Count;
};

struct clock
{
    float dt;
    float FPS;
    float SecondsElapsed;
};

struct input_state
{
    float dt;
    float FPS;
    float SecondsElapsed;
    
    bool32 Clicked;
    glm::vec2 ClickPosition;
    glm::vec2 MousePosition;
    input_command_buffer CommandBuffer;
    gesture_buffer GestureBuffer;
};

#endif //INPUT_H
