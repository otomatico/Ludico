#include "../src/etc/env"
#include "../src/lib/Engine2D.h"
#include "../src/etc/Sprite.c"

int main()
{
    Canvas *canvasMain = Canvas_Create(SCREEN_WIDTH, SCREEN_HEIGHT);
    Graphic *gfx = Graphic_Init();
    CanvasLayerStack LayerDB = {0,NULL};
    LayerDB.count = 2;
    LayerDB.list = allocateArray(Canvas*,2);
    for(int index=0; index<2; index++){
        LayerDB.list[index]=Sprite_Load(DBSprite[index]);
    }

    Layer *layer = Layer_Init();

    hidecursor();
    cleaner();

    gfx->Clear(canvasMain,BLACK);

    layer->MergeCanvas(canvasMain, LayerDB.list[0], 0, 7);
    layer->MergeCanvas(canvasMain, LayerDB.list[1], 6, 6);

    for (int i = 1; i < 18; i++)
    {
        layer->ScrollX(canvasMain, 2, 0);
        gfx->Draw(canvasMain, 1, 1);
        Sleep(50);
    }
    gotoXY(SCREEN_WIDTH-16, 10);
    colorGraphic(WHITE, BLACK);
    printf("8th Son Studio %c",184);
    showcursor();
    resetColor();
    printf("\n\n\n");
    
    return 0;
}
