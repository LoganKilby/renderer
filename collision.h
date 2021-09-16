/* date = September 14th 2021 8:55 am */

#ifndef COLLISION_H
#define COLLISION_H

struct ray
{
    glm::vec3 Origin;
    glm::vec3 Direction;
};

internal bool
PointInRect(glm::vec2, rect);

#endif //COLLISION_H
