
#ifndef _GAME_H_
#include "../../env"
#include "../../domain/Enum.h"
#include "../Utils.cpp"
#define _GAME_H_
class IGame
{
protected:
    int FPS;
    StateGame stateGame;

public:
    virtual void Draw() = 0;
    virtual void Logic() = 0;
    int Run()
    {
        while (stateGame != EXIT)
        {
            Logic();
            Draw();
            Sleep(1000 / FPS); // Controla a velocidade del Jogo
        }
        cleaner();
        return 0;
    }
};
#endif