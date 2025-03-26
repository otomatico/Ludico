#include <list>
#include "lib/Utils.hpp"

class Snake : public BasePlayer
{

public:
    Snake(int x = 1, int y = 1)
    {
        position.x = (short)x;
        position.y = (short)y;
    }
    list<Point> GetPositions()
    {
        return tail;
    }
    bool Collide(Point *value)
    {
        bool crash = false;
        for (int i = 0; i < tail.size() && !crash; i++)
        {
            crash = value->x == tail[i].x && value->y == tail[i].y;
        }
        return crash;
    }
    void Move(unsigned char key)
    {
        switch (key)
        {
        case KEY_UP:
        case KEY_DOWN:
        case KEY_RIGHT:
        case KEY_LEFT:
            lastKey = key;
            break;
        default:
            break;
        }
        Move_Snake();
    }

protected:
    list<Point> tail = {};
    unsigned char lastKey;

private:
    void Move_Snake()
    {
        switch (lastKey)
        {
        case KEY_UP:
            position.y--;
            break;
        case KEY_DOWN:
            position.y++;
            break;
        case KEY_RIGHT:
            position.x++;
            break;
        case KEY_LEFT:
            position.x--;
            break;
        default:
            break;
        }
        tail.push_back(position);
    }
};