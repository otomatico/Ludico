/* Component.c*/
#ifndef _COMPONENTS_C_
#define _COMPONENTS_C_

#include "../etc/env"
#include "Entities.h"

// Snake
Entity_ECS *CreateSnake(int length, int startX, int startY)
{
    Entity_ECS *e = allocate(Entity_ECS);
    e->activo = 1;
    e->type = ENTITY_SNAKE;

    SnakeData *s = allocate(SnakeData);
    s->length = length;
    for (int i = 0; i < length; i++)
    {
        s->body[i].x = startX - i;
        s->body[i].y = startY;
    }
    e->data = s;

    e->vel.activo = 1;
    e->vel.dx = 1;
    e->vel.dy = 0;

    return e;
}

// Food
Entity_ECS *CreateFood(int x, int y)
{
    Entity_ECS *e = allocate(Entity_ECS);
    e->activo = 1;
    e->type = ENTITY_FOOD;

    PointData *f = allocate(PointData);
    f->x = x;
    f->y = y;
    e->data = f;

    e->vel.activo = 0;
    return e;
}

// Wall
Entity_ECS *CreateWall(int x, int y)
{
    Entity_ECS *e = allocate(Entity_ECS);
    e->activo = 1;
    e->type = ENTITY_WALL;

    PointData *w = allocate(PointData);
    w->x = x;
    w->y = y;
    e->data = w;

    e->vel.activo = 0;
    return e;
}

// Square Wall
void CreateBorderWalls(World_ECS *world, int width, int height)
{
    // Bordes superior e inferior
    for (int x = 0; x < width; x++)
    {
        Entity_ECS *top = CreateWall(x, 0);
        World_CreateEntity(world, top);

        Entity_ECS *bottom = CreateWall(x, height - 1);
        World_CreateEntity(world, bottom);
    }

    // Bordes izquierdo y derecho
    for (int y = 1; y < height - 1; y++)
    { // excluye esquinas ya creadas
        Entity_ECS *left = CreateWall(0, y);
        World_CreateEntity(world, left);

        Entity_ECS *right = CreateWall(width - 1, y);
        World_CreateEntity(world, right);
    }
}

// Colisión snake-self
int SnakeSelfCollision(Entity_ECS *snake)
{
    if (!snake || !snake->activo || snake->type != ENTITY_SNAKE)
        return 0;

    SnakeData *s = (SnakeData *)snake->data;
    int headX = s->body[0].x;
    int headY = s->body[0].y;

    // Recorremos el cuerpo a partir del segundo segmento
    for (int i = 1; i < s->length; i++)
    {
        if (s->body[i].x == headX && s->body[i].y == headY)
        {
            return 1; // colisión
        }
    }
    return 0;
}

// Colisión snake-wall
int SnakeWallCollision(World_ECS *w, Entity_ECS *snake)
{
    if (!snake || !snake->activo || snake->type != ENTITY_SNAKE)
        return 0;

    SnakeData *s = (SnakeData *)snake->data;
    int headX = s->body[0].x;
    int headY = s->body[0].y;

    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        Entity_ECS *e = w->entities[i];
        if (!e || !e->activo)
            continue;
        if (e->type != ENTITY_WALL)
            continue;

        PointData *wdata = (PointData *)e->data;
        if (wdata->x == headX && wdata->y == headY)
        {
            return 1; // colisión con pared
        }
    }

    return 0; // no colisión
}

// Colisión snake-food
int SnakeFoodCollision(World_ECS *w, int snakeId)
{
    Entity_ECS *snake = w->entities[snakeId];
    SnakeData *s = (SnakeData *)snake->data;
    int eaten = 0;

    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        Entity_ECS *e = w->entities[i];
        if (!e || !e->activo)
            continue;

        if (e->type == ENTITY_FOOD)
        {
            PointData *f = (PointData *)e->data;
            if (f->x == s->body[0].x && f->y == s->body[0].y)
            {
                eaten = 1;
                DestroyEntity(e);
                w->entities[i] = NULL;
            }
        }
    }
    return eaten;
}

#endif