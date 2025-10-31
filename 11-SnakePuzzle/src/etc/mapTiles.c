/*MapTitles.c*/
#ifndef _MAPTILES_H_
#define _MAPTILES_H_

#include "../ecs/MapEntity.h"

MapEntity Map01 = (MapEntity){5, (EntityDraw[5]){
                                     (EntityDraw){APOINT, ENTITY_SNAKE, 8, GROUND_Y - 2},
                                     (EntityDraw){APOINT, ENTITY_FOOD, 9, GROUND_Y - 3},
                                     (EntityDraw){LINE_HORIZONTAL, ENTITY_PLATFORM, 5, GROUND_Y - 1, 17},
                                     (EntityDraw){LINE_HORIZONTAL, ENTITY_PLATFORM, 10, GROUND_Y - 4, 10},
                                     (EntityDraw){APOINT, ENTITY_EXIT, 23, GROUND_Y - 5}}};

MapEntity Map02 = (MapEntity){5, (EntityDraw[5]){
                                     (EntityDraw){APOINT, ENTITY_SNAKE, 8, GROUND_Y - 2},
                                     (EntityDraw){APOINT, ENTITY_ROCK, 9, GROUND_Y - 2},
                                     (EntityDraw){LINE_HORIZONTAL, ENTITY_PLATFORM, 5, GROUND_Y - 1, 10},
                                     (EntityDraw){LINE_HORIZONTAL, ENTITY_PLATFORM, 10, GROUND_Y - 4, 10},
                                     (EntityDraw){APOINT, ENTITY_EXIT, 22, GROUND_Y - 3}}};

MapEntity Map03 = (MapEntity){7, (EntityDraw[7]){
                                    (EntityDraw){APOINT, ENTITY_SPIKE, 9, GROUND_Y - 2},
                                    (EntityDraw){APOINT, ENTITY_FOOD, 5, GROUND_Y - 2},
                                    (EntityDraw){LINE_HORIZONTAL, ENTITY_PLATFORM, 6, GROUND_Y - 3, 3},
                                    (EntityDraw){LINE_HORIZONTAL, ENTITY_PLATFORM, 5, GROUND_Y - 1, 5},
                                    (EntityDraw){LINE_VERTICAL, ENTITY_PLATFORM, 10, GROUND_Y - 3, 3},
                                    (EntityDraw){APOINT, ENTITY_SNAKE, 8, GROUND_Y - 4},
                                    (EntityDraw){APOINT, ENTITY_EXIT, 13, GROUND_Y - 5}}
                                };

// Es una array de punteros que apuntan a los datos estaticos
MapEntity *Tiles[MAX_TILES] = {    &Map01,    &Map02,     &Map03,    &Map01};

#endif