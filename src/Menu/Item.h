#pragma once

#include "Text.h"

#include <Arduino.h>
#include <string>

class Item : public Text
{
private:
    int index;

public:
    Item(uint16_t x, uint16_t y, uint16_t color, string text) : Text(x, y, color, text)
    {
        this->index = 0;
        this->setSize(MENU_ITEM_SIZE);
    }

    ~Item()
    {
    }
};
