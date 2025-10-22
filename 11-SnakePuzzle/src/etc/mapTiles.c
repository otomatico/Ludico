/*MapTitles.c*/
#ifndef _MAPTILES_H_
#define _MAPTILES_H_

#include "../ecs/MapEntity.h"

MapEntity Map01 = (MapEntity){5, (EntityDraw[5]){
                                     (EntityDraw){APOINT, ENTITY_SNAKE, 8, GROUND_Y - 2},
                                     (EntityDraw){APOINT, ENTITY_FOOD, 8, GROUND_Y - 3},
                                     (EntityDraw){LINE_HORIZONTAL, ENTITY_PLATFORM, 5, GROUND_Y - 1, SCREEN_WIDTH - 10},
                                     (EntityDraw){LINE_HORIZONTAL, ENTITY_PLATFORM, 10, GROUND_Y - 4, 10},
                                     (EntityDraw){APOINT, ENTITY_EXIT, 23, GROUND_Y - 5}}};

MapEntity Map02 = (MapEntity){5, (EntityDraw[5]){
                                     (EntityDraw){APOINT, ENTITY_SNAKE, 8, GROUND_Y - 2},
                                     (EntityDraw){APOINT, ENTITY_ROCK, 9, GROUND_Y - 2},
                                     (EntityDraw){LINE_HORIZONTAL, ENTITY_PLATFORM, 5, GROUND_Y - 1, 10},
                                     (EntityDraw){LINE_HORIZONTAL, ENTITY_PLATFORM, 10, GROUND_Y - 4, 10},
                                     (EntityDraw){APOINT, ENTITY_EXIT, 22, GROUND_Y - 3}}};

// Es una array de punteros que apuntan a los datos estaticos
MapEntity *Tiles[2] = {&Map01, &Map02};

#endif