#include "entity.h"

internal void
ClearEntityGroup(entity_group *EntitySelections)
{
    memset(EntitySelections->Entities, 0, ArrayCount(EntitySelections->Entities) * sizeof(entity));
    EntitySelections->Count = 0;
}

internal void
SelectEntitiesInScreenRegion(entity_group *SelectedEntities, entity_group *EntityGroup, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix, rect ScreenRegion)
{
    ClearEntityGroup(SelectedEntities);
    
    float X, Y, Width, Height;
    GetViewport(&X, &Y, &Width, &Height);
    
    glm::vec4 Result;
    for(entity *Entity = &EntityGroup->Entities[0]; 
        Entity < &EntityGroup->Entities[0] + EntityGroup->Count; 
        ++Entity)
    {
        Result = ProjectionMatrix * ViewMatrix * glm::vec4(Entity->Position, 1.0f);
        Result /= Result.w;
        
        glm::vec2 NDC;
        NDC.x = ((Result.x + 1.0f) * 0.5f) * Width;
        NDC.y = ((Result.y + 1.0f) * 0.5f) * Height;
        
        if(PointInRect(NDC, ScreenRegion))
        {
            printf("Entity selected\n");
            entity EntitySelected = *Entity;
            EntitySelected.Scale *= 2.0f;
            SelectedEntities->Entities[SelectedEntities->Count++] = EntitySelected;
        }
    }
}

internal void
QueueEntityGroupForRendering(entity_group *EntityGroup, draw_buffer *Buffer)
{
    for(entity *Entity = &EntityGroup->Entities[0];
        Entity < &EntityGroup->Entities[0] + EntityGroup->Count;
        ++Entity)
    {
        draw_command Command;
    }
}