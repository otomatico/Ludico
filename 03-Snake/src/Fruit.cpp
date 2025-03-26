#include "lib/Utils.hpp"

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
        return (Point) {
             rand() % (BOARD_WIDTH - 1),
             rand() % (BOARD_HEIGHT - 1)
        };
    }
};