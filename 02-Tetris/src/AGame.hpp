
#ifndef _GAME_H_
#define _GAME_H_
#include "lib/Entities.cpp"
#include "lib/keyboard.h"
class AGame
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
            Logic();
            if (drawable) // evitar parpadeo
            {
                Draw();
                drawable = false;
            }
            Sleep(1000 / FPS); // Controla a velocidade del Jogo
        }
        return 0;
    }
};
#endif