/* date = September 2nd 2021 1:38 pm */

#ifndef EDITOR_H
#define EDITOR_H

#include "camera.h"
#include "input.h"
#include "entity.h"

struct editor
{
    // TODO: Will likely want multiple cameras
    camera Camera;
    bool32 Active;
    bool32 RotateCamera;
};

#endif //EDITOR_H
