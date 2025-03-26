#include "lib/Utils.hpp"
#include "etc/env"

class Fruit
{

public:
    Fruit()
    {
        position = generatePosition();
    }
    Point GetPosition()
    {
        return position;
    }
    bool Collide(Point *value)
    {
        return value->x == position.x && value->y == position.y;
    }
    void Move()
    {
        position = generatePosition();
    }

protected:
    Point position;

    private:
    Point generatePosition(){
        Point res;
        res.x = (short) rand() % (BOARD_WIDTH - 1);
        res.y = (short)rand() % (BOARD_HEIGHT - 1);
        return res;
    }
};