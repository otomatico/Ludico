/*🕹️ systems.h*/
#ifndef _SYSTEMS_C_
#define _SYSTEMS_C_

#include "../etc/env"
#include "../lib/Engine2D.h"

#include "Entities.h"
#include "Components.c"

// Entrada para la serpiente
void SystemInput(World_ECS *w, int snakeId)
{
    int key = press();
    if (key == KEY_EMPTY)
        return;

    Entity_ECS *snake = w->entities[snakeId];
    if (!snake->vel.activo)
        return;

    switch (key)
    {
    case KEY_UP:
        if (snake->vel.dy == 0)
        {
            snake->vel.dx = 0;
            snake->vel.dy = -1;
        }
        break;
    case KEY_DOWN:
        if (snake->vel.dy == 0)
        {
            snake->vel.dx = 0;
            snake->vel.dy = 1;
        }
        break;
    case KEY_LEFT:
        if (snake->vel.dx == 0)
        {
            snake->vel.dx = -1;
            snake->vel.dy = 0;
        }
        break;
    case KEY_RIGHT:
        if (snake->vel.dx == 0)
        {
            snake->vel.dx = 1;
            snake->vel.dy = 0;
        }
        break;
    default:
        break;
    }
}

// Movimiento
void SystemMovement(World_ECS *w, int width, int height)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        Entity_ECS *e = w->entities[i];
        if (!e || !e->activo || !e->vel.activo)
            continue;

        if (e->type == ENTITY_SNAKE)
        {
            SnakeData *s = (SnakeData *)e->data;
            // Mover cuerpo
            for (int j = s->length - 1; j > 0; j--)
            {
                s->body[j].x = s->body[j - 1].x;
                s->body[j].y = s->body[j - 1].y;
            }
            s->body[0].x += e->vel.dx;
            s->body[0].y += e->vel.dy;

            // Wrap
            if (s->body[0].x < 0)
                s->body[0].x = width - 1;
            if (s->body[0].x >= width)
                s->body[0].x = 0;
            if (s->body[0].y < 0)
                s->body[0].y = height - 1;
            if (s->body[0].y >= height)
                s->body[0].y = 0;
        }
    }
}

// Colisión
int SystemCollision(World_ECS *w, int snakeId)
{
    Entity_ECS *snake = w->entities[snakeId];
    if (SnakeFoodCollision(w, snakeId))
    {
        Entity_ECS *newFood = CreateFood(rand() % (SCREEN_WIDTH - 2) + 1, rand() % (SCREEN_HEIGHT - 2) + 1);
        World_CreateEntity(w, newFood);

        SnakeData *sdata = (SnakeData *)snake->data;
        // Para evitar posicion fantasma en [0,0]
        sdata->body[sdata->length].x = sdata->body[sdata->length - 1].x;
        sdata->body[sdata->length].y = sdata->body[sdata->length - 1].y;
        sdata->length++; // crecer
    }
    // Colisión consigo misma
    if (SnakeSelfCollision(snake))
    {
        printf("\nHas chocado contigo mismo!\n");
        return 0;
    }

    // Colisión con paredes
    if (SnakeWallCollision(w, snake))
    {
        printf("\nHas chocado con una pared!\n");
        return 0;
    }
    return 1;
}

// Renderizado
void SystemRender(World_ECS *w, Graphic gfx, Canvas *canvas)
{
    gfx.Clear(canvas, BLACK);

    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        Entity_ECS *e = w->entities[i];
        if (!e || !e->activo)
            continue;

        switch (e->type)
        {
        case ENTITY_SNAKE:
        {
            SnakeData *s = (SnakeData *)e->data;
            gfx.SetPixel(canvas, s->body[0].x, s->body[0].y, LIGHTGREEN);
            for (int j = 1; j < s->length; j++)
                gfx.SetPixel(canvas, s->body[j].x, s->body[j].y, GREEN);
            break;
        }
        case ENTITY_FOOD:
        {
            PointData *f = (PointData *)e->data;
            gfx.SetPixel(canvas, f->x, f->y, LIGHTRED);
            break;
        }
        case ENTITY_WALL:
        {
            PointData *wdata = (PointData *)e->data;
            gfx.SetPixel(canvas, wdata->x, wdata->y, YELLOW);
            break;
        }
        }
    }

    gfx.Draw(canvas, MARGIN_X, MARGIN_Y);
}
#endif