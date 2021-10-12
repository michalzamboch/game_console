#include <Arduino.h>
#include <ctime>
#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>

#define TFT_CS     5
#define TFT_RST    4
#define TFT_DC     2

/* zapisovane hodnoty ovladacich tlacitek
 * zapisou se do promenne unsigned int ovladani
 */
#define T_DO_PREDU 1
#define T_DO_ZADU 2
#define T_DO_LEVA 4
#define T_DO_PRAVA 8
#define T_1 16      // pravé horní tlačítko
#define T_2 32      // levé horní tlačítko

/* piny na lolinu
 * muzou se taky pouzit jako odkazy na zjistovani prectenych hodnot z tlacitek
 * pustím do toho šťávu přes P_HOT (pin 36) - bude to OUTPUT
 */
#define P_DO_PREDU 13
#define P_DO_ZADU 12
#define P_DO_LEVA 25
#define P_DO_PRAVA 27
#define P_1 33      // pravé horní tlačítko
#define P_2 32      // levé horní tlačítko

/*
 * DISPLAY: 128 * 160 PIXELŮ
 * HRY NA KONZOLI: SNAKE, TOWER OF HANOI A NĚCO DALŠÍHO TŘEBA OSEKANÍ SPACE INVADERS
 */

/*
 * Příkad k maturitní otázce číslo 6
*/
typedef struct
{
    unsigned long interval;
    unsigned long soucasnaHodnota;
}interval;
