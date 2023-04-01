#pragma once
#include "Shape.h"

extern Adafruit_ST7735 tft;

class Circle : public Shape
{
private:
    uint16_t radius;

    void innerPrint(uint16_t color)
    {
        if (this->getFill())
        {
            tft.fillCircle(this->x, this->y, this->radius, color);
        }
        else
        {
            tft.drawCircle(this->x, this->y, this->radius, color);
        }
    }

public:
    Circle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color) : Shape(x, y, color)
    {
        this->radius = radius;
    }

    ~Circle()
    {
    }

    /*----------------------------------------------*/

    uint16_t getRadius()
    {
        return this->radius;
    }

    void setRadius(uint16_t radius)
    {
        this->radius = radius;
    }

    /*----------------------------------------------*/

    
};
