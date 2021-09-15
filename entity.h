/* date = September 2nd 2021 1:25 pm */

#ifndef ENTITY_H
#define ENTITY_H

#define MAX_ENTITY_COUNT 50

#include "collision.h"

struct entity
{
    glm::vec3 Position;
    glm::vec3 Scale;
    euler_angles Rotation;
    
    // width, height, depth
    // half-width, half-height, half-depth
};

struct entity_group
{
    int Count;
    entity Entities[MAX_ENTITY_COUNT];
};

struct frame_entity_selections
{
    int EntitySelectionCount;
    int Selections[MAX_ENTITY_COUNT];
};

struct oriented_bounding_box
{
    
};


#endif //ENTITY_H
