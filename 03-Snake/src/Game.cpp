#ifndef _GAME_H_
#include "Game.hpp"
#endif

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
        // en lugar de clear(), esto evita parpadeos
        //gotoxy(1, 1);
         cls();
        drawBackground();
        switch (this->stateGame)
        {
        case MENU:
            drawOpenning();
            break;
        case OPENING:

            break;
        default:
            drawGameScore();
            drawTail();
            drawFruit();
            drawBoard();
            if (this->stateGame == OVER)
            {
                drawGameOver();
            }
            if (this->stateGame == PAUSE)
            {
                drawPause();
            }
        }
        resetColor();
        animateCount++;
    }

    void Logic()
    {
        char key = press();
        switch (stateGame)
        {
        case OPENING:
            // level = 1;
            // score = 0;
            // drawable = true;
            Initialize();
            stateGame = MENU;
            break;
        case MENU:
            if(press()){
                stateGame = START;
                level = 1;
                score = 0;
                drawable = true;
            }
            break;
        case START:
            drawable = true;
            if (key == KEY_ESC)
            {
                stateGame = PAUSE;
            }
            else
            {
                player->Move(key);
                Point position = player->GetPosition();
                if (player->Collide(&position))
                {
                    stateGame = OVER;
                }
                if (enemy->Collide(&position))
                {
                    enemy->Move();
                }
                else
                {
                    player->Step();
                }
            }
            break;
        case PAUSE:

            if (key == KEY_ESC)
            {
                stateGame = START;
                drawable = true;
            }
            break;
        case OVER:
            if (key != KEY_EMPTY)
            {
                stateGame = OPENING;
                drawable = true;
            }
            break;
        }
    }

private:
    Fruit *enemy;
    Snake *player;
    int score;
    int level;

    void Initialize()
    {
        player = new Snake(BOARD_WIDTH / 2, BOARD_HEIGHT / 2);
        enemy = new Fruit();
        stateGame = OPENING;
        level = 1;
        score = 0;
        animateCount = 0;
        drawable = true;
        FPS = 2;
    }
    void drawBackground()
    {
        colorGraphic(YELLOW, LIGHTGREY);
        for (int y = 1; y < SCREEN_HEIGHT; y++)
        {
            DrawBarHorizontal(1, y, SCREEN_WIDTH, CHAR_SOLID_50);
        }
    }
    void drawOpenning()
    {
        colorGraphic(GREEN, RED);
        DrawWindow(BOARD_MARGIN_X, BOARD_MARGIN_Y, 32, 6);
        DrawString(BOARD_MARGIN_X + 1, BOARD_MARGIN_Y + 1, "         Snake en C++          ");
        DrawString(BOARD_MARGIN_X + 1, BOARD_MARGIN_Y + 2, "         by Otomatico          ");
        DrawString(BOARD_MARGIN_X + 1, BOARD_MARGIN_Y + 3, "                               ");
        DrawString(BOARD_MARGIN_X + 1, BOARD_MARGIN_Y + 4, "        Press any key          ");
        resetColor();
    }

    void drawBoard()
    {
        colorGraphic(WHITE, LIGHTGREY);
        DrawWindow(BOARD_MARGIN_X - 1, BOARD_MARGIN_Y - 1, BOARD_WIDTH, BOARD_HEIGHT);
        for (int row = 0; row < BOARD_HEIGHT; row++)
        {
            DrawBarHorizontal(BOARD_MARGIN_X, BOARD_MARGIN_Y + row, BOARD_WIDTH, CHAR_SOLID_50);
        }
    }

    void drawTail()
    {
        textcolor(GREEN);
        for (Point tail : player->GetPositions())
        {
            DrawChar(tail.x, tail.y, CHAR_SOLID);
        }
        textcolor(LIGHTGREEN);
        Point head = player->GetPosition();
        DrawChar(head.x, head.y, CHAR_SOLID);
    }

    void drawFruit()
    {
        Point head = enemy->GetPosition();
        textcolor(LIGHTRED);
        DrawChar(head.x, head.y, CHAR_SOLID);
    }

    void drawPause()
    {
        resetColor();
        DrawWindow(BOARD_MARGIN_X, BOARD_MARGIN_Y, 32, 6);
        DrawString(BOARD_MARGIN_X + 1, BOARD_MARGIN_Y + 1, "           Game Pause          ");
        DrawString(BOARD_MARGIN_X + 1, BOARD_MARGIN_Y + 2, "                               ");
        DrawString(BOARD_MARGIN_X + 1, BOARD_MARGIN_Y + 3, "       Press [ESC]  Exit       ");
        DrawString(BOARD_MARGIN_X + 1, BOARD_MARGIN_Y + 4, "     Press [Intro] Reboot      ");
        DrawString(BOARD_MARGIN_X + 1, BOARD_MARGIN_Y + 5, "     Press [Space] continue    ");
    }

    void drawGameOver()
    {
        colorGraphic(RED, DARKGREY);
        DrawWindow(BOARD_MARGIN_X, BOARD_MARGIN_Y, 32, 4);
        DrawString(BOARD_MARGIN_X+1, BOARD_MARGIN_Y + 1, "          Game Over           ");
        DrawString(BOARD_MARGIN_X+1, BOARD_MARGIN_Y + 2, "                              ");
        DrawString(BOARD_MARGIN_X+1, BOARD_MARGIN_Y + 3, "      Press any continue      ");

    }

    void drawGameScore()
    {
        colorGraphic(DARKGREY, WHITE);
        DrawString(1, 1, " █▀▀ █▌ █  █▀█  █ ▄▀ █▀▀   ┌─────────────────────────────╖  ");
        DrawString(1, 2, " ▀▀█ █▀▄█ ▐█▄█▌ ██▄  █▀    │ SCORE: 0000      LEVEL: 000 ║  "); //, this->score, this->level);
        DrawString(1, 3, " ▀▀▀ ▀  ▀ ▀▀ ▀▀ ▀  ▀ ▀▀▀   └─────────────────────────────╜  ");
    }

    /*
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
        */
};