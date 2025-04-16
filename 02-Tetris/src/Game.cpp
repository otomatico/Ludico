#include "AGame.hpp"
/**Estos #include estam aqui porque es proprio de la clase */
#include <ctime>
#include "etc/env"
#include "lib/Canvas4Bit.cpp"
#include "Piece.cpp"
#include "Board.cpp"

#define stepAnimation(step) ++animateCount > step

using Matrix4x4 = bool[4][4];
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
        FPS = 3;
        hidecursor();
    }
    ~Game()
    {
        showcursor();
    }
    void Draw()
    {
        cleaner();
        if (stateGame == OPENING)
        {
            DrawString(4, 4, "Press any key");
        }
        if (stateGame == START)
        {
            drawBackground();
            drawBoard();
        }
    }

    bool Logic()
    {
        // drawable=true;
        if (stateGame == OPENING)
        {
            if (kbhit())
            {
                currentPiece = new Piece();
                nextPiece = new Piece();
                board = new Board(BOARD_WIDTH, BOARD_HEIGHT);
                stateGame = START;
                score = 0;
                level = 1;
                animateCount = 0;
            }
            return true;
        }
        if (stateGame == START)
        {
            Point piecePosition = currentPiece->GetPosition();
            Matrix4x4 &shape = currentPiece->GetShape();
            if (board->Collide(piecePosition, shape))
            {
                gotoXY(60,1);printf("x:%d y:%d",piecePosition.x,piecePosition.y);
                if (piecePosition.y == 0)
                {
                    board->solidifyPiece(piecePosition, shape);
                    score += board->clearRows() * 100;
                    // level = fibonachiLevel(score);
                    free(currentPiece);
                    currentPiece = nextPiece;
                    nextPiece = new Piece();
                }
                else
                {
                    stateGame = OVER;
                }
                return true;
            }
            if ((animateCount++) % 4 == 0)
            {

                int key = 0;
                if (kbhit())
                {
                    key = getchar();
                }
                currentPiece->Move(key);
                if (key > 0)
                {
                    piecePosition = currentPiece->GetPosition();
                    if (board->Collide(piecePosition, currentPiece->GetShape()))
                    {
                        currentPiece->RollBack(key);
                    }
                }
                animateCount = 0;
            }
        }
        return false;
    }

private:
    CanvasColor *canvas = NULL;
    CanvasColor *frame = NULL;
    Piece *currentPiece = NULL;
    Piece *nextPiece = NULL;
    Board *board = NULL;
    int score;
    int level;
    /*
    void Initialize()
    {
        stateGame = START;
        score = 0;
        animateCount = 0;
        FPS = 1;
    }
    */

    void drawBackground()
    {
        frame->Clear(CYAN);
        //imprimir la lines de puntos lateral
        frame->SetLineDot(0,1,0,SCREEN_HEIGHT - 1,LIGHTBLUE);
        frame->SetLineDot(1,0,1,SCREEN_HEIGHT - 1,LIGHTBLUE);
        frame->SetLineDot(SCREEN_WIDTH - 2,0,SCREEN_WIDTH - 2,SCREEN_HEIGHT - 1,BLUE);
        frame->SetLineDot(SCREEN_WIDTH - 1,1,SCREEN_WIDTH - 1,SCREEN_HEIGHT - 1,BLUE);
        //Imprimir la linea de punto abajo
        frame->SetLineDot(0,SCREEN_HEIGHT - 1, SCREEN_WIDTH / 2 ,SCREEN_HEIGHT - 1,LIGHTBLUE);
        frame->SetLineDot(SCREEN_WIDTH / 2,SCREEN_HEIGHT - 1,SCREEN_WIDTH - 1,SCREEN_HEIGHT - 1,BLUE);
        //psedomarco gris
        frame->SetLine(2, 0, 2, SCREEN_HEIGHT - 2, DARKGREY);
        frame->SetLine(2, SCREEN_HEIGHT - 2, SCREEN_WIDTH - 3, SCREEN_HEIGHT - 2, DARKGREY);
        frame->SetLine(SCREEN_WIDTH - 3, 0, SCREEN_WIDTH - 3, SCREEN_HEIGHT - 2, DARKGREY);
        
        // DrawShape
        if (nextPiece != NULL)
        {
            Matrix4x4 &shape = nextPiece->GetShape();
            drawShape(BOARD_MARGIN_X + BOARD_WIDTH * PIXEL_SIZE, 8, shape, frame);
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
        Point positionShape = currentPiece->GetPosition();
        Matrix4x4 &shape = currentPiece->GetShape();
        drawShape(positionShape.x*PIXEL_SIZE, positionShape.y*PIXEL_SIZE, shape, canvas, MAGENTA);
        canvas->Draw(BOARD_MARGIN_X, BOARD_MARGIN_Y);

        resetColor();
    }
    void drawShape(int x, int y, Matrix4x4 shape, CanvasColor *context, BYTE color = RED)
    {
        
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                if (shape[row][col])
                {
                    drawPixelSize(context, x + col * PIXEL_SIZE, y + row * PIXEL_SIZE, PIXEL_SIZE, color);
                }
            }
        }
    }
    void drawPixelSize(CanvasColor *context, int x, int y, int size, BYTE color)
    {
        for (int sizeRow = 0; sizeRow < size; sizeRow++)
            for (int sizeCol = 0; sizeCol < size; sizeCol++)
            {
                context->SetPixel(x + sizeCol, y + sizeRow, color);
            }
    }
};
