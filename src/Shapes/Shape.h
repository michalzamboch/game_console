#pragma once
#include "General/Displayable.h"
#include "General/Simulatable.h"

#include <Arduino.h>
//#include <Adafruit_ST7735.h>

class Shape : public Displayable
{
protected:
    bool fill = true;
    
    virtual void innerPrint(uint16_t color);

public:
    Shape(uint16_t x, uint16_t y, uint16_t color, bool fill) : Displayable(x, y, color)
    {
        this->fill = fill;
    }

    Shape(uint16_t x, uint16_t y, bool fill) : Displayable(x, y)
    {
        this->fill = fill;
    }

    Shape(uint16_t x, uint16_t y) : Displayable(x, y)
    {
    }

    ~Shape()
    {
    }

    bool getFill()
    {
        return this->fill;
    }

    void setFill(bool fill)
    {
        this->fill = fill;
    }
};
