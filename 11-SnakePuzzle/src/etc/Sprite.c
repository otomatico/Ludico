#include "../lib/Sprites.h"

SpriteStack Sprite01 =(SpriteStack){5,0,
    (GraphicCommand[5]){
        (GraphicCommand){DRAW_LINE,RED, 0, 0, 8, 0},
        (GraphicCommand){DRAW_LINE,YELLOW, 0, 1, 8, 1},
        (GraphicCommand){DRAW_LINE,GREEN, 0, 2, 8, 2},
        (GraphicCommand){DRAW_LINE,CYAN, 0, 3, 8, 3},
        (GraphicCommand){DRAW_LINE,BLUE, 0, 4, 8, 4}
    }
};
SpriteStack Sprite02 = (SpriteStack){15, 2,
            (GraphicCommand[15]){
                (GraphicCommand){DRAW_LINE, WHITE, 1, 0, 13, 0},
                (GraphicCommand){DRAW_RECT, WHITE, 0, 1, 14, 4},
                (GraphicCommand){DRAW_LINE, WHITE, 1, 5, 13, 5},
                (GraphicCommand){DRAW_LINE, DARKGREY, 1, 6, 13, 6},
                (GraphicCommand){DRAW_RECT, LIGHTBLUE, 4, 1, 6, 4},
                (GraphicCommand){DRAW_RECT, LIGHTCYAN, 4, 1, 5, 3},
                (GraphicCommand){DRAW_PIXEL, DARKGREY, 0, 5},
                (GraphicCommand){DRAW_PIXEL, DARKGREY, 14, 5},
                (GraphicCommand){DRAW_PIXEL, DARKGREY, 12, 1},
                (GraphicCommand){DRAW_PIXEL, MAGENTA, 1, 3},
                (GraphicCommand){DRAW_PIXEL, MAGENTA, 2, 4},
                (GraphicCommand){DRAW_PIXEL, RED, 12, 4},
                (GraphicCommand){DRAW_PIXEL, RED, 13, 3},
                (GraphicCommand){DRAW_PIXEL, LIGHTGREEN, 2, 1},
                (GraphicCommand){DRAW_PIXEL, LIGHTGREY, 13, 1}}
            };
SpriteStack Sprite03 = (SpriteStack){9, 2,
            (GraphicCommand[9]){
                (GraphicCommand){DRAW_LINE, LIGHTGREEN, 2, 1, 13, 1},
                (GraphicCommand){DRAW_LINE, LIGHTGREEN, 1, 2, 14, 2},
                (GraphicCommand){DRAW_LINE, LIGHTGREEN, 0, 3, 15, 3},
                (GraphicCommand){DRAW_LINE, GREEN, 0, 4, 15, 4},
                (GraphicCommand){DRAW_LINE, GREEN, 1, 5, 14, 5},
                (GraphicCommand){DRAW_LINE, GREEN, 2, 6, 13, 6},
                (GraphicCommand){DRAW_LINE, BLUE, 3, 7, 13, 7},
                (GraphicCommand){DRAW_LINE, BLUE, 0, 5, 2, 7},
                (GraphicCommand){DRAW_LINE, BLUE, 14, 6, 15, 5}
            }
            };

SpriteStack *DBSprite[MAX_STACKSPRITE] ={&Sprite01,&Sprite02,&Sprite03};