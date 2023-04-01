#pragma once

#include "General/Displayable.h"

class Point : public Displayable
{
private:
    void innerPrint(uint16_t color)
    {
        
    }

public:
    Point(uint16_t x, uint16_t y, uint16_t color) : Displayable(x, y, color)
    {
    }

    ~Point()
    {
    }
};
