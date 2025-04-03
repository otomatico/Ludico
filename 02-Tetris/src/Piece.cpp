#include "lib/Entities.cpp"
#include "lib/keyboard.h"
#include <string>

class Piece
{
private:
    Point position;
    Point old;
    int shape[4][4];

    void copyShape(const int newShape[4][4])
    {
        memcpy(shape, newShape, sizeof(shape));
    }

public:
    void Rotate(int angle)
    {
        int rotated[4][4];
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
        old = position;
        switch (key)
        {
        case KEY_LEFT:
            position.x--;
            break;
        case KEY_RIGHT:
            position.x++;
            break;
        case KEY_DOWN:
            position.y++;
            break;
        case KEY_UP:
            Rotate(1);
            break;
        }
        position.y++;
    }

    void RollBack(int key)
    {
        position = old;
        if (key == KEY_UP)
        {
            Rotate(-1);
        }
        position.y++;
    }
    
    Point GetPosition()
    {
        return position;
    }
};