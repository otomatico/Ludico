#ifndef _COMPONENT_PLAYER_C_
#define _COMPONENT_PLAYER_C_

#include "EntitySnake.h"
// Component Player

// Inicializar Player
static inline void Player_Init(Entity_ECS *e, int x, int y)
{
    e = allocate(Entity_ECS);
    e->type = ENTITY_SNAKE;
    e->active = 1;

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

// Colisión snake-tail
static inline TypeEntity SnakeSelfCollision(Entity_ECS *player, PointData *position)
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

// Move Player
static inline void Player_Move(Entity_ECS *player, int supported)
{
    SnakeData *snake = (SnakeData *)player->data;
    if (!supported)
    {
        player->vel.dx = 0;
        player->vel.dy = GRAVITY;
    }
    // Mover cola siguiendo la cabeza
    if (player->vel.dx || player->vel.dy)
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
        snake->body[0].y += player->vel.dy;
    }
}

typedef struct
{
    void (*Initialize)(Entity_ECS *, int x, int y);
    void (*Moviment)(Entity_ECS *, int);
    TypeEntity (*CollideSelf)(Entity_ECS *player, PointData *position);
    void (*Destroy)(Entity_ECS *);
} ComponentPlayer;

ComponentPlayer ComponentPlayer_Init(void)
{
    ComponentPlayer player;

    player.Initialize = Player_Init;
    player.Moviment = Player_Move;
    player.CollideSelf = SnakeSelfCollision;
    player.Destroy = Entity_Destroy;
    return player;
}

#endif