#include "../src/etc/env"
#include "../src/lib/Engine2D.h"

int main()
{
    Canvas *canvas = Canvas_Create(15, 8);
    Canvas *canvasB = Canvas_Create(SCREEN_WIDTH, SCREEN_HEIGHT);
    Graphic gfx = Graphic_Init();

    hidecursor();
    cleaner();
    gfx.Clear(canvasB,BLACK);
    int halfX =SCREEN_WIDTH/2;
    int halfY =SCREEN_HEIGHT/2;
    for(int c= 0; c<SCREEN_WIDTH;c++)
    {
        gfx.SetLine(canvasB,0,halfY,c+1,halfY,GREEN);
        gfx.SetLine(canvasB,0,halfY+1,c,halfY+1,YELLOW);
        gfx.SetLine(canvasB,0,halfY+2,c+1,halfY+2,BLUE);
        
        gfx.Draw(canvasB,1,1);
        Sleep(100);
    }

    gfx.Clear(canvas,BLACK);

    gfx.SetLine(canvas,1,0,13,0,WHITE);
    gfx.SetRectangle(canvas,0,1,14,4,WHITE);
    gfx.SetLine(canvas,1,5,13,5,WHITE);
    gfx.SetLine(canvas,1,6,13,6,DARKGREY);
    gfx.SetPixel(canvas,14,5,DARKGREY);
    gfx.SetPixel(canvas,0,5,DARKGREY);

    gfx.SetRectangle(canvas,4,1,6,4,LIGHTBLUE);
    gfx.SetRectangle(canvas,4,1,5,3,LIGHTCYAN);

    gfx.SetPixel(canvas,1,3,MAGENTA);
    gfx.SetPixel(canvas,2,4,MAGENTA);

    gfx.SetPixel(canvas,12,4,RED);
    gfx.SetPixel(canvas,13,3,RED);

    gfx.SetPixel(canvas,2,1,LIGHTGREEN);
   
    gfx.SetPixel(canvas,12,1,DARKGREY);
    gfx.SetPixel(canvas,13,1,LIGHTGREY);


    gfx.Draw(canvas,halfX-6,halfY-5);

    showcursor();
    resetColor();
    printf("\n\n\n");
    return 0;
}

