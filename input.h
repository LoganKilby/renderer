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

// NOTE: Keyboard input
struct input_command_buffer
{
    input_command Commands[15]; // TODO: Idk how many commands I need
    int Count;
};

// NOTE: Key state lookup table for GLFW-specific key enumerations
// You would look in the table by saying Keys[GLFW_KEY_A] for example
struct key_table
{
    key_state Keys[359] = { NEUTRAL };
};

// NOTE: Key state lookup table for GLFW-specific key enumerations
// You would look in the table by saying Keys[GLFW_KEY_A] for example
struct mouse_button_table
{
    key_state Buttons[8] = { NEUTRAL };
};

// NOTE: Mouse gestures
enum gesture_type
{
    MOVE,
    SCROLL,
    DRAG
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

struct input_state
{
    float dt;
    float FPS;
    float SecondsElapsed;
    
    // TODO: I need a gesture command buffer / syste that's not integrated with the keyboard input
    // Raw mouse input
    glm::vec2 PrevMousePos;
    glm::vec2 MouseSelectionBoxBegin;
    input_command_buffer CommandBuffer;
    gesture_buffer GestureBuffer;
};

#endif //INPUT_H
