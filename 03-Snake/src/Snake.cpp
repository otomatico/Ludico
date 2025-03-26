#include "lib/Utils.hpp"
#include <iostream>
#include <list>
#include "etc/env"
using namespace std;
class Snake : public BasePlayer
{

public:
    Snake(int x = 1, int y = 1)
    {
        resetPosition(x, y);
    }

    void resetPosition(int x, int y)
    {
        while (tail.size() > 0)
        {
            tail.pop_back();
        }
        tail.push_back((Point){(short)(x - 1), (short)y});
        tail.push_back((Point){(short)(x - 2), (short)y});
        position.x = (short)x;
        position.y = (short)y;
        lastKey = KEY_RIGHT;
    }

    list<Point> GetPositions()
    {
        return tail;
    }
    bool Collide(Point *value)
    {
        bool crash = false;
        Point aux;
        for (int i = 0; i < tail.size() && !crash; i++)
        {
            aux = tail.front();
            crash = value->x == aux.x && value->y == aux.y;
            tail.pop_front();
            tail.push_back(aux);
        }
        return crash || value->x == position.x && value->y == position.y;
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
    void Step()
    {
        tail.pop_front();
    }

protected:
    list<Point> tail;
    unsigned char lastKey;

private:
    void Move_Snake()
    {
        tail.push_back(position);
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
    }
};