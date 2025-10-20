/*🕹️ systems.h*/
#ifndef _SYSTEMS_C_
#define _SYSTEMS_C_

#include "../etc/env"
#include "../lib/Engine2D.h"

#include "Entities.h"
#include "Components.c"

void InputSystem(World_ECS *w, int key)
{
    int snakeId = 0;
    while (snakeId < w->count && (&(w->entities[snakeId])->type) != ENTITY_SNAKE)
    {
        snakeId++;
    };

    if (snakeId < w->count)
    {
        Entity_ECS *e = &w->entities[snakeId];
        SnakeData *sd = (SnakeData *)e->data;
        PointData *head = &sd->body[0];

        switch (key)
        {
        case KEY_LEFT:
            e->vel.dx = -1;
            break;
        case KEY_RIGHT:
            e->vel.dx = 1;
            break;
        case KEY_UP:
            e->vel.dy = -1;
            break;
        }
    }
}

void PhysicsSystem(World_ECS *w)
{
    for (int i = 0; i < w->count; i++)
    {
        Entity_ECS *e = &w->entities[i];
        if (!e || !e->activo)
            continue;
        if (e->type == ENTITY_SNAKE)
        {
            SnakeData *s = (SnakeData *)e->data;
            // Verificar si alguna parte está apoyada
            int supported = 0;
            for (int j = 0; j < s->length; j++)
            {
                if (IsSolidAt(w, s->body[j].x, s->body[j].y + 1, e))
                {
                    supported = 1;
                    break;
                }
            }
            // Determinar caída: solo si ninguna parte tiene apoyo
            int fall = supported ? 0 : 1;

            // Mover cola siguiendo la cabeza
            if (e->vel.dx || e->vel.dy || fall)
            {
                for (int j = s->length; j > 0; j--)
                {
                    if (!supported)
                    {
                        s->body[j].y += 1; // toda la serpiente cae
                    }
                    else
                    {
                        s->body[j].x = s->body[j - 1].x;
                        s->body[j].y = s->body[j - 1].y;
                    }
                }

                // Mover cabeza
                s->body[0].x += e->vel.dx;
                s->body[0].y += fall ? fall : e->vel.dy;
            }

            // Reset de velocidades temporales
            e->vel.dx = 0;
            e->vel.dy = 0;
        }
    }
}

int CollisionSystem(World_ECS *w)
{
    for (int i = 0; i < w->count; i++)
    {
        Entity_ECS *e = &w->entities[i];
        if (e->type != ENTITY_SNAKE || !e->activo)
            continue;

        SnakeData *sd = (SnakeData *)e->data;
        PointData *head = &sd->body[0];

        // 1. Caída al vacío
        if (head->y > SCREEN_HEIGHT)
        {
            cleaner();
            printf("💀 Has caído al vacío...");
            return 0;
        }

        // 2. Comer comida /Portal
        for (int j = 0; j < w->count; j++)
        {
            Entity_ECS *f = &w->entities[j];
            if (f->activo)
            {
                if (f->type == ENTITY_FOOD)
                {
                    PointData *p = (PointData *)f->data;
                    if (p->x == head->x && p->y == head->y)
                    {
                        f->activo = 0;
                        if (sd->length < MAX_BODY_SNAKE)
                        {
                            sd->length++; // añadir segmento
                        }
                    }
                }

                // Colisión con salida/portal
                if (f->type == ENTITY_EXIT)
                {
                    PointData *p = (PointData *)f->data;
                    if (p->x == head->x && p->y == head->y)
                    {
                        gotoXY(1, SCREEN_HEIGHT + 3);
                        printf("- Has alcanzado la salida, cargando nuevo mapa...");
                        // Llamamos a la función de cargar mapa
                        InitWorld(w); // reinicia mundo, se puede mejorar para distintos niveles
                        return 1;
                    }
                }
            }
        }
    }
}

void RenderSystem(World_ECS *w, Graphic *gfx, Canvas *c)
{
    gfx->Clear(c, BLACK);

    for (int i = 0; i < w->count; i++)
    {
        Entity_ECS *e = &w->entities[i];
        if (!e->activo)
            continue;

        if (e->type == ENTITY_SNAKE)
        {
            SnakeData *sd = (SnakeData *)e->data;
            for (int j = 0; j < sd->length; j++)
            {
                gfx->SetPixel(c, sd->body[j].x, sd->body[j].y, GREEN);
            }
        }
        else
        {
            PointData *p = (PointData *)e->data;
            BYTE color = WHITE;
            if (e->type == ENTITY_WALL)
                color = LIGHTGREY;
            if (e->type == ENTITY_FOOD)
                color = LIGHTRED;
            if (e->type == ENTITY_PLATFORM)
                color = YELLOW;
            if (e->type == ENTITY_EXIT)
                color = CYAN;
            gfx->SetPixel(c, p->x, p->y, color);
        }
    }

    gfx->Draw(c, 2, 2);
}
#endif