#include "lib/Entities.cpp"
#include "lib/keyboard.h"
#include <string>

using Matrix4x4 = bool[4][4];

const Matrix4x4 SHAPES[7] =
    {
        {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // SHAPE_I
        {{0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}}, // SHAPE_J
        {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}, // SHAPE_L
        {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // SHAPE_O
        {{0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // SHAPE_S
        {{0, 1, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // SHAPE_T
        {{0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}  // SHAPE_Z
};

class Piece
{
private:
    Point position;
    Point old;
    Matrix4x4 shape;

    void copyShape(const Matrix4x4 newShape)
    {
        memcpy(shape, newShape, sizeof(shape));
    }

public:
    Piece()
    {
        position.x = 4;
        position.y = 0;

        int index = rand() % 7;
        copyShape(SHAPES[index]);
    }
    ~Piece()
    {
    }
    void Rotate(int angle)
    {
        Matrix4x4 rotated;
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                if (angle == 1)
                {
                    rotated[col][row] = shape[row][3 - col];
                }
                else if (angle == -1)
                {
                    rotated[row][col] = shape[3 - col][row];
                }
            }
        }
        copyShape(rotated);
    }

    void Move(int key)
    {
        position.y++;
        old = position;
        if (key == KEY_LEFT)
        {
            position.x--;
            return;
        }
        if (key == KEY_RIGHT)
        {
            position.x++;
            return;
        }
        if (key == KEY_DOWN)
        {
            position.y++;
            return;
        }
        if (key == KEY_UP)
        {
            Rotate(1);
        }
    }

    void RollBack(int key)
    {
        position = old;
        if (key == KEY_UP)
        {
            Rotate(-1);
        }
        // position.y++;
    }

    Point GetPosition()
    {
        return position;
    }

    Matrix4x4 &GetShape()
    {
        return shape;
    }
};
