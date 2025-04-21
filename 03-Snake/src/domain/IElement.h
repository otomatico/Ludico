#include "Enum.h"
#ifndef _ELEMENT_H_
#define _ELEMENT_H_

class IElement
{
public:
    Point GetPosition()
    {
        return position;
    }
    bool Collide(Point *value)
    {
        return value->x == position.x && value->y == position.y;
    }
    virtual void Move() = 0;

protected:
    Point position;
};
#endif