/*MapEntity.h*/
#ifndef _ENTITY_MAP_H_
#define _ENTITY_MAP_H_

#include "EntitySnake.h"

typedef enum
{
    APOINT,
    LINE_HORIZONTAL,
    LINE_VERTICAL
} TypeDraw;

typedef struct
{
    TypeDraw draw;
    TypeEntity entity;
    int x;
    int y;
    int lenght;
} EntityDraw;

typedef struct
{
    int lenght;
    EntityDraw *tiles;
} MapEntity;
#endif