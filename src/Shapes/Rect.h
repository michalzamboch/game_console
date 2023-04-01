#pragma once
#include "Shape.h"

#include <Arduino.h>
#include <Adafruit_ST7735.h>

extern Adafruit_ST7735 tft;

class Rect : public Shape
{
private:
    uint16_t width;
    uint16_t height;

    struct
    {
        bool round = false;
        uint16_t radius = 1;
    } Roudness;

    void innerPrint(uint16_t color)
    {
        if (this->Roudness.round)
        {
            if (this->getFill())
            {
                tft.fillRoundRect(this->x, this->y, this->width, this->height, this->Roudness.radius, color);
            }
            else
            {
                tft.drawRoundRect(this->x, this->y, this->width, this->height, this->Roudness.radius, color);
            }
        }
        else
        {
            if (this->getFill())
            {
                tft.fillRect(this->x, this->y, this->width, this->height, color);
            }
            else
            {
                tft.drawRect(this->x, this->y, this->width, this->height, color);
            }
        }
    }

public:
    Rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color = ST7735_WHITE) : Shape(x, y, color)
    {
        this->width = width;
        this->height = height;
    }

    ~Rect()
    {
    }

    /*------------------------------------*/

    uint16_t getWidth()
    {
        return this->width;
    }

    uint16_t getHeight()
    {
        return this->height;
    }

    uint16_t getX2()
    {
        return this->x + this->width;
    }

    uint16_t getY2()
    {
        return this->y + this->height;
    }

    /*------------------------------------*/

    uint16_t setWidth(uint16_t width)
    {
        this->width = width;
    }

    uint16_t setHeight(uint16_t height)
    {
        this->height = height;
    }

    /*------------------------------------*/

    bool higher(uint16_t val = 0)
    {
        if (this->y < val)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool lower(uint16_t val)
    {
        if (this->getY2() > val)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool tooLeft(uint16_t val = 0)
    {
        if (this->x < val)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool tooRight(uint16_t val)
    {
        if (this->getX2() > val)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /*------------------------------------*/
};