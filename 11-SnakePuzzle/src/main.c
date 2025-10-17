/*🎮 main.c*/
#include "ecs/ECS.h"
#include <stdlib.h>
#include <time.h>

#define SNAKE_LENGTH 5
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
    Entity_ECS *food = CreateFood(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
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
            running = 0;

        Sleep(500);
    }

    // Liberar memoria
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (world.entities[i])
            DestroyEntity(world.entities[i]);
    }

    showcursor();
    resetColor();
    // cleaner();
    printf("Game Over! Score: %d\n", score);

    Canvas_Destroy(canvas);
    return 0;
}
