#include "lib/Utils.hpp"

class FlappyBird
{

public:
    FlappyBird(int x = 1, int y = 1)
    {
        position.x = (short)x;
        position.y = (short)y;
    }
    Point GetPosition()
    {
        return position;
    }
    bool Collide(Point *value)
    {
        return value->x == position.x && value->y == position.y;
    }
    void Move(char key)
    {
        if (key == KEY_SPACE && position.y > 1)
        {
            position.y -= 2;
        }
        position.y++;
    }

protected:
    Point position;
};