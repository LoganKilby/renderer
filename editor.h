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
    int EntitySelected;
    bool32 EditorActive;
    bool32 DrawSelectionRegion;
    bool32 Clicked;
    glm::vec2 SelectionRegionOrigin; // Updated every fram the user clicks
};

#endif //EDITOR_H
