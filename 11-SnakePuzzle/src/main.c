/*🎮 main.c*/
#include "ecs/ECS.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // usleep()

int main()
{
    Canvas *canvas = Canvas_Create(SCREEN_WIDTH, SCREEN_HEIGHT);
    Graphic gfx = Graphic_Init();

    World_ECS world;
    Component rules = Component_Init();
    System system = System_Init();

    rules.world.Initialize(&world);
    rules.player.Initialize(world.player,0,0);
    rules.map.Load(&world, Tiles, &rules.world, 0);
    hidecursor();
    cleaner();

    int running = 1;
    while (running)
    {
        int key = press();
        system.WatchGamePad(world.player, key);
        running = system.Collide(&world, &rules);
        system.Physics(&world, &rules);
        system.Render(&world, &gfx, canvas);
        Sleep(100);
        if(key == KEY_ESC){
            running = 0;
        }
    }

    rules.world.Destroy(&world);
    showcursor();
    resetColor();
    gotoXY(2, (SCREEN_HEIGHT/2)+2);
    printf("Juego terminado.\n");
    return 0;
}
