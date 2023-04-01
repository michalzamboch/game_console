#pragma once

#include "General/Displayable.h"

class Line : public Displayable
{
private:
    uint16_t x2;
    uint16_t y2;

    void innerPrint(uint16_t color)
    {
    }

public:
    Line(uint16_t x, uint16_t y, uint16_t color) : Displayable(x, y, color)
    {
    }

    ~Line()
    {
    }

    uint16_t getX2()
    {
        return this->x2;
    }

    uint16_t getY2()
    {
        return this->y2;
    }
};
