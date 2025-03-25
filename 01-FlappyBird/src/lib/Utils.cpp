#include <iostream>   // printf
#include "display.h"  // gotoxy, textcolor, textbackground
#include "keyboard.h" //_kbhit, _getch, press
#include <stdlib.h>   // srand, rand

#ifndef _UTILS_H_
#include "Utils.hpp"
#endif

typedef enum
{
    OPENING,
    MENU,
    START,
    PAUSE,
    OVER,
    ENDING,
    EXIT
} StateGame;
typedef struct
{
    short x;
    short y;
} Point;

// Base class
class BasePlayer
{
public:
    Point GetPosition()
    {
        return position;
    }
    bool Collide(Point *value)
    {
        return value->x == position.x && value->y == position.y;
    }
    // virtual void Move(char *key) = 0;

protected:
    Point position;
};

void DrawBarVertical(short x, short y, short size, char c)
{
    for (short index = 0; index < size; index++)
    {
        DrawChar(x, y + index, c);
    }
}
void DrawBarHorizontal(short x, short y, short size, char c)
{
    gotoxy(x, y);
    for (short index = 0; index < size; index++)
    {
        printf("%c", c);
    }
}
void DrawWindow(short x, short y, short width, short height)
{
    height--;
    DrawChar(x, y, 0xc9);
    DrawBarHorizontal(x + 1, y, width - 1, 0xcd);
    DrawChar(x + width, y, 0xbb);
    DrawBarVertical(x + width, y + 1, height - 1, 0xba);
    DrawChar(x + width, y + height, 0xbc);
    DrawBarHorizontal(x + 1, y + height, width - 1, 0xcd);
    DrawChar(x, y + height, 0xc8);
    DrawBarVertical(x, y + 1, height - 1, 0xba);
}
