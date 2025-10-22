/*🕹️ systems.h*/
#ifndef _SYSTEMS_C_
#define _SYSTEMS_C_

#include "../etc/mapTiles.c"
#include "../lib/Engine2D.h"
#include "Components.c"

void MapSystem(World_ECS *w, ComponentWorld *rules, int level)
{
    rules->Destroy(w);
    rules->Initialize(w);
    w->id = level;
    MapEntity *Map = Tiles[level % 2];
    for (int index = 0; index < Map->lenght; index++)
    {
        EntityDraw *line = &(Map->tiles[index]);
        switch (line->draw)
        {
        case APOINT:
            rules->CreateEntity(w, line->entity, line->x, line->y);
            break;
        case LINE_HORIZONTAL:
            for (int x = 0; x < line->lenght; x++)
            {
                rules->CreateEntity(w, line->entity, line->x + x, line->y);
            }
            break;
        case LINE_VERTICAL:
            for (int y = 0; y < line->lenght; y++)
            {
                rules->CreateEntity(w, line->entity, line->x, line->y + y);
            }
            break;
        }
    }
}

void InputSystem(Entity_ECS *player, int key)
{
    switch (key)
    {
    case KEY_LEFT:
        player->vel.dx = -1;
        break;
    case KEY_RIGHT:
        player->vel.dx = 1;
        break;
    case KEY_UP:
        player->vel.dy = -1;
        break;
    case KEY_DOWN:
        player->vel.dy = 1;
        break;
    }
}

inline void PhysicsSystem(World_ECS *w, ComponentWorld *rules, ComponentPlayer *play)
{
    Entity_ECS *player = w->player;
    SnakeData *snake = (SnakeData *)player->data;
    // Verificar si alguna parte está apoyada
    int supported = 0;
    PointData position;
    // Esta soportado por algun elemento
    for (int index = 0; index < snake->length && !supported; index++)
    {
        position.x = snake->body[index].x;
        position.y = snake->body[index].y + 1;
        if (rules->Collide(w, &position) != ENTITY_NONE)
        {
            supported = 1;
        }
    }

    PointData head = (PointData){snake->body[0].x + player->vel.dx, snake->body[0].y + player->vel.dy};
    // Evite subir sobre si mismo o traspasar paredes
    TypeEntity entity = rules->Collide(w, &head);
    // Si la posicion esta Ocupada
    if (play->CollideSelf(player, &head) == ENTITY_SNAKE)
    {
        player->vel.dx = 0;
        player->vel.dy = 0;
    }
    if (entity != ENTITY_PLATFORM && entity != ENTITY_ROCK)
    {
        play->Moviment(player, supported);
    }
    // Reset de velocidades temporales
    player->vel.dx = 0;
    player->vel.dy = 0;
}

inline int CollisionSystem(World_ECS *w, ComponentWorld *rules)
{
    SnakeData *snake = (SnakeData *)w->player->data;
    PointData *head = &snake->body[0];

    // Caída al vacío
    if (head->y > SCREEN_HEIGHT)
    {
        return 0;
    }
    TypeEntity hit = rules->Collide(w, head);
    switch (hit)
    {
    case ENTITY_EXIT:
        MapSystem(w, rules, w->id + 1);
        break;
    case ENTITY_FOOD:
        rules->EnabledEntity(w, head, 0);
        snake->length++;
        break;
    }
    return 1;
}

void RenderSystem(World_ECS *w, Graphic *gfx, Canvas *c)
{
    gfx->Clear(c, DARKGREY);
    for (int index = 0; index < w->count; index++)
    {
        Entity_ECS *e = w->entities[index];
        if (!e->active)
            continue;

        PointData *p = (PointData *)e->data;
        BYTE color = WHITE;
        if (e->type == ENTITY_SPIKE)
            color = LIGHTMAGENTA;
        if (e->type == ENTITY_ROCK)
            color = LIGHTGREY;
        if (e->type == ENTITY_FOOD)
            color = LIGHTRED;
        if (e->type == ENTITY_PLATFORM)
            color = YELLOW;
        if (e->type == ENTITY_EXIT)
            color = CYAN;
        gfx->SetPixel(c, p->x, p->y, color);
    }

    SnakeData *sd = (SnakeData *)w->player->data;
    gfx->SetPixel(c, sd->body[0].x, sd->body[0].y, BLUE);
    for (int index = 1; index < sd->length; index++)
    {
        gfx->SetPixel(c, sd->body[index].x, sd->body[index].y, GREEN);
        // gotoXY(2, (SCREEN_HEIGHT / 2) + 2 + index);printf("Player[%d]:{x:%d, y:%d}", index, sd->body[index].x, sd->body[index].y);
    }

    gfx->Draw(c, MARGIN_X, MARGIN_Y);
    gotoXY(SCREEN_WIDTH + MARGIN_X + 1, MARGIN_Y);
    printf("Level %d", w->id);
    resetColor();
}

typedef struct
{
    void (*WatchGamePad)(Entity_ECS *, int);

    void (*Physics)(World_ECS *, ComponentWorld *, ComponentPlayer *);
    int (*Collide)(World_ECS *, ComponentWorld *);

    void (*MapLoad)(World_ECS *, ComponentWorld *, int);
    void (*Render)(World_ECS *, Graphic *, Canvas *);
    // void (*Destroy)(World_ECS *);
    // void (*Destroy)(Component *);
} System;

static System System_Init(void)
{
    System sys;
    sys.WatchGamePad = InputSystem;
    sys.Physics = PhysicsSystem;
    sys.Collide = CollisionSystem;
    sys.MapLoad = MapSystem;
    sys.Render = RenderSystem;
    return sys;
}

#endif