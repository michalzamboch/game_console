#pragma once
#include "General/Displayable.h"
#include "General/Constants.h"

#include <Arduino.h>
#include <string>
#include <Adafruit_ST7735.h>
#include <stdio.h>
#include <stdarg.h>

using namespace std;

extern Adafruit_ST7735 tft;

class Text : public Displayable
{
protected:
    uint16_t size = TEXT_SIZE;
    string text = "";

    void innerPrint(uint16_t color)
    {
        tft.setTextColor(color);
        tft.setTextSize(this->size);
        tft.setCursor(this->getX(), this->getY());
        tft.print(text.c_str());
    }

public:
    Text(uint16_t x, uint16_t y, uint16_t color, string text) : Displayable(x, y, color)
    {
        this->text = text;
    }

    Text(uint16_t x, uint16_t y, string text) : Displayable(x, y)
    {
        this->text = text;
    }

    Text(uint16_t x, uint16_t y) : Displayable(x, y)
    {
    }

    ~Text()
    {
    }

    /*void print()
    {
        tft.setTextSize(this->size);
        tft.setCursor(this->getX(), this->getY());
        tft.print(text.c_str());
    }

    void erase()
    {
    }*/

    int setText(const char *fmt, ...)
    {
        char buffer[512];
        va_list args;

        va_start(args, fmt);
        int rc = vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);
        
        string temp(buffer);
        this->text = temp;

        return rc;
    }
    
    void setText(string text)
    {
        this->text = text;
    }

    void setSize(uint16_t size)
    {
        this->size = size;
    }

    string getText()
    {
        return this->text;
    }
};
