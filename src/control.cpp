/* *
 * Soubor control.cpp složí k inicializování ovládání tlačítky
 * inicializují se zde jednotlivé interrupty
 * */

#include "include.h"

/********************************************************************/

extern Adafruit_ST7735 tft;

extern unsigned int ovladani;

static void jednotlivePiny(void *pin);
static void inicializaceInterruptu();
static void inicializacePinu();
static void obratitBit(unsigned int *porovnanany, unsigned int bit, unsigned int pin);

void vypisBity(int x, int y);
void inicializaceOvladani();

/********************************************************************/

static void jednotlivePiny(void *pin)
{
    unsigned int bit = 0;
    switch ((int)pin)
    {
    case P_DO_PREDU: bit = T_DO_PREDU;
        break;
    case P_DO_ZADU: bit = T_DO_ZADU;
        break;
    case P_DO_LEVA: bit = T_DO_LEVA;
        break;
    case P_DO_PRAVA: bit = T_DO_PRAVA;
        break;
    case P_1: bit = T_1;
        break;
    case P_2: bit = T_2;
        break;
    }

    obratitBit(&ovladani, bit, (int)pin);
}

static void obratitBit(unsigned int *porovnanany, unsigned int bit, unsigned int pin)
{
    delay(10);

    if (((*porovnanany & bit) == 0) && (digitalRead(pin) == LOW))
    {
        delay(10);
        if (digitalRead(pin) == LOW)
        {      
            *porovnanany |= bit;
        }
    }
    else
    {
        delay(10);
        if (digitalRead(pin) == HIGH)
        {
            *porovnanany &= ~bit;
        }
    }
}

static void inicializaceInterruptu()
{
    attachInterruptArg(digitalPinToInterrupt(P_DO_PREDU), jednotlivePiny, (void *)P_DO_PREDU, CHANGE);
    attachInterruptArg(digitalPinToInterrupt(P_DO_ZADU), jednotlivePiny, (void *)P_DO_ZADU, CHANGE);
    attachInterruptArg(digitalPinToInterrupt(P_DO_LEVA), jednotlivePiny, (void *)P_DO_LEVA, CHANGE);
    attachInterruptArg(digitalPinToInterrupt(P_DO_PRAVA), jednotlivePiny, (void *)P_DO_PRAVA, CHANGE);

    attachInterruptArg(digitalPinToInterrupt(P_1), jednotlivePiny, (void *)P_1, CHANGE);
    attachInterruptArg(digitalPinToInterrupt(P_2), jednotlivePiny, (void *)P_2, CHANGE);
}

static void inicializacePinu()
{
    pinMode(P_DO_PREDU, INPUT_PULLUP);
    pinMode(P_DO_ZADU, INPUT_PULLUP);
    pinMode(P_DO_LEVA, INPUT_PULLUP);
    pinMode(P_DO_PRAVA, INPUT_PULLUP);
  
    pinMode(P_1, INPUT_PULLUP);
    pinMode(P_2, INPUT_PULLUP);
    
    // všechny piny propojit s GND (modrý drát s GND)
}

void inicializaceOvladani()
{
    inicializacePinu();
    inicializaceInterruptu();
}

void vypisBity(int x, int y)
{
    tft.setCursor(x, y);
    tft.fillRect(x, y, 140, 20, ST7735_BLACK);
    for (unsigned int bit = 32; bit >= 1; bit >>= 1)
    {
        if ((ovladani & bit) == bit)
        {  
            tft.print("1");
        }
        else
        {
            tft.print("0");
        }
    }
}
