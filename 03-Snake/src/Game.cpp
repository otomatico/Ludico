#ifndef _GAME_H_
#include "Game.hpp"
#endif

#define BOARD_WIDTH 60
#define BOARD_HEIGHT 10

#define BOARD_MARGIN_X 5
#define BOARD_MARGIN_Y 5

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
        gotoxy(1, 1);
        drawBackground();
        switch (this->stateGame)
        {
        case OPEN:
            drawOpenning();
            break;
        default:
            drawGameScore(this);
            drawTail(&this->snake.tail, &this->buffer);
            drawFruit(&this->fruit, &this->buffer);
            drawBoard(&this->buffer);
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
        int change = 0;
        switch (this.stateGame)
        {
        case OPEN:

            if (stick->event() && stick->key())
            {
                this.level = 1;
                this.score = 0;
                initPlayer(&(this->snake));
                initFruit(&(this->fruit), this->level);
                this->stateGame = GAME;
                change = 1;
            }
            break;
        case GAME:
            char key = '\0';
            change = 1;
            clearGFX(&this->buffer);
            if (stick->event())
            {
                key = stick->key();
            }
            if (key == KEY_PAUSE)
            {
                this->stateGame = PAUSE;
            }
            else
            {
                int hit = collision(&this->snake, &this->fruit);
                if (hit < 0)
                {
                    this->stateGame = OVER;
                }
                else
                {
                    moveGame(this, hit, key);
                }
            }
            break;
        case PAUSE:

            if (stick->event())
            {
                switch (stick->key())
                {
                case KEY_ESC:
                    this->stateGame = EXIT;
                    break;
                case KEY_ENTER:
                    this->stateGame = OPEN;
                    break;
                case KEY_PAUSE:
                    this->stateGame = GAME;
                    break;
                }
                change = 1;
            }
            break;
        case OVER:
            if (stick->event())
            {
                destroyTail(&(this->snake.tail));
                destroyStack(&(this->fruit));
                this->stateGame = OPEN;
                stick->key();
                change = 1;
            }
            break;
        }
        return change;
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
        score = 0;
        animateCount = 0;
    }
    void drawBackground()
    {
        colorGraphic(YELLOW, LIGHTGREY);
        for (int y = 1; y < SCREEN_HEIGHT; y++)
        {
            for (int x = 1; x < SCREEN_WIDTH; x++)
            {
                DrawString(x, y, "░");
            }
        }
    }
    void drawOpenning()
    {
        colorGraphic(GREEN, RED);
        DrawString(BOARD_MARGIN_X, BOARD_MARGIN_Y + 0, "╔══════════════════════════════╗");
        DrawString(BOARD_MARGIN_X, BOARD_MARGIN_Y + 1, "║   CSnake, Snake make in C    ║");
        DrawString(BOARD_MARGIN_X, BOARD_MARGIN_Y + 2, "║                              ║");
        DrawString(BOARD_MARGIN_X, BOARD_MARGIN_Y + 3, "║                              ║");
        DrawString(BOARD_MARGIN_X, BOARD_MARGIN_Y + 4, "║       Press any key          ║");
        DrawString(BOARD_MARGIN_X, BOARD_MARGIN_Y + 5, "╚══════════════════════════════╝");
    }

    void drawBoard()
    {
        int row;
        textcolor(WHITE);
        DrawWindow(BOARD_MARGIN_X - 1, BOARD_MARGIN_Y - 1, BOARD_WIDTH, BOARD_HEIGHT);
    }

    void drawTail()
    {
        Point2D board = {BOARD_MARGIN_X, BOARD_MARGIN_Y};
        Node *pointer = this->first;
        Point2D head;
        textcolor(GREEN);
        while (pointer != NULL)
        {
            head = look(pointer);
            putPixelGFX(buffer, head.x, head.y, GREEN);
            pointer = pointer->next;
        }
    }

    void drawFruit(Stack *this, gfx *buffer)
    {
        Point2D board = {BOARD_MARGIN_X, BOARD_MARGIN_Y};
        Node *pointer = this->first;
        Point2D head;
        textcolor(RED);
        while (pointer != NULL)
        {
            head = look(pointer);
            putPixelGFX(buffer, head.x, head.y, RED);
            pointer = pointer->next;
        }
    }

    void drawPause()
    {
        resetColor();
        DrawString(BOARD_MARGIN_X, BOARD_MARGIN_Y + 0, "╔══════════════════════════════╗");
        DrawString(BOARD_MARGIN_X, BOARD_MARGIN_Y + 1, "║          Game Pause          ║");
        DrawString(BOARD_MARGIN_X, BOARD_MARGIN_Y + 2, "║                              ║");
        DrawString(BOARD_MARGIN_X, BOARD_MARGIN_Y + 3, "║      Press [ESC]  Exit       ║");
        DrawString(BOARD_MARGIN_X, BOARD_MARGIN_Y + 4, "║    Press [Intro] Reboot      ║");
        DrawString(BOARD_MARGIN_X, BOARD_MARGIN_Y + 5, "║    Press [Space] continue    ║");
        DrawString(BOARD_MARGIN_X, BOARD_MARGIN_Y + 6, "╚══════════════════════════════╝");
    }

    void drawGameOver()
    {
        colorGraphic(RED, DARKGREY);
        DrawString(BOARD_MARGIN_X, BOARD_MARGIN_Y + 0, "╔══════════════════════════════╗");
        DrawString(BOARD_MARGIN_X, BOARD_MARGIN_Y + 1, "║          Game Over           ║");
        DrawString(BOARD_MARGIN_X, BOARD_MARGIN_Y + 2, "║                              ║");
        DrawString(BOARD_MARGIN_X, BOARD_MARGIN_Y + 3, "║      Press any continue      ║");
        DrawString(BOARD_MARGIN_X, BOARD_MARGIN_Y + 4, "╚══════════════════════════════╝");
    }

    void drawGameScore()
    {
        colorGraphic(DARKGREY, WHITE);
        // ▀ ▄ █ ▌ ▐ └┘│╜╖║ ┌
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