/* Component.c*/
#ifndef _COMPONENTS_C_
#define _COMPONENTS_C_

#include "Entities.h"

typedef struct
{
    void (*Initialize)(World_ECS *);
    int (*CreateEntity)(World_ECS *, TypeEntity, int, int);
    TypeEntity (*Collide)(World_ECS *, PointData *);
    void (*EnabledEntity)(World_ECS *, PointData *, int);
    void (*MovimentPlayer)(Entity_ECS *, int);
    void (*Destroy)(World_ECS *);
} Component;

// Inicializar el "MUNDO"
static inline void World_Init(World_ECS *w)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
        w->entities[i] = NULL;
    w->count = 0;
    w->player = NULL;
    w->id = 0;
}

// Crear nueva entidad
static inline Entity_ECS *CreateEntity(TypeEntity type, int x, int y)
{
    Entity_ECS *e = allocate(Entity_ECS);
    e->type = type;
    e->active = 1;

    if (type == ENTITY_SNAKE)
    {
        SnakeData *sd = allocate(SnakeData);
        sd->length = 3;
        for (int i = 0; i < sd->length; i++)
        {
            sd->body[i].x = x - i;
            sd->body[i].y = y;
        }
        sd->gravityEnabled = 1;
        e->data = sd;
    }
    else
    {
        PointData *p = allocate(PointData);
        p->x = x;
        p->y = y;
        e->data = p;
    }
    e->vel.dx = 0;
    e->vel.dy = 0;
    return e;
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

// Destruir entidad
static inline void DestroyByEntity(Entity_ECS *e)
{
    if (!e)
        return;
    free(e->data);
    free(e);
}

static inline void World_EnabledEntityByPoint(World_ECS *w, PointData *point, int active)
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
        DestroyByEntity(w->entities[i]);
        w->entities[i] = NULL;
    }
}

// Colisión snake-tail
inline TypeEntity SnakeSelfCollision(Entity_ECS *player, PointData *position)
{
    SnakeData *sd = (SnakeData *)player->data;
    PointData *tail = NULL;
    // Recorremos el cuerpo a partir del segundo segmento
    for (int index = 0; index < sd->length; index++)
    {
        tail = &sd->body[index];
        if (EqualPoint(tail, position))
        {
            return ENTITY_SNAKE;
        }
    }
    return ENTITY_NONE;
}

static inline TypeEntity PointCollision(World_ECS *w, PointData *position)
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
    return ENTITY_NONE; // SnakeSelfCollision(w->player, position);
}

static inline void PlayerMoviment(Entity_ECS *player, int supported)
{
    SnakeData *snake = (SnakeData *)player->data;

    // Mover cola siguiendo la cabeza
    if (player->vel.dx || player->vel.dy || !supported)
    {
        for (int j = snake->length; j > 0; j--)
        {
            if (supported)
            {
                snake->body[j].x = snake->body[j - 1].x;
                snake->body[j].y = snake->body[j - 1].y;
            }
            else
            {
                snake->body[j].y += GRAVITY; // todo cae
            }
        }
        // Mover cabeza
        snake->body[0].x += player->vel.dx;
        snake->body[0].y += supported ? player->vel.dy : GRAVITY;
    }

    // Reset de velocidades temporales
    //player->vel.dx = 0;
    //player->vel.dy = 0;
}

static Component Component_Init(void)
{
    Component world;
    world.Initialize = World_Init;
    world.CreateEntity = World_CreateEntity;
    world.EnabledEntity = World_EnabledEntityByPoint;
    world.Destroy = World_Destroy;
    world.Collide = PointCollision;
    world.MovimentPlayer = PlayerMoviment;
    return world;
}
// Component WorldComponent = Component_Init();
#endif