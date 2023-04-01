#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

class Display
{
private:
    int cs;
    int rst;
    int dc;
    Adafruit_ST7735 *display;

public:
    Display(int cs, int rst, int dc)
    {
        this->display = new Adafruit_ST7735(cs, rst, dc);
    }

    Display() : Display(5, 4, 2)
    {
        this->display = new Adafruit_ST7735(cs, rst, dc);
    }

    ~Display()
    {
        delete this->display;
    }

    Adafruit_ST7735 get()
    {
        return *this->display;
    }
};