#include "../domain/IElement.h"
#include "../env"
#include "../infrastructure/Utils.cpp"
#include <iostream>
#include <list>
using namespace std;
class Snake : public IElement
{

public:
    Snake()
    {
        tail = {};
    }

    ~Snake()
    {
        while (!tail.empty())
        {
            tail.pop_back();
        }
    }

    void resetPosition(int x, int y)
    {
        while (!tail.empty())
        {
            tail.pop_back();
        }
        tail.push_back((Point){(short)(x - 2), (short)y});
        tail.push_back((Point){(short)(x - 1), (short)y});
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
        bool crash;
        for (Point slice : tail)
        {
            crash = (value->x == slice.x && value->y == slice.y);
            if (crash)
            {
                return true;
            }
        }
        return false;
    }

    void Move(int key)
    {
        if (key == KEY_UP || key == KEY_DOWN || key == KEY_RIGHT || key == KEY_LEFT)
        {
            lastKey = key;
        }
        Move();
    }

    void Move()
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

    void Step()
    {
        tail.pop_front();
    }

protected:
    list<Point> tail;
    int lastKey;
};