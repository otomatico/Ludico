#ifndef _COMPONENTS_C_
#define _COMPONENTS_C_

#include "EntitySnake.h"
#include "ComponentWorld.c"
#include "ComponentPlayer.c"
#include "ComponentMap.c"

// Componentes
typedef struct _Component
{
    ComponentWorld world;
    ComponentPlayer player;
    ComponentMap map;
} Component;

Component Component_Init(void)
{
    Component c;
    c.world = ComponentWorld_Init();
    c.player = ComponentPlayer_Init();
    c.map = ComponentMap_Init();
    return c;
}

#endif