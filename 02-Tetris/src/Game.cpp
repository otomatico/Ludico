#include "AGame.hpp"
/**Estos #include estam aqui porque es proprio de la clase */
#include <ctime>
#include "etc/env"
#include "lib/Canvas4Bit.cpp"

#define stepAnimation(step) ++animateCount > step

class Game : public AGame
{

public:
    Game()
    {
        srand(time(NULL));
        stateGame = OPENING;
        animateCount = 0;
        score = 0;
        drawable = true;
        canvas = new CanvasColor(BOARD_WIDTH * PIXEL_SIZE, BOARD_HEIGHT * PIXEL_SIZE);
        frame = new CanvasColor(SCREEN_WIDTH, SCREEN_HEIGHT);
        hidecursor();
    }
    ~Game()
    {
        showcursor();
    }
    void Draw()
    {
        clear();
        drawBackground();
        drawBoard();
    }

    void
    Logic()
    {
        // drawable=true;
    }

private:
    CanvasColor *canvas;
    CanvasColor *frame;
    int score;

    void Initialize()
    {
        stateGame = START;
        score = 0;
        animateCount = 0;
        FPS = 1;
    }

    void drawBackground()
    {
        frame->Clear(CYAN);
        for (int i = 0; i < SCREEN_HEIGHT - 1; i++)
        {
            frame->SetPixel((i + 1) % 2, i, LIGHTBLUE);
            frame->SetPixel(SCREEN_WIDTH - ((i + 1) % 2) - 1, i, BLUE);

            frame->SetPixel(2, i, DARKGREY);
            frame->SetPixel(SCREEN_WIDTH - 3, i, DARKGREY);
        }
        for (int i = 0; i < SCREEN_WIDTH / 2; i++)
        {
            if (i % 2 == 0)
            {
                frame->SetPixel(i, SCREEN_HEIGHT - 1, LIGHTBLUE);
            }
            else
            {
                frame->SetPixel(SCREEN_WIDTH - i, SCREEN_HEIGHT - 1, BLUE);
            }
        }
        for (int i = 2; i < SCREEN_WIDTH - 2; i++)
        {
            frame->SetPixel(i, SCREEN_HEIGHT - 2, DARKGREY);
        }
        frame->Draw(1, 1);

        colorGraphic(WHITE, CYAN);
        DrawString(BOARD_MARGIN_X + BOARD_WIDTH * PIXEL_SIZE + 2, 2, "Score");
        DrawInt(BOARD_MARGIN_X + BOARD_WIDTH * PIXEL_SIZE + 8, 2, score);
        DrawString(BOARD_MARGIN_X + BOARD_WIDTH * PIXEL_SIZE + 2, 4, "Next Piece");
        resetColor();
    }
    void drawBoard()
    {
        canvas->Clear();
        canvas->Draw(BOARD_MARGIN_X, BOARD_MARGIN_Y);
        resetColor();
    }
};
