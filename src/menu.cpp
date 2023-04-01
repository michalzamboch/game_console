#include "include.h"

extern unsigned int ovladani;
extern Adafruit_ST7735 tft;
extern void inicializujDisplay();
extern void vypisBity(int x, int y);

/*
 * Příklad pro 2. maturitní otázku, strukturované programování
 * tento soubor je napsaný nejvíce pomocí techniky jazyka C - klasické strukturované programování 
*/

short menuMain();

static void vypisMenu();
static void ukazatel(short pozice, uint16_t color);
static short ovladaniMenu();

/**********************************************************************************************/

short menuMain()
{
    vypisMenu();

    short vracenaHodnota = ovladaniMenu();

    return vracenaHodnota;
}

static short ovladaniMenu()
{
    short pozice = 1;
    ukazatel(pozice, ST7735_WHITE);

    unsigned long soucasnyCas = millis();
    interval intPrepnuti = { 150, soucasnyCas };
    ovladani = 0;

    while (true)
    {
        soucasnyCas = millis();

        if ((ovladani == T_DO_PREDU) && (pozice > 1) && ((soucasnyCas - intPrepnuti.soucasnaHodnota) > intPrepnuti.delka))
        {
            intPrepnuti.soucasnaHodnota = soucasnyCas;
            ukazatel(pozice, ST7735_BLACK);
            pozice--;
            ukazatel(pozice, ST7735_WHITE);
        }
        else if ((ovladani == T_DO_ZADU) && (pozice < 3) && ((soucasnyCas - intPrepnuti.soucasnaHodnota) > intPrepnuti.delka))
        {
            intPrepnuti.soucasnaHodnota = soucasnyCas;
            ukazatel(pozice, ST7735_BLACK);
            pozice++;
            ukazatel(pozice, ST7735_WHITE);
        }
        else if ((ovladani & T_2) == T_2)
        {
            return pozice;
        }
    }
}

static void vypisMenu()
{
    tft.fillScreen(ST7735_BLACK);
    tft.setTextSize(1);
    tft.setTextColor(ST7735_WHITE);

    tft.setCursor(8, 5);
    tft.print("Play Station Hostalkova");

    tft.drawLine(3, 18, 156, 18, ST7735_WHITE);

    tft.setTextSize(2);
    tft.setCursor(20, 25);
    tft.print("Snake");
    tft.setCursor(20, 45);
    tft.print("Tower");
    tft.setCursor(20, 65);
    tft.print("Invaze");
}

static void ukazatel(short pozice, uint16_t color)
{
    switch (pozice)
    {
    case 1:
        tft.fillCircle(10, 31, 5, color);
        break;
    case 2:
        tft.fillCircle(10, 51, 5, color);
        break;
    case 3:
        tft.fillCircle(10, 71, 5, color);
        break;
    }
}

/*********************************************************************/