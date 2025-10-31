#include "ecs/ECS.h"
#include "etc/Game.c"

int main()
{
    World_ECS world;
    Component rules = Component_Init();
    System system = System_Init();
    Sprite_Init();

    hidecursor();
    cleaner();

    StateGame running = STATE_OPEN;
    int step = 0;

    do
    {
        switch (running)
        {
        case STATE_OPEN:
            Open_Render(step);
            step++;
            if (step == 32 || press() == KEY_ESC)
            {
                step = 0;
                running = STATE_MENU;
            }

            break;
        case STATE_MENU:
            int option = press();
            Menu_Render(step);
            step = (step + 1) % 255;
            if (option == KEY_ENTER)
            {
                step = 0;
                running = STATE_START;
                Game_init(&rules, &world);
                resetColor();
                cleaner();
            }
            if (option == KEY_ESC)
            {
                running = STATE_EXIT;
            }

            break;
        case STATE_START:

            Game_Loop(&system, &rules, &world, &running);
            if (running != STATE_START)
            {
                step = 0;
            }
            break;
        case STATE_OVER:
            running = STATE_OPEN;
            resetColor();
            cleaner();
            break;
        case STATE_EXIT:
            rules.world.Destroy(&world);
        default:
            resetColor();
            cleaner();
            break;
        }

        Sleep(50);
    } while (running != STATE_EXIT);

    showcursor();
    resetColor();
    gotoXY(2, (SCREEN_HEIGHT / 2) + 2);
    printf("Juego terminado.\n");
    return 0;
}
