#ifndef _GAME_H_
#include "Game.hpp"
#endif

#define BOARD_WIDTH 60
#define BOARD_HEIGHT 10

#define MAX_PIPE 4
#define WIDE 15

class Game : public GameBase
{

public:
    Game()
    {
        srand(time(NULL));
        stateGame = OPENING;
        animateCount = 0;
        drawable = true;
        hidecursor();
    }
    void Draw()
    {
        clear();
        if (stateGame == MENU)
        {
            DrawMenu();
        }
        if (stateGame == START || stateGame == PAUSE || stateGame == OVER)
        {
            DrawBase();
            DrawPipe();
            DrawBird();
            if (stateGame == OVER)
            {
                DrawGameOver();
            }
            if (stateGame == PAUSE)
            {
                DrawPause();
            }
        }
        resetColor();
        animateCount++;
    }

    void Logic()
    {
        switch (stateGame)
        {
        case OPENING:
            FPS = 5;
            if (stepAnimation(10))
            {
                stateGame = MENU;
                drawable = true;
                animateCount = 0;
            }
            break;
        case MENU:
            drawable = true;
            LogicMenu();
            break;
        case START:
            drawable = true;
            LogicGameLoop();
            break;
        case PAUSE:
            LogicPause();
            break;
        default:
            if (stepAnimation(15))
            {
                drawable = true;
                stateGame = MENU;
                animateCount = 0;
            }
            break;
        }
    }

private:
    Pipe *enemy[MAX_PIPE];
    FlappyBird *player;
    int score;

    void Initialize()
    {
        player = new FlappyBird(1, BOARD_HEIGHT / 2);

        for (short index = 0; index < MAX_PIPE; index++)
        {
            short start = (WIDE * (index + 1));
            enemy[index] = new Pipe(start, BOARD_WIDTH, BOARD_HEIGHT);
        }
        stateGame = OPENING;
        score = 0;
        animateCount = 0;
    }

    void DrawMenu()
    {
        short Y = (BOARD_HEIGHT - 2) / 2;
        DrawWindow(10, Y - 1, SCREEN_WIDTH - 20, 4);
        DrawString((SCREEN_WIDTH - 28) / 2, Y, "Flappy Bird en C++ con Clase");
        DrawString((SCREEN_WIDTH - 13) / 2, Y + 1, "by Otomatico");
        if (animateCount % 4 == 0)
        {
            DrawString((SCREEN_WIDTH - 14) / 2, Y + 5, "Press any key");
        }
    }
    void DrawPause()
    {
        textcolor(WHITE);
        short X = (BOARD_WIDTH - 5) / 2;
        short Y = (BOARD_HEIGHT - 1) / 2;
        DrawWindow(X - 5, Y - 1, 15, 3);
        DrawString(X, Y, "PAUSE");
    }
    void DrawGameOver()
    {
        short Y = BOARD_HEIGHT / 2;
        textcolor(LIGHTRED);
        DrawString(BOARD_WIDTH + 6, Y, "Game Over!");
        textcolor(YELLOW);
        DrawString(BOARD_WIDTH + 2, Y + 1, "Final");
        DrawScore();
        bell();
    }
    void DrawBird()
    {
        Point bird = player->GetPosition();
        textcolor(YELLOW);
        DrawChar(bird.x, bird.y, 0xfe);
        resetColor();
    }
    void DrawScore()
    {
        gotoxy(BOARD_WIDTH + 8, (BOARD_HEIGHT / 2) + 1);
        printf("Score: %03d", score);
        resetColor();
    }
    void DrawBase()
    {

        colorGraphic(LIGHTGREY, RED);
        DrawBarHorizontal(1, BOARD_HEIGHT, BOARD_WIDTH, 0xb2);
        resetColor();

        textcolor(DARKGREY);
        DrawBarHorizontal(BOARD_WIDTH + 1, BOARD_HEIGHT, SCREEN_WIDTH - BOARD_WIDTH - 2, 0xdc);
        DrawBarVertical(BOARD_WIDTH, 1, BOARD_HEIGHT, 0xdb);
        DrawBarHorizontal(BOARD_WIDTH + 1, 1, SCREEN_WIDTH - BOARD_WIDTH - 2, 0xdf);
        DrawBarVertical(SCREEN_WIDTH - 1, 1, BOARD_HEIGHT, 0xdb);

        textcolor(LIGHTGREY);
        DrawScore();
    }
    void DrawPipe()
    {
        for (int index = 0; index < MAX_PIPE; index++)
        {
            Point pipe = enemy[index]->GetPosition();
            short gap = pipe.y + GAP;
            textcolor(GREEN);
            DrawBarVertical(pipe.x, 1, pipe.y, 0xdb);
            DrawBarVertical(pipe.x, gap, BOARD_HEIGHT - gap, 0xdb);
        }
    }

    void LogicPause()
    {
        char key = press();
        if (key == KEY_ESC)
        {
            stateGame = START;
            drawable = true;
        }
    }
    void LogicGameLoop()
    {
        char key = press();
        if (key == KEY_ESC)
        {
            stateGame = PAUSE;
        }
        else
        {
            player->Move(key);
            Point bird = player->GetPosition();
            stateGame = bird.y >= BOARD_HEIGHT ? OVER : START;
            for (short index = 0; index < MAX_PIPE && stateGame != OVER; index++)
            {
                if (enemy[index]->Collide(&bird))
                {
                    stateGame = OVER;
                    animateCount = 0;
                }
                else
                {
                    bool passed = enemy[index]->Move();
                    score += passed ? 1 : 0;
                }
            }
        }
        drawable = true;
    }
    void LogicMenu()
    {
        FPS = 2;
        animateCount = stepAnimation(8) ? 0 : animateCount;
        if (press())
        {
            Initialize();
            stateGame = START;
            FPS = 3;
        }
    }
};