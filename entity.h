/* date = September 2nd 2021 1:25 pm */

#ifndef ENTITY_H
#define ENTITY_H



#define MAX_ENTITY_COUNT 50

struct entity_group
{
    int Count;
    glm::vec3 Position[MAX_ENTITY_COUNT];
    glm::vec3 Scale[MAX_ENTITY_COUNT];
    glm::vec3 Rotation[MAX_ENTITY_COUNT];
    glm::vec4 Area[MAX_ENTITY_COUNT];
};

internal int SelectEntityAtScreenPoint(glm::vec2 ScreenCoords, entity_group *EntityCache);
#endif //ENTITY_H
