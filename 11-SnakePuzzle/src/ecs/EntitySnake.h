/*🧩 EntitySnake.h*/
#ifndef _ENTITY_SNAKE_H_
#define _ENTITY_SNAKE_H_

#include "../etc/env"
#include "Entities.h"

typedef enum _TE
{
    ENTITY_NONE,
    ENTITY_SNAKE,       // 🐍 Snake
    ENTITY_FOOD,        // 🍎 Food
    ENTITY_PLATFORM,    // 🧱 Wall
    ENTITY_ROCK,        // 🪨 Rock
    ENTITY_SPIKE,       // 🔱 Spike
    ENTITY_EXIT         // 🚪 Door
} TypeEntity;

typedef struct
{
    int x, y;
} PointData;

int EqualPoint(PointData *a, PointData *b)
{
    return a->x == b->x && a->y == b->y;
}

typedef struct
{
    int length;
    int gravityEnabled;
    PointData body[MAX_BODY_SNAKE];
} SnakeData;

Entity_ECS *CreateEntity(TypeEntity type, int x, int y)
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
#endif
