/* date = September 14th 2021 8:55 am */

#ifndef COLLISION_H
#define COLLISION_H

struct line
{
    glm::vec3 Direction;
    glm::vec3 Origin;
    // float Length;
};

struct plane
{
    glm::vec3 Position;
    glm::vec3 Normal;
    float d; // Distance from origin (0, 0, 0)
};

struct ray
{
    glm::vec3 Origin;
    glm::vec3 Direction;
};

#endif //COLLISION_H
