/* *
 * Soubor snake.cpp obsahuje kód ke hře SNAKE
 * */

#include "include.h"

#define PIXEL 8
#define X (160 / PIXEL)
#define Y (128 / PIXEL)
#define X_MAX (X - 1)
#define Y_MAX (Y - 1)
#define X_MIN 0
#define Y_MIN 2

extern unsigned int ovladani;
extern Adafruit_ST7735 tft;
extern int nahodneCislo(int from, int to);
extern short **twoDimShortArr(int x, int y);
extern bool opakovatHru();
extern bool prekrocilCas(interval *porovnavany);
extern void vypisBity(int x, int y);

/*
 * Příklad pro 14. maturitní otázku
 * Metoda - třída (objekt)
*/
class snake
{
private:
    /*
     * Příkad k maturitní otázce číslo 3
     *  Datové typy
    */

    short delka;
    short **had;
    short mazaciKostka[2];
    interval intZmenaSmeru;
    interval intVykresliPosun;

    /*
     * Příkad k maturitní otázce číslo 5
     * POLE
    */
    short ovoce[2];
    time_t casOvoce;
    bool objevSe;
    int skore;

    bool ukonciHru;
    int smer;

    void vypisSkore();
    void vyhodnotHru();

    void vykresliHada();
    void vykresliKostickuHada(int kostickaIndex, uint16_t color);
    void prodluzHada();
    void vymazHada();

    void posunHadaOKostku();
    void posunutiVeSmeru(int yIndex, int zmena, int extreme);
    void zmenSmer(int tempSmer);
    void ovladaniFunkce();

    void ovoceFunkce();
    void snezeniOvoce();
    void prodluzHada(short oKolik);
    
    bool kolidujeSHadem(short poziceX, short poziceY);
    bool kolidujeSeZdi(short poziceX, short poziceY);
    bool kolidujeS(short poziceX, short poziceY, short sCimX, short sCimY);
    bool hadKolidujeSamSeSebou();
    bool hadNarazil();

protected:
    void jadroHry();

public:
    snake();        // konstruktor
    ~snake();       // destruktor
    friend void snakeTheGame();
};

/***************************************** KOSTRA HRY *****************************************/

snake::snake()      // konstruktor
{
    tft.fillScreen(ST7735_BLACK);
    tft.setTextSize(1);
    tft.drawLine(0, (2 * PIXEL - 1), 159, (2 * PIXEL - 1), ST7735_WHITE);
    tft.setCursor(50, 5);
    tft.print("Skore: ");

    ovoce[0] = -1;
    ovoce[1] = -1;
    time(&casOvoce);
    objevSe = true;

    delka = 7;
    skore = 0;
    smer = T_DO_PRAVA;
    ukonciHru = false;

    unsigned long soucasnyCas = millis();
    intVykresliPosun = {250, soucasnyCas};
    intZmenaSmeru = {250, soucasnyCas};

    had = twoDimShortArr(7, 2);
    short tempX = 10;
    short tempY = 7;
    mazaciKostka[0] = tempX - 1;
    mazaciKostka[1] = tempY;

    /*
     * Příklad pro 11. mat. ot.
     * for cyklus
    */
    for (int i = 0; i < delka; i++)
    {
        had[i][0] = tempX;
        had[i][1] = tempY;
        tempX--;
    }
}

snake::~snake()       // destruktor
{
    vymazHada();
}

void snake::vymazHada()
{
    for (int i = 0; i < delka; i++)
    {
        delete[] had[i];
        had[i] = NULL;
    }

    delete[] had;
    had = NULL;
}

/* snakeTheGame()
 * celková hra, neptří do třídy snake
*/
void snakeTheGame()      // funkce, která se volá pro spuštění hry
{
    snake *hraSnake = new snake;

    hraSnake->jadroHry();

    delete hraSnake;
}

void snake::jadroHry()
{
    vypisSkore();
    vykresliHada();
    
    /*
     * Příklad pro 11. mat. ot.
     * do-while cyklus
    */
    do
    {
        ovladaniFunkce();      // čtení z ovladani + nasměrování kam had poleze
        
        ovoceFunkce();
        snezeniOvoce();
    
        if (ukonciHru == false)
        {
            if (prekrocilCas(&intVykresliPosun) == true)
            {
                posunHadaOKostku();
                vykresliHada();
            }          
        
            if (hadNarazil() == true)
            {
                vyhodnotHru();  // pak už vyskočí
            }
        }
    } while (ukonciHru == false);
}

/***************************************** VYHODNOCENI HRY *****************************************/

void snake::vyhodnotHru()
{
    ukonciHru = true;
    tft.fillScreen(ST7735_BLACK);
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(2);
    tft.setCursor(25, 40);
    tft.printf("GAME OVER");

    tft.setTextSize(2);
    tft.setCursor(25, 60);
    tft.printf("SKORE:");
    tft.setCursor(25, 80);
    tft.printf("%d", skore);

    unsigned long soucasnyCas = millis();
    interval intPrepnuti = { 200, soucasnyCas };
    ovladani = 0;
    
    /*
     * Příklad pro 11. mat. ot.
     * while cyklus
    */
    while (true)
    {
        soucasnyCas = millis();
        if ((ovladani > 0) && ((soucasnyCas - intPrepnuti.soucasnaHodnota) > intPrepnuti.delka))
        {
            break;
        }
    }
}

/***************************************** VYKRESLOVÁNÍ *****************************************/

void snake::vykresliHada()
{
    tft.fillRect(mazaciKostka[0] * PIXEL, mazaciKostka[1] * PIXEL, PIXEL, PIXEL, ST7735_BLACK);

    mazaciKostka[0] = had[delka - 1][0];
    mazaciKostka[1] = had[delka - 1][1];

    for (int index = 0; index < delka; index++)
    {
        vykresliKostickuHada(index, ST7735_GREEN);
    }
}

void snake::vykresliKostickuHada(int kostickaIndex, uint16_t color)
{
    tft.fillRoundRect(had[kostickaIndex][0] * PIXEL, had[kostickaIndex][1] * PIXEL, PIXEL, PIXEL, 1, color);
}

/********************************************** OVOCE **********************************************/

void snake::ovoceFunkce()
{
    time_t casOvoce2;
    time(&casOvoce2);
    double casRozdil = difftime(casOvoce2, casOvoce);
    
    if ((objevSe == true) && (casRozdil > 6))
    {
        do
        {
            ovoce[0] = nahodneCislo(X_MIN, X_MAX);
            ovoce[1] = nahodneCislo(Y_MIN, Y_MAX);
        } while (kolidujeSHadem(ovoce[0], ovoce[1]) == true);

        tft.fillRoundRect(ovoce[0] * PIXEL, ovoce[1] * PIXEL, PIXEL, PIXEL, PIXEL / 2, ST7735_YELLOW);
        
        objevSe = false;
        time(&casOvoce);
    }
    else if ((objevSe == false) && (casRozdil > 5))
    {
        if (kolidujeSHadem(ovoce[0], ovoce[1]) == false)
        {
            tft.fillRoundRect(ovoce[0] * PIXEL, ovoce[1] * PIXEL, PIXEL, PIXEL, PIXEL / 2, ST7735_BLACK);
        }
        ovoce[0] = -1;
        ovoce[1] = -1;

        objevSe = true;
        time(&casOvoce);
    }
}

void snake::snezeniOvoce()
{
    if (kolidujeSHadem(ovoce[0], ovoce[1]) == true)
    {
        ovoce[0] = -1;
        ovoce[1] = -1;

        objevSe = true;
        time(&casOvoce);

        prodluzHada(1);
        skore += 100;
        vypisSkore();
    }
}

void snake::prodluzHada(short oKolik)
{
    short **tempHad = twoDimShortArr(delka + 1, 2);
    
    for (int i = 0; i < delka; i++)
    {
        tempHad[i][0] = had[i][0];
        tempHad[i][1] = had[i][1];
    }
    
    tempHad[delka][0] = mazaciKostka[0];
    tempHad[delka][1] = mazaciKostka[1];
    mazaciKostka[0] = -1;
    mazaciKostka[1] = -1;
    
    if (delka > 0)
    {
        vymazHada();
    }
    
    had = tempHad;
    tempHad = NULL;

    delka++;
}

void snake::vypisSkore()
{
    tft.fillRect(90, 0, 60, 14, ST7735_BLACK);
    tft.setCursor(90, 5);
    tft.print(skore);
}

/*********************************************** TESTY KOLIZÍ ***********************************************/

bool snake::kolidujeSHadem(short poziceX, short poziceY)
{
    for (short i = 0; i < delka; i++)
    {
        if (kolidujeS(poziceX, poziceY, had[i][0], had[i][1]))
        {
            return true;
        } 
    }
    
    return false;
}

bool snake::kolidujeS(short poziceX, short poziceY, short sCimX, short sCimY)
{
    if ((poziceX == sCimX) && (poziceY == sCimY))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool snake::kolidujeSeZdi(short poziceX, short poziceY)
{
    if ((poziceX > X_MAX) || (poziceX < X_MIN))
    {
        return true;
    }
    
    if ((poziceY > Y_MAX) || (poziceY < Y_MIN))
    {
        return true;
    }
    
    return false;
}

bool snake::hadNarazil()
{
    if (hadKolidujeSamSeSebou() == true)
    {
        ukonciHru = true;
        return true;
    }
    else
    {
        return false;
    }
}

bool snake::hadKolidujeSamSeSebou()
{
    for (int i = 2; i < delka; i++)
    {
        if ((had[i][0] == had[0][0]) && (had[i][1] == had[0][1]))
        {
            return true;
        }
    }
    return false;
}

/*********************************************** POHYB HADA ***********************************************/

void snake::posunHadaOKostku()
{
    for (int i = (delka - 1); i > 0; i--)
    {
        had[i][0] = had[i - 1][0];
        had[i][1] = had[i - 1][1];
    }
    
    /*
     * 2. příklad pro 10. maturitní otázku
     * if/else podmínky
    */
    if (smer == T_DO_LEVA)
    {
        posunutiVeSmeru(0, -1, X_MAX);
    }
    else if (smer == T_DO_PREDU)
    {
        posunutiVeSmeru(1, -1, Y_MAX);
    }
    else if (smer == T_DO_PRAVA)
    {
        posunutiVeSmeru(0, 1, X_MIN);
    }
    else if (smer == T_DO_ZADU)
    {
        posunutiVeSmeru(1, 1, Y_MIN);
    }
}

void snake::posunutiVeSmeru(int yIndex, int zmena, int extreme)
{
    short tester[2];
    
    tester[0] = had[0][0];
    tester[1] = had[0][1];
    tester[yIndex] += zmena;

    if (kolidujeSeZdi(tester[0], tester[1]) == true)
    {
        had[0][yIndex] = extreme;
    }
    else
    {
        had[0][yIndex] += zmena;
    }
}

void snake::ovladaniFunkce()
{
    if ((ovladani & T_2) == T_2)
    {
        ukonciHru = true;
        return;
    }

    unsigned long soucasnyCas = millis();
    if ((ovladani == T_DO_PRAVA) && (smer != T_DO_PRAVA) && (smer != T_DO_LEVA) && ((soucasnyCas - intZmenaSmeru.soucasnaHodnota) > intZmenaSmeru.delka))
    {
        zmenSmer(T_DO_PRAVA);
        return;
    }
    else if ((ovladani == T_DO_LEVA) && (smer != T_DO_LEVA) && (smer != T_DO_PRAVA) && ((soucasnyCas - intZmenaSmeru.soucasnaHodnota) > intZmenaSmeru.delka))
    {
        zmenSmer(T_DO_LEVA);
        return;
    }
    else if ((ovladani == T_DO_PREDU) && (smer != T_DO_PREDU) && (smer != T_DO_ZADU) && ((soucasnyCas - intZmenaSmeru.soucasnaHodnota) > intZmenaSmeru.delka))
    {
        zmenSmer(T_DO_PREDU);
        return;
    }
    else if ((ovladani == T_DO_ZADU) && (smer != T_DO_ZADU) && (smer != T_DO_PREDU) && ((soucasnyCas - intZmenaSmeru.soucasnaHodnota) > intZmenaSmeru.delka))
    {
        zmenSmer(T_DO_ZADU);
        return;
    }

}

void snake::zmenSmer(int tempSmer)
{
    smer = tempSmer;
    ovladani = 0;
    intZmenaSmeru.soucasnaHodnota = millis();
}
