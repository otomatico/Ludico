#ifndef _ENUM_H_
#define _ENUM_H_

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
    int x;
    int y;
} Point;
#endif