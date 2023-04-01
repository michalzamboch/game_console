#pragma once

#include "Item.h"

#include "General/Constants.h"

#include <Arduino.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>
#include <string>
#include <vector>

using namespace std;

class Menu
{
private:
    vector<Item> items;
    static const uint16_t generalSize = TEXT_SIZE;

public:
    Menu();
    ~Menu();

    void add(string text);
    void add(uint16_t x, uint16_t y, string text);

    void print();
    void pointerUp();
    void pointerDown();
};
