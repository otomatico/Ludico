#include "lib/Utils.hpp"

#define GAP 3

class Pipe : public BasePlayer
{

public:
    Pipe(int start = 0, int width = 60, int height = 10)
    {
        size.x = (short)width;
        size.y = (short)height;
        position.x = (short)start;
        position.y = generateGapPipe();
    }
    Point GetPosition()
    {
        return position;
    }
    bool Collide(Point *value)
    {
        return position.x == value->x && !(position.y <= value->y && value->y < (position.y + GAP));
    }
    int Move()
    {
        position.x--;
        if (position.x < 1)
        {
            position.x = size.x;
            position.y = generateGapPipe();
        }
        return position.x == 1;
    }

private:
    Point size;
    short generateGapPipe()
    {
        return 2 + (rand() % (size.y - GAP - 2));
    }
};