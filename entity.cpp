#include "entity.h"

internal void
ClearEntityGroup(entity_group *Group)
{
    if(Group->Count)
    {
        memset(Group->Entities, 0, ArrayCount(Group->Entities) * sizeof(entity));
        Group->Count = 0;
    }
}

internal void
SelectEntity(entity Entity, entity_group *SelectionGroup)
{
    Assert(SelectionGroup->Count < ArrayCount(SelectionGroup->Entities));
    SelectionGroup->Entities[SelectionGroup->Count++] = Entity;
}

internal void
PushEntityToGroup(entity_group *EntityGroup, entity Entity)
{
    Assert(EntityGroup->Count < ArrayCount(EntityGroup->Entities));
    EntityGroup->Entities[EntityGroup->Count++] = Entity;
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
            SelectEntity(*Entity, SelectedEntities);
        }
    }
}

internal void
QueueEntityGroupForRendering(entity_group *EntityGroup, draw_buffer *Buffer)
{
    // TODO: Calculate Model matrix instead?
    draw_command Command;
    for(entity *Entity = &EntityGroup->Entities[0];
        Entity < &EntityGroup->Entities[0] + EntityGroup->Count;
        ++Entity)
    {
        Command.Primitive = Entity->Primitive;
        Command.Position = Entity->Position;
        Command.Scale = Entity->Scale;
        Command.Rotation = Entity->Rotation;
        PushDrawCommand(Buffer, Command);
    }
}
