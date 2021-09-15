#include "entity.h"

internal void
ClearEntitySelections(frame_entity_selections *EntitySelections)
{
    memset(EntitySelections->Selections, 0, sizeof(EntitySelections->Selections));
    EntitySelections->EntitySelectionCount = 0;
}

internal glm::mat4 
RotateMat4(const glm::mat4 &Matrix, euler_angles Angles)
{
    // The order of calculating euler angles is important right?
    // TODO: switch to quaternions?
    glm::mat4 Result = Matrix;
    Result = glm::rotate(Result, glm::radians(Angles.Pitch), glm::vec3(1, 0, 0));
    Result = glm::rotate(Result, glm::radians(Angles.Yaw), glm::vec3(0, 1, 0));
    Result = glm::rotate(Result, glm::radians(Angles.Roll), glm::vec3(0, 0, 1));
    return Result;
}

internal void
SelectEntitiesAtScreenRegion(frame_entity_selections *EntitySelection, entity_group *Entities, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix, rect SelectionRegion)
{
    
}

internal bool
RayCollision(glm::vec3 Point, oriented_bounding_box OOB)
{
    // instead of finding the basis vectors of the OBB, use the maximum x, y, and z values of the primitives
    
}