
#include "display/ICanvas.h"
#include "input/keyboard.h" //Sleep(milesecond)
#ifndef _UTILS_H_
#define _UTILS_H_

#define CHAR_SOLID_25 0xb0
#define CHAR_SOLID_50 0xb1
#define CHAR_SOLID_75 0xb2
#define CONER_TOP_LEFT 0xc9
#define CONER_TOP_RIGHT 0xbb
#define CONER_DOWN_RIGHT 0xbc
#define CONER_DOWN_LEFT 0xc8
#define BAR_HORIZONTAL 0xcd
#define BAR_VERTICAL 0xba

void DrawBarVertical(short x, short y, short size, char c)
{
    for (short index = 0; index < size; index++)
    {
        DrawChar(x, y + index, c);
    }
}

void DrawBarHorizontal(short x, short y, short size, char c)
{
    gotoXY(x, y);
    for (short index = 0; index < size; index++)
    {
        printf("%c", c);
    }
}

void DrawWindow(short x, short y, short width, short height)
{
    height--;
    DrawChar(x, y, CONER_TOP_LEFT);
    DrawBarHorizontal(x + 1, y, width - 1, BAR_HORIZONTAL);
    DrawChar(x + width, y, CONER_TOP_RIGHT);
    DrawBarVertical(x + width, y + 1, height - 1, BAR_VERTICAL);
    DrawChar(x + width, y + height, CONER_DOWN_RIGHT);
    DrawBarHorizontal(x + 1, y + height, width - 1, BAR_HORIZONTAL);
    DrawChar(x, y + height, CONER_DOWN_LEFT);
    DrawBarVertical(x, y + 1, height - 1, BAR_VERTICAL);
}

#endif