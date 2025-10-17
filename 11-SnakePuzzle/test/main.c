// gcc -o snake.exe main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*⚡ env*/
#ifndef _ENV_H_
#define _ENV_H_

#define allocateArray(A, B) (A *)malloc(sizeof(A) * ((B > 0) ? (B) : 1))
#define allocate(A) (A *)malloc(sizeof(A))

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif

#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 20

#define MARGIN_X 2
#define MARGIN_Y 2

#define MAX_ENTITIES 128
#define MAX_BODY_SNAKE 20

#define SNAKE_LENGTH 5
// #define LINUX       // Si quieres compilar en linux, por defecto es para windows
#endif

/*🕹️ Keyboard.h*/
#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_
#ifdef LINUX
// Codigo para LINUX
#include <unistd.h> //
#define Sleep(a) usleep(a * 1000)

#include <fcntl.h>
#include <termios.h>
// Captura de teclado
static int _initialized_ = 0;
void init_kbhit()
{
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
    _initialized_ = 1;
}

int _kbhit()
{
    if (!_initialized_)
    {
        init_kbhit();
    }
    int ch = getchar();
    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }
}
#else
// Codigo para WINDOWS
#include <conio.h>   // _kbhit y _getch
#include <windows.h> //Sleep
#endif

// Constante para GamePAD
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

#define KEY_ESC 27
#define KEY_SPACE 32
#define KEY_ENTER 10
#define KEY_A 97
#define KEY_B 115
#define KEY_EMPTY 0

int press()
{
    int key = KEY_EMPTY;
    if (_kbhit())
    {
        key = _getch();
        if (key == 224)
        {
            key = _getch();
        }
        switch (key)
        {
        case KEY_UP:
        case KEY_DOWN:
        case KEY_RIGHT:
        case KEY_LEFT:
        case KEY_ESC:
        case KEY_SPACE:
        case KEY_ENTER:
        case KEY_A:
        case KEY_B:
            return key;
        }
    }
    return KEY_EMPTY;
}

#endif

/*🧱 canvas.h*/
#ifndef _CANVAS_H_
#define _CANVAS_H_

typedef struct
{
    BYTE color; // foreground (4 bits altos) | background (4 bits bajos)
} Pixel;

typedef struct
{
    int width;
    int height;        // Altura física (en píxeles)
    int logicalHeight; // Altura lógica (en caracteres)
    Pixel *buffer;
} Canvas;

// Funciones base del canvas
static inline Canvas *Canvas_Create(int width, int height)
{
    Canvas *c = allocate(Canvas);
    if (!c)
        return NULL;
    c->width = width;
    c->height = height;
    c->logicalHeight = height / 2;
    int length = width * c->logicalHeight;
    c->buffer = allocateArray(Pixel, length);
    memset(c->buffer, 0, length * sizeof(Pixel));
    return c;
}

static inline void Canvas_Destroy(Canvas *c)
{
    if (c)
    {
        free(c->buffer);
        free(c);
    }
}

#endif

/*🎨 graphic.h*/
#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#define colorGraphic(front, back) printf("\e[%d;%d;%dm", (front & 0x8 ? 1 : 0), ((back & 0x7) + (back & 0x8 ? 100 : 40)), ((front & 0x7) + 30))
#define resetColor() printf("\e[0m")
#define gotoXY(X, Y) printf("\e[%d;%dH", (Y), (X))
#define hidecursor() printf("\e[?25l")
#define showcursor() printf("\e[?25h")
#define cleaner() printf("\e[1;1H\e[2J\e[0m")

#define CHAR_UPPER 0xdf

// COLORES
#define BLACK 0x0
#define RED 0x1
#define GREEN 0x2
#define YELLOW 0x3
#define BLUE 0x4
#define MAGENTA 0x5
#define CYAN 0x6
#define LIGHTGREY 0x7
#define DARKGREY 0x8
#define LIGHTRED 0x9
#define LIGHTGREEN 0xa
#define LIGHTYELLOW 0xb
#define LIGHTBLUE 0xc
#define LIGHTMAGENTA 0xd
#define LIGHTCYAN 0xe
#define WHITE 0xf

typedef struct
{
    void (*Clear)(Canvas *, BYTE);
    void (*SetPixel)(Canvas *, int, int, BYTE);
    void (*SetLine)(Canvas *, int, int, int, int, BYTE);
    void (*SetLineDot)(Canvas *, int, int, int, int, BYTE);
    void (*Draw)(Canvas *, int, int);
} Graphic;

static void Graphic_UpdatePixelColor(Canvas *c, int x, int logicalY, BYTE newColor, int isUpper)
{
    int index = logicalY * c->width + x;
    if (isUpper)
        c->buffer[index].color = (newColor << 4) | (c->buffer[index].color & 0x0F);
    else
        c->buffer[index].color = (c->buffer[index].color & 0xF0) | (newColor & 0x0F);
}

static void Graphic_Clear(Canvas *c, BYTE color)
{
    int length = c->width * c->logicalHeight;
    BYTE combined = (color << 4) | color;
    // memset(c->buffer, (color << 4) | color, length * sizeof(Pixel));
    for (int i = 0; i < length; i++)
        c->buffer[i].color = combined;
}

static void Graphic_SetPixel(Canvas *c, int x, int y, BYTE color)
{
    if (x >= 0 && x < c->width && y >= 0 && y < c->height)
    {
        int logicalY = y / 2;
        int isUpper = (y % 2 == 0);
        Graphic_UpdatePixelColor(c, x, logicalY, color, isUpper);
    }
}

static void Graphic_SetLine(Canvas *c, int x0, int y0, int x1, int y1, BYTE color)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1)
    {
        Graphic_SetPixel(c, x0, y0, color);
        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

static void Graphic_SetLineDot(Canvas *c, int x0, int y0, int x1, int y1, BYTE color)
{
    int drawable = 1;
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1)
    {
        if (drawable)
            Graphic_SetPixel(c, x0, y0, color);

        drawable = !drawable;

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

static void Graphic_Draw(Canvas *c, int startX, int startY)
{
    for (int row = 0; row < c->logicalHeight; row++)
    {
        gotoXY(startX, startY + row);
        for (int col = 0; col < c->width; col++)
        {
            Pixel p = c->buffer[row * c->width + col];
            colorGraphic((p.color >> 4) & 0x0F, p.color & 0x0F);
            putchar(CHAR_UPPER);
        }
    }
    resetColor();
}

// Inicializador
static inline Graphic Graphic_Init(void)
{
    Graphic g;
    g.Clear = Graphic_Clear;
    g.SetPixel = Graphic_SetPixel;
    g.SetLine = Graphic_SetLine;
    g.SetLineDot = Graphic_SetLineDot;
    g.Draw = Graphic_Draw;
    return g;
}

#endif

/*🧩 Entities.h*/
#ifndef _ENTITY_H_
#define _ENTITY_H_

typedef enum
{
    ENTITY_SNAKE, // 🐍 Snake
    ENTITY_FOOD,  // 🍎 Food
    ENTITY_WALL   // 🧱 Wall
} TypeEntity;

typedef struct
{
    int x, y;
} PointData;

typedef struct
{
    PointData body[MAX_BODY_SNAKE];
    int length;
} SnakeData;

typedef struct
{
    int dx, dy;
    int activo;
} Velocity;

typedef struct
{
    int activo;
    TypeEntity type;
    void *data;   // apunta a la estructura concreta
    Velocity vel; // opcional
} Entity_ECS;

typedef struct
{
    Entity_ECS *entities[MAX_ENTITIES]; // OJO Array de punteros
    int count;
} World_ECS;

static inline void World_Init(World_ECS *w)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
        w->entities[i] = NULL;
    w->count = 0;
}

// Crear nueva entidad y devolver índice
static inline int World_CreateEntity(World_ECS *w, Entity_ECS *e)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (w->entities[i] == NULL)
        {
            w->entities[i] = e;
            w->count++;
            return i;
        }
    }
    return -1; // no hay espacio
}

// Destruir entidad
void DestroyEntity(Entity_ECS *e)
{
    if (!e)
        return;
    free(e->data);
    free(e);
}
#endif

/* 🧰 Component.c*/
#ifndef _COMPONENTS_C_
#define _COMPONENTS_C_

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

/*⚙️ systems.h*/
#ifndef _SYSTEMS_C_
#define _SYSTEMS_C_

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

            // Wrap - Atrevesar Paredes ..
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

/*🧠 main.c*/
#include <time.h>

int main()
{
    cleaner();
    hidecursor();

    Canvas *canvas = Canvas_Create(SCREEN_WIDTH, SCREEN_HEIGHT);
    Graphic gfx = Graphic_Init();
    World_ECS world;
    World_Init(&world);
    srand((unsigned)time(NULL));

    // Crear paredes en los bordes
    CreateBorderWalls(&world, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Crear serpiente
    Entity_ECS *snake = CreateSnake(SNAKE_LENGTH, 10, 10);
    int snakeId = World_CreateEntity(&world, snake);

    // Crear comida
    Entity_ECS *food = CreateFood(rand() % ( SCREEN_WIDTH-2) +1, rand() % (SCREEN_HEIGHT-2)+1);
    World_CreateEntity(&world, food);

    int score = 0;
    int running = 1;

    while (running)
    {
        SystemInput(&world, snakeId);
        SystemMovement(&world, SCREEN_WIDTH, SCREEN_HEIGHT);

        if (!SystemCollision(&world, snakeId))
        {
            running = 0;
        }

        SystemRender(&world, gfx, canvas);

        gotoXY(MARGIN_X, SCREEN_HEIGHT + MARGIN_Y);
        resetColor();
        SnakeData *s = (SnakeData *)snake->data;
        score = s->length - SNAKE_LENGTH;
        printf("Score: %d", score);

        if (press() == KEY_ESC)
        {
            running = 0;
        }
        Sleep(400);
    }

    // Liberar memoria
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (world.entities[i])
            DestroyEntity(world.entities[i]);
    }

    showcursor();
    resetColor();
    cleaner();
    printf("Game Over! Score: %d\n", score);

    Canvas_Destroy(canvas);
    return 0;
}
