#ifndef _COMPONENT_MAP_C_
#define _COMPONENT_MAP_C_

#include "MapEntity.h"
#include "ComponentWorld.c"
#include "ComponentPlayer.c"
#include "Components.h"

// Componentes
// # Map
void MapLoad(World_ECS *w, MapEntity **Tile, ComponentWorld *world, int level)
{
    MapEntity *Map = Tile[level % MAX_TILES];
    for (int index = 0; index < Map->lenght; index++)
    {
        EntityDraw *line = &(Map->tiles[index]);
        switch (line->draw)
        {
        case APOINT:
            world->CreateEntity(w, line->entity, line->x, line->y);
            break;
        case LINE_HORIZONTAL:
            for (int x = 0; x < line->lenght; x++)
            {
                world->CreateEntity(w, line->entity, line->x + x, line->y);
            }
            break;
        case LINE_VERTICAL:
            for (int y = 0; y < line->lenght; y++)
            {
                world->CreateEntity(w, line->entity, line->x, line->y + y);
            }
            break;
        }
    }
    w->id = level;
}

typedef struct C_Map
{
    // void (*Initialize)(MapEntity **);
    void (*Load)(World_ECS *, MapEntity **, ComponentWorld *, int level);
    // void (*Destroy)(MapEntity **);
} ComponentMap;

ComponentMap ComponentMap_Init(void)
{
    ComponentMap m;
    m.Load = MapLoad;
    // player.Destroy = DestroyByEntity;
    return m;
}
#endif