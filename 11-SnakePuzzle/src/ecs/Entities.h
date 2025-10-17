/*🧩 Entities.h*/
#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "../etc/env"

typedef enum
{
    ENTITY_SNAKE, // 🐍 Snake
    ENTITY_FOOD,  // 🍎 Food
    ENTITY_WALL   // 🧱 Wall
} TypeEntity;

typedef struct
{
    int x, y;
} PointData;

typedef struct
{
    PointData body[MAX_BODY_SNAKE];
    int length;
} SnakeData;

typedef struct
{
    int dx, dy;
    int activo;
} Velocity;

typedef struct
{
    int activo;
    TypeEntity type;
    void *data;   // apunta a la estructura concreta
    Velocity vel; // opcional
} Entity_ECS;

typedef struct
{
    Entity_ECS *entities[MAX_ENTITIES]; // OJO Array de punteros
    int count;
} World_ECS;

static inline void World_Init(World_ECS *w)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
        w->entities[i] = NULL;
    w->count = 0;
}

// Crear nueva entidad y devolver índice
static inline int World_CreateEntity(World_ECS *w, Entity_ECS *e)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (w->entities[i] == NULL)
        {
            w->entities[i] = e;
            w->count++;
            return i;
        }
    }
    return -1; // no hay espacio
}

// Destruir entidad
void DestroyEntity(Entity_ECS *e)
{
    if (!e)
        return;
    free(e->data);
    free(e);
}
#endif
