#include "IGame.h"
/**Estos #include estam aqui porque es proprio de la clase */
#include "../display/Canvas1bit.cpp"
#include "../input/keyboard.h"
#include "../../application/Fruit.cpp"
#include "../../application/Snake.cpp"

#define stepAnimation(step) ++animateCount > step

class Game : public IGame
{

public:
    Game()
    {
        srand(time(NULL));
        stateGame = OPENING;
        animateCount = 0;
        drawable = true;
        canvas = new Canvas1bit(BOARD_WIDTH, BOARD_HEIGHT);
        player = new Snake();
        enemy = new Fruit();
        hidecursor();
    }
    ~Game()
    {
        showcursor();
    }
    void Draw()
    {

        gotoXY(1, 1);
        if (stateGame == OPENING)
        {
            if (drawable)
            {
                drawBackground();
                drawable = false;
            }
            return;
        }

        if (stateGame == MENU)
        {
            if (drawable)
            {
                drawBackground();
                drawOpenning();
                drawable = false;
            }
            return;
        }
        if (stateGame == START || stateGame == OVER || stateGame == PAUSE)
        {
            if (drawable)
            {
                drawBackground();
                drawScore();
            }

            if (stateGame == START)
            {
                drawBoard();
            }
            if (stateGame == OVER && drawable)
            {
                drawBoard();
                drawGameOver();
            }
            if (stateGame == PAUSE && drawable)
            {
                drawBoard();
                drawPause();
            }
            drawable = false;
        }
        resetColor();
    }

    void
    Logic()
    {
        int key = press();
        if (stateGame == MENU)
        {
            drawable = LogicMenu(key);
            return;
        }
        if (stateGame == START)
        {
            drawable = LogicStart(key);
            return;
        }
        if (stateGame == PAUSE)
        {
            drawable = LogicPause(key);
            return;
        }
        if (stateGame == OVER && key == KEY_ESC)
        {
            stateGame = MENU;
            drawable = true;
            return;
        }
        if (stateGame == OPENING && stepAnimation(12))
        {
            stateGame = MENU;
            drawable = true;
            animateCount = 0;
        }
    }

private:
    bool drawable;
    int animateCount;
    ICanvas *canvas;
    Fruit *enemy;
    Snake *player;
    int score;

    void Initialize()
    {
        player->resetPosition(BOARD_WIDTH / 2, BOARD_HEIGHT / 2);
        enemy->Move();
        stateGame = START;
        score = 0;
        animateCount = 0;
        FPS = 3;
    }

    bool inBoard(Point value)
    {
        bool crash = (0 <= value.x && value.x < BOARD_WIDTH) && (0 <= value.y && value.y < BOARD_HEIGHT);
        return crash;
    }

    void drawBackground()
    {
        colorGraphic(YELLOW, LIGHTGREY);
        for (int y = 1; y < SCREEN_HEIGHT; y++)
        {
            DrawBarHorizontal(1, y, SCREEN_WIDTH, CHAR_SOLID_50);
        }
        colorGraphic(BLUE, CYAN);
        DrawBarHorizontal(1, 1, SCREEN_WIDTH, CHAR_SOLID_25);
        colorGraphic(WHITE, RED);
        DrawChar(SCREEN_WIDTH, 1, 0x9e);
        resetColor();
    }

    void drawOpenning()
    {
        const int margin_x = SCREEN_WIDTH / 2 - 16;
        colorGraphic(BLACK, LIGHTGREEN);
        DrawString(margin_x, BOARD_MARGIN_Y + 1, "          Snake en C++          ");
        DrawString(margin_x, BOARD_MARGIN_Y + 2, "          by Otomatico          ");
        DrawString(margin_x, BOARD_MARGIN_Y + 4, "       Press [ESC]  Exit        ");
        DrawString(margin_x, BOARD_MARGIN_Y + 5, "    Press [SPACE] Start Game    ");
        DrawBarHorizontal(margin_x, BOARD_MARGIN_Y, 32, CHAR_EMPTY);
        DrawBarHorizontal(margin_x, BOARD_MARGIN_Y + 3, 32, CHAR_EMPTY);
        DrawBarHorizontal(margin_x, BOARD_MARGIN_Y + 6, 32, CHAR_EMPTY);
        resetColor();
    }

    void drawPause()
    {
        resetColor();
        DrawWindow(BOARD_MARGIN_X, BOARD_MARGIN_Y, 32, 6);
        DrawString(BOARD_MARGIN_X + 1, BOARD_MARGIN_Y + 1, "           Game Pause          ");
        DrawString(BOARD_MARGIN_X + 1, BOARD_MARGIN_Y + 3, "       Press [ESC]  Exit       ");
        DrawString(BOARD_MARGIN_X + 1, BOARD_MARGIN_Y + 4, "     Press [Space] continue    ");
        DrawBarHorizontal(BOARD_MARGIN_X + 1, BOARD_MARGIN_Y + 2, 31, CHAR_EMPTY);
    }

    void drawGameOver()
    {
        colorGraphic(MAGENTA, BLACK);
        DrawWindow(BOARD_MARGIN_X, BOARD_MARGIN_Y, 32, 5);
        DrawString(BOARD_MARGIN_X + 1, BOARD_MARGIN_Y + 1, "          Game Over            ");
        DrawString(BOARD_MARGIN_X + 1, BOARD_MARGIN_Y + 3, "      Press [ESC] continue     ");
        DrawBarHorizontal(BOARD_MARGIN_X + 1, BOARD_MARGIN_Y + 2, 31, CHAR_EMPTY);
        resetColor();
    }

    void drawScore()
    {
        colorGraphic(DARKGREY, LIGHTGREY);
        DrawBarHorizontal(1, SCREEN_HEIGHT - 1, SCREEN_WIDTH, CHAR_EMPTY);
        gotoXY(70, SCREEN_HEIGHT - 1);
        printf("SCORE %03d", score);
        resetColor();
    }

    void drawBoard()
    {
        canvas->Clear();
        // drawTail
        for (Point tail : player->GetPositions())
        {
            canvas->SetPixel(tail.x, tail.y, WHITE);
        }
        Point value = player->GetPosition();
        canvas->SetPixel(value.x, value.y, WHITE);

        // drawFruit
        value = enemy->GetPosition();
        canvas->SetPixel(value.x, value.y, WHITE);

        colorGraphic(WHITE, DARKGREY);
        canvas->Draw((SCREEN_WIDTH - BOARD_WIDTH) / 2, BOARD_MARGIN_Y);
    }

    bool LogicMenu(int key)
    {
        if (key == KEY_SPACE)
        {
            Initialize();
            return true;
        }
        if (key == KEY_ESC)
        {
            stateGame = EXIT;
            return true;
        }
        return false;
    }

    bool LogicStart(int key)
    {
        if (key == KEY_ESC)
        {
            stateGame = PAUSE;
            return true;
        }

        player->Move(key);
        Point position = player->GetPosition();
        if (!inBoard(position) || player->Collide(&position))
        {
            stateGame = OVER;
            return true;
        }
        if (enemy->Collide(&position))
        {
            enemy->Move();
            score++;
            return true;
        }
        player->Step();
        return false;
    }

    bool LogicPause(int key)
    {
        if (key == KEY_ESC)
        {
            stateGame = MENU;
            return true;
        }
        if (key == KEY_SPACE)
        {
            stateGame = START;
            return true;
        }
        return false;
    }
};

/*
        BYTE S[]={
            0b00111110,
            0b01100000,
            0b01000000,
            0b01100000,
            0b00111100,
            0b00000110,
            0b00000010,
            0b00000110,
            0b01111100
        };
        BYTE N[]={
            0b00000000,
            0b01000010,
            0b01000010,
            0b01100010,
            0b01110010,
            0b01011010,
            0b01001110,
            0b01000010
        };
        BYTE A[]={
            0b00000000,
            0b00111100,
            0b01000010,
            0b01000010,
            0b01111110,
            0b01000010,
            0b01000010,
            0b01000010
        };
        BYTE K[]={
            0b00000000,
            0b01000010,
            0b01000100,
            0b01001000,
            0b01110000,
            0b01111100,
            0b01000010,
            0b01000010
        };
        BYTE E[]={
            0b00000000,
            0b01111110,
            0b01000000,
            0b01000000,
            0b01111100,
            0b01000000,
            0b01000000,
            0b01111110
        };


    void drawTail()
    {
        for (Point tail : player->GetPositions())
        {
            canvas->SetPixel(tail.x, tail.y);
        }
        Point value = player->GetPosition();
        canvas->SetPixel(value.x, value.y);
    }

    void drawFruit()
    {
        Point value = enemy->GetPosition();
        canvas->SetPixel(value.x, value.y);
    }
*/