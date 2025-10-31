#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "../etc/env"
#ifndef TypeEntity
typedef enum _TE TypeEntity;
#endif

typedef struct
{
    int dx, dy;
    int activo;
} Velocity;

typedef struct
{
    int active;
    int type;
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

// Destruir entidad
void Entity_Destroy(Entity_ECS *e)
{
    if (!e)
        return;
    free(e->data);
    free(e);
}
#endif
