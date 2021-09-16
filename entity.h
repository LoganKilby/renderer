/* date = September 2nd 2021 1:25 pm */

#ifndef ENTITY_H
#define ENTITY_H

#define MAX_ENTITY_COUNT 50

#include "collision.h"

struct entity
{
    glm::vec3 Position; // World space
    glm::vec3 Scale;
    euler_angles Rotation;
};

struct entity_group
{
    int Count;
    entity Entities[MAX_ENTITY_COUNT];
};

internal void ClearEntityGroup(entity_group *);

#endif //ENTITY_H
