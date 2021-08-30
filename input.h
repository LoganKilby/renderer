/* date = August 30th 2021 1:44 pm */

#ifndef INPUT_H
#define INPUT_H

enum button_state
{
    NEUTRAL,
    PRESSED,
    RELEASED
};

struct input_command
{
    button_state Action;
    int Key;
};


struct input_state
{
    float dt;
    float SecondsElapsed;
    float LastCommandTimeStamp; // In seconds
    float CommandLatency;
    
    glm::vec3 PrevMousePos;
    
    int CommandsBuffered;
    input_command CommandBuffer[3];
    button_state KeyTable[349] = {NEUTRAL};
};

#endif //INPUT_H
