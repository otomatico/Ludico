/*🧩 Entities.h*/
#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "../etc/env"

typedef enum
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

typedef struct
{
    int dx, dy;
    int activo;
} Velocity;

typedef struct
{
    int active;
    TypeEntity type;
    Velocity vel; // opcionalmente obligatoria
    void *data;   // apunta a la estructura concreta
} Entity_ECS;

typedef struct
{
    int id;
    int count;
    Entity_ECS *player; // Extrair Jugador .. reduz complejidad
    Entity_ECS *entities[MAX_ENTITIES]; // OJO Array de punteros
} World_ECS;

#endif
