#include "../ecs/ECS.h"
#include "../lib/Engine2D.h"
#include "Sprite.c"

typedef enum
{
    STATE_OPEN,
    STATE_MENU,
    STATE_START,
    STATE_PAUSE,
    STATE_OVER,
    STATE_END,
    STATE_EXIT
} StateGame;

Canvas *canvas = NULL;
Graphic *gfx = NULL;
Layer *layer = NULL;
CanvasLayerStack *SpriteDB = NULL;

void Sprite_Init(void)
{
    canvas = Canvas_Create(SCREEN_WIDTH, SCREEN_HEIGHT);
    gfx = Graphic_Init();
    layer = Layer_Init();
    //=================================================================
    SpriteDB = allocate(CanvasLayerStack);
    SpriteDB->count = MAX_STACKSPRITE;
    SpriteDB->list = allocateArray(Canvas *, 2);
    for (int index = 0; index < MAX_STACKSPRITE; index++)
    {
        SpriteDB->list[index] = Sprite_Load(DBSprite[index]);
    }
}

void spr(int index, int x, int y)
{
    layer->MergeCanvas(canvas, SpriteDB->list[index], x, y);
}

void Game_init(Component *rules, World_ECS *world)
{
    rules->world.Initialize(world);
    rules->player.Initialize(world->player, 0, 0);
    rules->map.Load(world, Tiles, &rules->world, 0);
}

void Game_Loop(System *system, Component *rules, World_ECS *world, StateGame *running)
{
    int key = press();
    system->WatchGamePad(world->player, key);
    int collide = system->Collide(world, rules);
    system->Physics(world, rules);
    system->Render(world, gfx, canvas);
    (*running) = (collide ? STATE_START : STATE_OVER);
    if (key == KEY_ESC)
    {
        *running = STATE_EXIT;
    }
}

void Open_Render(int step)
{
    if (step == 0)
    {
        gfx->Clear(canvas, BLACK);
        spr(0, 0, 7);
        spr(1, 6, 6);
    }

    if (0 <= step && step <= 18)
    {
        layer->ScrollX(canvas, 2, 0);
        gfx->Draw(canvas, MARGIN_X, MARGIN_Y);
    }
    if (step == 18)
    {
        gotoXY(SCREEN_WIDTH - 16, 11);
        colorGraphic(LIGHTGREY, BLACK);
        printf("8th Son Studio %c", 184);
    }
}

void Menu_Render(int step)
{
    if(step==0)
    {
        gfx->Clear(canvas, BLACK);
        spr(2, 11, 4);//centro en 6
        gfx->Draw(canvas, MARGIN_X, MARGIN_Y);
        gotoXY(SCREEN_WIDTH - 16, 11);
        colorGraphic(LIGHTGREY, BLACK);
        printf("8th Son Studio %c", 184);
    }

    gotoXY(18, 5);
    colorGraphic(BLACK, LIGHTGREEN);
    printf("cSnake");
    gotoXY(19, 6);
    colorGraphic(WHITE, GREEN);
    printf("Puzzle");

    gotoXY(13, 8);
    colorGraphic(step % 8 < 4 ? CYAN : YELLOW, BLACK);
    printf("Press Enter key");
}
