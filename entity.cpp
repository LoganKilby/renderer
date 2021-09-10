internal int
SelectEntityAtScreenPoint(glm::vec2 ScreenCoords, entity_group *EntityCache)
{
    // Look up entity and return index
    
    // Check screen space coordinates of mouse and entity
    
    // Run through all entities? Sort entities by screen quadrant?
    glm::mat4 OrthoMatrix = GetViewportOrthoMatrix(0.0f, 1.0f);
    glm::mat4 ModelMatrix;
    // Compute the position of each entity until we find one that fits
    for(int EntityIndex = 0; EntityIndex < EntityCache->Count; ++EntityIndex)
    {
        
    }
    
    return 0;
}