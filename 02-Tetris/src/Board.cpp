#include "etc/env"
#include <stdlib.h>
#include "lib/Entities.cpp"

#define allocateArray(A, B) (A *)malloc(sizeof(A) * ((B > 0) ? (B) : 1))

class Board
{
private:
    int width;
    int height;
    bool *screen;

public:
    Board(int w, int h)
    {
        width = w;
        height = h;
        screen = allocateArray(bool, w *h);
    }

    bool inside(int x, int y)
    {
        return (0 <= x && x < width) && (0 <= y && y < height);
    }

    bool checkHit(Point position, bool *shape, int shapeSize = 4)
    {
        bool square;
        bool hit = false;
        for (int row = shapeSize - 1; row >= 0 && !hit; row--)
        {
            for (int col = 0; col < shapeSize && !hit; col++)
            {
                square = shape[row * shapeSize + col];
                if (square)
                {
                    int x = position.x + col;
                    int y = position.y + row;
                    hit = inside(x, y) ? screen[y * shapeSize + x] != 0 : 1;
                }
            }
        }
        return hit;
    }
    void Draw(int x, int y)
    {
    }
}