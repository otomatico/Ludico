//.\build.bat ../test/plataform plataform
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // usleep()
#include "../src/lib/Engine2D.h"

#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 20
#define GRAVITY 1
#define GROUND_Y (SCREEN_HEIGHT - 2)
#define MAX_ENTITIES 128
#define MAX_BODY_SNAKE 10

int running = 1;

//------------------------------------------------------------
// Tipos de datos específicos
//------------------------------------------------------------
typedef struct
{
    int x, y;
} PointData;

typedef struct
{
    PointData body[MAX_BODY_SNAKE];
    int length;
} SnakeData;

typedef enum
{
    ENTITY_NONE,
    ENTITY_SNAKE,
    ENTITY_FOOD,
    ENTITY_WALL,
    ENTITY_PLATFORM,
    ENTITY_EXIT
} EntityType;

typedef struct
{
    EntityType type;
    int active;
    int gravityEnabled;
    int vx, vy;
    void *data; // SnakeData o PointData
} Entity_ECS;

typedef struct
{
    Entity_ECS entities[MAX_ENTITIES];
    int entityCount;
} World_ECS;

//------------------------------------------------------------
// Utilidades ECS
//------------------------------------------------------------
Entity_ECS CreateEntity(EntityType type, int x, int y)
{
    Entity_ECS e = {0};
    e.type = type;
    e.active = 1;

    if (type == ENTITY_SNAKE)
    {
        SnakeData *sd = malloc(sizeof(SnakeData));
        sd->length = 3;
        for (int i = 0; i < sd->length; i++)
        {
            sd->body[i].x = x - i;
            sd->body[i].y = y;
        }
        e.data = sd;
        e.gravityEnabled = 1;
    }
    else
    {
        PointData *p = malloc(sizeof(PointData));
        p->x = x;
        p->y = y;
        e.data = p;
        e.gravityEnabled = 0;
    }
    return e;
}

void DestroyWorld(World_ECS *w)
{
    for (int i = 0; i < w->entityCount; i++)
    {
        if (w->entities[i].data)
            free(w->entities[i].data);
    }
}

//------------------------------------------------------------
// Colisiones
//------------------------------------------------------------
int IsSolidAt(World_ECS *w, int x, int y, Entity_ECS *ignore)
{
    for (int i = 0; i < w->entityCount; i++)
    {
        Entity_ECS *e = &w->entities[i];
        if (!e->active || e == ignore)
            continue;

        if (e->type == ENTITY_WALL || e->type == ENTITY_PLATFORM)
        {
            PointData *p = (PointData *)e->data;
            if (p->x == x && p->y == y)
                return 1;
        }
        else if (e->type == ENTITY_SNAKE)
        {
            SnakeData *sd = (SnakeData *)e->data;
            for (int j = 0; j < sd->length; j++)
                if (sd->body[j].x == x && sd->body[j].y == y)
                    return 1;
        }
    }
    return 0;
}

//------------------------------------------------------------
// Sistemas
//------------------------------------------------------------
//------------------------------------------------------------
// Escenario inicial
//------------------------------------------------------------
void InitWorld(World_ECS *w)
{
    w->entityCount = 0;

    // Snake
    w->entities[w->entityCount++] = CreateEntity(ENTITY_SNAKE, 8, GROUND_Y - 1);

    // Piso
    for (int i = 5; i < SCREEN_WIDTH - 5; i++)
        w->entities[w->entityCount++] = CreateEntity(ENTITY_WALL, i, GROUND_Y);

    // Plataforma central
    for (int i = 10; i < 20; i++)
        w->entities[w->entityCount++] = CreateEntity(ENTITY_PLATFORM, i, GROUND_Y - 3);

    // Comida
    w->entities[w->entityCount++] = CreateEntity(ENTITY_FOOD, 8, GROUND_Y - 2);

    // Salida
    w->entities[w->entityCount++] = CreateEntity(ENTITY_EXIT, 23, GROUND_Y - 4);
}

void InputSystem(World_ECS *w, int key)
{
    int snakeId = 0;
    while (snakeId < w->entityCount && (&w->entities[snakeId])->type != ENTITY_SNAKE)
    {
        snakeId++;
    };

    if (snakeId < w->entityCount)
    {
        Entity_ECS *e = &w->entities[snakeId];
        SnakeData *sd = (SnakeData *)e->data;
        PointData *head = &sd->body[0];

        switch (key)
        {
        case KEY_LEFT:
            e->vx = -1;
            break;
        case KEY_RIGHT:
            e->vx = 1;
            break;
        case KEY_UP:
            e->vy = -1;
            break;
        case KEY_ESC:
            running = 0;
            break;
        }
    }
}

void PhysicsSystem(World_ECS *w)
{
    for (int i = 0; i < w->entityCount; i++)
    {
        Entity_ECS *e = &w->entities[i];
        if (!e || !e->active)
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
            if (e->vx || e->vy || fall)
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
                s->body[0].x += e->vx;
                s->body[0].y += fall ? fall : e->vy;
            }

            // Reset de velocidades temporales
            e->vx = 0;
            e->vy = 0;
        }
    }
}

//------------------------------------------------------------
// Colisiones y crecimiento
//------------------------------------------------------------
void CollisionSystem(World_ECS *w)
{
    for (int i = 0; i < w->entityCount; i++)
    {
        Entity_ECS *e = &w->entities[i];
        if (e->type != ENTITY_SNAKE || !e->active)
            continue;

        SnakeData *sd = (SnakeData *)e->data;
        PointData *head = &sd->body[0];

        // 1. Caída al vacío
        if (head->y > SCREEN_HEIGHT)
        {
            cleaner();
            printf("\n💀 Has caído al vacío...\n");
            running = 0;
            return;
        }

        // 2. Comer comida /Portal
        for (int j = 0; j < w->entityCount; j++)
        {
            Entity_ECS *f = &w->entities[j];
            if (f->active)
            {
                if (f->type == ENTITY_FOOD)
                {
                    PointData *p = (PointData *)f->data;
                    if (p->x == head->x && p->y == head->y)
                    {
                        f->active = 0;
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
                        return;
                    }
                }
            }
        }
    }
}

void RenderSystem(World_ECS *w, Graphic *gfx, Canvas *c)
{
    gfx->Clear(c, BLACK);

    for (int i = 0; i < w->entityCount; i++)
    {
        Entity_ECS *e = &w->entities[i];
        if (!e->active)
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

//------------------------------------------------------------
// Programa principal
//------------------------------------------------------------
int main()
{
    Canvas *canvas = Canvas_Create(SCREEN_WIDTH, SCREEN_HEIGHT);
    Graphic gfx = Graphic_Init();

    World_ECS world;
    InitWorld(&world);

    hidecursor();
    cleaner();

    while (running)
    {
        int key = press();
        InputSystem(&world, key);
        CollisionSystem(&world);
        PhysicsSystem(&world);
        RenderSystem(&world, &gfx, canvas);
        usleep(8000);
    }

    DestroyWorld(&world);
    showcursor();
    resetColor();
    cleaner();
    printf("Juego terminado.\n");
    return 0;
}
