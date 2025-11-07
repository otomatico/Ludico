#include <genesis.h> // Para VDP_getScreenHeight()
#include "TileMap.h"
// 224 pixels de alto / 8 = 28 tiles
#define GROUND_Y 27
static const EntityDrawMap Map01_TilesData[5] = {
    {.draw = MAP_POINT, .entity = ENTITY_SNAKE, .x = 8, .y = GROUND_Y - 2, .lenght = 0},
    {.draw = MAP_POINT, .entity = ENTITY_FOOD, .x = 9, .y = GROUND_Y - 3, 0},
    {.draw = MAP_LINE_HORIZONTAL, .entity = ENTITY_PLATFORM, .x = 5, .y = GROUND_Y - 1, .lenght = 17},
    {.draw = MAP_LINE_HORIZONTAL, .entity = ENTITY_PLATFORM, .x = 10, .y = GROUND_Y - 4, .lenght = 10},
    {.draw = MAP_POINT, .entity = ENTITY_EXIT, .x = 23, .y = GROUND_Y - 5, .lenght = 0}};
static const EntityMap Map01 = {
    .lenght = 5,
    .tiles = (EntityDrawMap *)Map01_TilesData // Añade cast o declara .tiles como const
};

static const EntityDrawMap Map02_TilesData[5] = {
    {.draw = MAP_POINT, .entity = ENTITY_SNAKE, .x = 8, .y = GROUND_Y - 2, .lenght = 0},
    {.draw = MAP_POINT, .entity = ENTITY_ROCK, .x = 9, .y = GROUND_Y - 2, .lenght = 0},
    {.draw = MAP_LINE_HORIZONTAL, .entity = ENTITY_PLATFORM, .x = 5, .y = GROUND_Y - 1, .lenght = 10},
    {.draw = MAP_LINE_HORIZONTAL, .entity = ENTITY_PLATFORM, .x = 10, .y = GROUND_Y - 4, .lenght = 10},
    {.draw = MAP_POINT, .entity = ENTITY_EXIT, .x = 22, .y = GROUND_Y - 3, .lenght = 0}};
static const EntityMap Map02 = {
    .lenght = 5,
    .tiles = (EntityDrawMap *)Map02_TilesData // Añade cast o declara .tiles como const
};

static const EntityDrawMap Map03_TilesData[7] = {
    (EntityDrawMap){MAP_POINT, ENTITY_SPIKE, 9, GROUND_Y - 2, 0},
    (EntityDrawMap){MAP_POINT, ENTITY_FOOD, 5, GROUND_Y - 2, 0},
    (EntityDrawMap){MAP_LINE_HORIZONTAL, ENTITY_PLATFORM, 6, GROUND_Y - 3, 3},
    (EntityDrawMap){MAP_LINE_HORIZONTAL, ENTITY_PLATFORM, 5, GROUND_Y - 1, 5},
    (EntityDrawMap){MAP_LINE_VERTICAL, ENTITY_PLATFORM, 10, GROUND_Y - 3, 3},
    (EntityDrawMap){MAP_POINT, ENTITY_SNAKE, 8, GROUND_Y - 4, 0},
    (EntityDrawMap){MAP_POINT, ENTITY_EXIT, 13, GROUND_Y - 5, 0}};
static const EntityMap Map03 = {
    .lenght = 7,
    .tiles = (EntityDrawMap *)Map03_TilesData // Añade cast o declara .tiles como const
};

const EntityMap *Tiles[MAX_TILES] = {&Map01, &Map02, &Map03, &Map01};