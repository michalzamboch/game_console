#pragma once
#include "Shapes/Rect.h"
#include <vector>

using namespace std;

class Bat : public Rect, public Simulatable
{
private:

public:
    Bat(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color, short speedX, short speedY)
        : Rect(x, y, width, height, color)
    {
        this->setSpeed(speedX, speedY);

    }

    Bat(uint16_t x, uint16_t y, short speedX, short speedY)
        : Bat(x, y, 0, 0, ST7735_WHITE, speedX, speedY)
    {
        this->setWidth(10);
        this->setHeight(50);
    }

    ~Bat()
    {
    }

    void simulate()
    {
        // this->move(this->getSpeedX(), this->getSpeedY());
    }
};