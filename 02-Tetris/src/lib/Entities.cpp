#ifndef _ENTITIES_H_
#define _ENTITIES_H_

enum StateGame
{
    OPENING,
    MENU,
    START,
    PAUSE,
    OVER,
    ENDING,
    EXIT
};

struct Point
{
    int x;
    int y;
};
#endif