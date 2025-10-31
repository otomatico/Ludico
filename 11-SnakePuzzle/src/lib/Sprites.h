#include "../etc/env"
#include "Engine2D.h"
typedef enum
{
    DRAW_PIXEL,
    DRAW_LINE,
    DRAW_RECT
} GraphicDraw;
typedef struct
{
    GraphicDraw tag;
    BYTE color;
    int x0;
    int y0;
    int x1;
    int y1;
} GraphicCommand;

typedef struct
{
    int lenght;
    int size; //{0:8X8,1:8X16,2:16X8,3:16X16}
    GraphicCommand *command;
} SpriteStack;

Canvas * Sprite_Load(SpriteStack *sprite)
{
    int sizeSprite[4][2] = {{8, 8}, {8, 16}, {16, 8}, {16, 16}};
    Canvas *buffer = Canvas_Create(sizeSprite[sprite->size][0], sizeSprite[sprite->size][1]);
    for (int index = 0; index < sprite->lenght; index++)
    {
        GraphicCommand *line = &(sprite->command[index]);
        switch (line->tag)
        {
        case DRAW_PIXEL:
            Graphic_SetPixel(buffer, line->x0, line->y0, line->color);
            break;
        case DRAW_LINE:
            Graphic_SetLine(buffer, line->x0, line->y0, line->x1, line->y1, line->color);
            break;
        case DRAW_RECT:
            Graphic_SetRectangle(buffer, line->x0, line->y0, line->x1, line->y1, line->color);
            break;
        }
    }
    return buffer;
}