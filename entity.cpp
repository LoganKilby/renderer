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
SelectEntityAtScreenPoint(frame_entity_selections *EntitySelections, entity_group *EntityGroup, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix, glm::vec2 ScreenCoords)
{
    glm::vec2 NDCCoords = GetNDC(ScreenCoords);
    glm::vec4 ClipCoords = glm::vec4(NDCCoords, -1.0f, 1.0f);
    glm::vec4 EyeSpaceCoords = glm::inverse(ProjectionMatrix) * ClipCoords;
    EyeSpaceCoords = glm::vec4(EyeSpaceCoords.x, EyeSpaceCoords.y, -1.0f, 0.0f);
    glm::vec3 WorldSpaceCoords = glm::vec3(glm::inverse(ViewMatrix) * EyeSpaceCoords);
    WorldSpaceCoords = glm::normalize(WorldSpaceCoords);
    
#if 0
    glm::mat4 ModelMatrix;
    entity Entity;
    for(int EntityIndex = 0; EntityIndex < EntityGroup->Count; ++EntityIndex)
    {
        Entity = EntityGroup->Entities[EntityIndex];
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, Entity.Position);
        ModelMatrix = RotateMat4(ModelMatrix, Entity.Rotation);
        ModelMatrix = glm::scale(ModelMatrix, Entity.Scale);
        
        // TODO:
        // Test intersection with plane here. Construct the plane from vertices?
        // How wide and tall is the plane centered at Entity.Position?
        // I know that the geometry is simply a plane in this initial case. It could potentially
        // be a cube (6 planes) or... 
        // If all geometry had bounding boxes, wouldn't it always be really simple to always do
        // plane intersection tests?
        
        // Read book about plane intersections and how to do them.
    }
#endif
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