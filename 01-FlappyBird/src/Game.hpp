
#ifndef _GAME_H_

#include <string.h>
#include "lib/Utils.hpp"
// #define LINUX       // For compile from inux
#include "Pipe.cpp"
#include "Bird.cpp"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24

#define stepAnimation(step) ++animateCount > step

#define _GAME_H_

class GameBase
{
    protected:
    int FPS;
    bool drawable;
    int animateCount;
    StateGame stateGame;
    
    public:
    virtual void Draw() = 0;
    virtual void Logic() = 0;
    int Run()
    {
        while (stateGame != EXIT)
        {
            if (drawable) // evitar parpadeo
            {
                Draw();
                drawable = false;
            }
            Logic();
            Sleep(1000 / FPS); // Controla a velocidade del Jogo
        }
        return 0;
    }
};

#include "Game.cpp"
#endif