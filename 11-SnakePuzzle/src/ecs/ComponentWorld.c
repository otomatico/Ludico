#ifndef _COMPONENT_WORLD_C_
#define _COMPONENT_WORLD_C_

#include "Entities.h"
// Component World
//  Inicializar el "MUNDO"
static inline void World_Init(World_ECS *w)
{
    w->id = 0;
    w->count = 0;
    for (int i = 0; i < MAX_ENTITIES; i++)
        w->entities[i] = NULL;
    w->player = NULL;
}

// Crear nueva entidad y devolver índice
static inline int World_CreateEntity(World_ECS *w, TypeEntity type, int x, int y)
{
    if (type == ENTITY_SNAKE)
    {
        w->player = CreateEntity(type, x, y);
        return 0;
    }
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (w->entities[i] == NULL)
        {
            w->entities[i] = CreateEntity(type, x, y);
            w->count++;
            return i;
        }
    }
    return -1; // no hay espacio
}

static inline void World_EntityEnabledByPoint(World_ECS *w, PointData *point, int active)
{
    for (int i = 0; i < w->count; i++)
    {
        if (EqualPoint(w->entities[i]->data, point))
        {
            w->entities[i]->active = active;
            return;
        }
    }
}

static inline void World_Destroy(World_ECS *w)
{
    for (int i = 0; i < w->count; i++)
    {
        Entity_Destroy(w->entities[i]);
        w->entities[i] = NULL;
    }
    Entity_Destroy(w->player);
    w->player=NULL;
    w->count = 0;
}

static inline TypeEntity World_PointCollision(World_ECS *w, PointData *position)
{
    for (int index = 0; index < MAX_ENTITIES; index++)
    {
        Entity_ECS *e = w->entities[index];
        if (!e || !e->active)
            continue;

        PointData *f = (PointData *)e->data;
        if (EqualPoint(f, position))
        {
            return e->type;
        }
    }
    return ENTITY_NONE;
}


typedef struct
{
    void (*Initialize)(World_ECS *);
    int (*CreateEntity)(World_ECS *, TypeEntity, int, int);
    TypeEntity (*Collide)(World_ECS *, PointData *);
    void (*EnabledEntity)(World_ECS *, PointData *, int);
    void (*Destroy)(World_ECS *);
} ComponentWorld;

ComponentWorld ComponentWorld_Init(void)
{
    ComponentWorld world;

    world.Initialize = World_Init;
    world.CreateEntity = World_CreateEntity;
    world.EnabledEntity = World_EntityEnabledByPoint;
    world.Collide = World_PointCollision;
    world.Destroy = World_Destroy;
    return world;
}
#endif