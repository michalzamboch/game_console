#pragma once

#include <Arduino.h>
#include "Constants.h"
#include "Console/Display.h"

class Displayable
{
protected:
    uint16_t x;
    uint16_t y;
    uint16_t color = DEFAULT_COLOR;
    uint16_t eraseColor = DEFAULT_BACKGROUND_COLOR;

    Display *display = nullptr;

    bool refresh = true;
    bool enablePrint = true;
    bool alreadyPrinted = false;

    virtual void innerPrint(uint16_t color) = 0;

public:
    Displayable(uint16_t x, uint16_t y, uint16_t color = DEFAULT_COLOR)
    {
        this->x = x;
        this->y = y;
        this->color = color;
    }

    void print()
    {
        if (this->enablePrint == true)
        {
            innerPrint(this->color);
            this->alreadyPrinted = true;
        }
    }

    void erase()
    {
        innerPrint(this->eraseColor);
    }

    void moveX(short val)
    {
        this->x += val;
    }

    void moveY(short val)
    {
        this->y += val;
    }

    virtual void move(short x, short y)
    {
        this->x += x;        
        this->y += y;
    }

    virtual void set(uint16_t x, uint16_t y)
    {
        this->x = x;
        this->y = y;
    }

    void setColor(uint16_t color)
    {
        this->color = color;
    }

    uint16_t getColor()
    {
        return this->color;
    }

    uint16_t getX()
    {
        return this->x;
    }

    uint16_t getY()
    {
        return this->y;
    }

    uint16_t getEraseColor()
    {
        return this->eraseColor;
    }

    void setEraseColor(uint16_t eraseColor)
    {
        this->eraseColor = eraseColor;
    }

    bool getRefresh()
    {
        return this->refresh;
    }

    void setRefresh(bool refresh)
    {
        this->refresh = refresh;
    }

    bool getAlreadyPrinted()
    {
        return this->alreadyPrinted;
    }
};