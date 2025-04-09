#include "etc/env"
#include <stdlib.h>
#include <string.h>
#include "lib/Entities.cpp"

#define allocateArray(A, B) (A *)malloc(sizeof(A) * ((B > 0) ? (B) : 1))
using Matrix4x4 = bool[4][4];  
class Board
{
private:
    int width;
    int height;
    bool *screen;

    void dropDown(int row)
    {
        int col = 0;
        while (row > 0)
        {
            while (col < BOARD_WIDTH)
            {
                screen[row * BOARD_WIDTH + col] = screen[(row - 1) * BOARD_WIDTH + col];
            }
            row--;
        }
        while (col < BOARD_WIDTH)
        {
            screen[0] = false;
        }
    }

    int theyAreAllOne(bool *column)
    {
        bool hasFull = true;
        for (int col = 0; col < BOARD_WIDTH && hasFull; col++)
        {
            hasFull = column[col];
        }
        return hasFull;
    }

public:
    Board(int w, int h)
    {
        width = w;
        height = h;
        screen = allocateArray(bool, w *h);
    }

    bool Inside(int x, int y)
    {
        return (0 <= x && x < width) && (0 <= y && y < height);
    }

    bool Collide(Point position, Matrix4x4 shape)
    {
        bool hit = false;
        for (int row = 3; row >= 0 && !hit; row--)
        {
            for (int col = 0; col < 4 && !hit; col++)
            {
                if (shape[row][col])
                {
                    int x = position.x + col;
                    int y = (position.y + row) *width;
                    hit = Inside(x, y) && screen[y + x];
                }
            }
        }
        return hit;
    }

    bool *GetShape()
    {
        return screen;
    }

    void solidifyPiece(Point position, Matrix4x4 shape)
    {
        bool square;
        int y, row, col;

        position.y--;
        for (row = 0; row < 4; row++)
        {
            y = position.y + row;
            for (col = 0; col < 4; col++)
            {
                square = shape[row][col];
                if (square)
                {
                    screen[y + position.x + col] = square;
                }
            }
        }
    }

    int clearRows()
    {
        int cleared = 0, row = BOARD_HEIGHT - 1;
        while (row >= 0)
        {
            if (theyAreAllOne(&screen[row * BOARD_WIDTH]))
            {
                dropDown(row);
                cleared++;
            }
            else
            {
                row--;
            }
        }
        return cleared;
    }
};