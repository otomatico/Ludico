#ifndef _ELEMENT_H_
#define _ELEMENT_H_

typedef struct
{
    short x;
    short y;
} Point;

// Base class
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