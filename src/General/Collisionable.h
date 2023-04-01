#pragma once
#include "Shapes/Rect.h"

class Collisionable
{
public:
    static bool pointIn(int x, int y, Rect r)
    {
        return ((x >= r.getX() && y >= r.getY()) &&
                (x <= r.getX2() && y >= r.getY()) &&
                (x >= r.getX() && y <= r.getY2()) &&
                (x <= r.getX2() && y <= r.getY2()));
    }

    static bool rectInRect(Rect r1, Rect r2)
    {
        return (pointIn(r1.getX(), r1.getY(), r2) ||
                pointIn(r1.getX2(), r1.getY(), r2) ||
                pointIn(r1.getX(), r1.getY2(), r2) ||
                pointIn(r1.getX2(), r1.getY2(), r2));
    }

    static bool collidesRect(Rect r1, Rect r2)
    {
        return rectInRect(r1, r2) || rectInRect(r2, r1);
    }
};