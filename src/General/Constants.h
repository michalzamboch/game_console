#pragma once
#include <Adafruit_ST7735.h>

#define DEFAULT_COLOR ST7735_WHITE
#define DEFAULT_BACKGROUND_COLOR ST7735_BLACK

#define MENU_ITEM_SIZE 2
#define TEXT_SIZE 1

typedef enum
{
    UP = 0,
    DOWN = 1,
    RIGHT = 3,
    LEFT = 4
} Direction;

/*typedef enum
{
    UP = 1,
    DOWN = 2,
    LEFT = 4,
    RIGHT = 8,
    TOP_RIGHT = 16, // pravé horní tlačítko
    TOP_LEFT = 32   // levé horní tlačítko
} ButtonBit;

typedef enum
{
    UP = 13,
    DOWN = 12,
    LEFT = 25,
    RIGHT = 27,
    TOP_RIGHT = 33, // pravé horní tlačítko
    TOP_LEFT = 32   // levé horní tlačítko
} ButtonPin;
*/