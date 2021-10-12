/* *
 * Soubor invasion.cpp obsahuje kód ke hře invasion
 * */

#include <include.h>

extern unsigned int ovladani;

extern Adafruit_ST7735 tft;
extern int nahodneCislo(int from, int to);
extern short **twoDimShortArr(int x, int y);
extern bool opakovatHru();
extern bool prekrocilCas(interval *porovnavany);

#define X_MAX 159
#define Y_MAX 127
#define X_MIN 0
#define Y_MIN 15

/******************************************* HLAVNÍ TŘÍDA invasion *******************************************/

class invasion
{
private:
    /******* OBECNE PROMENNE *******/
    bool ukonciHru;
    int skore;
    interval intZtizeni;

    /******* PROMENNE K RAKETCE *******/
    const short sirkaRaketky = 9;
    const short vyskaRaketky = 5;
    const short raketkaPozY = 115;
    short raketkaPozX;
    interval intPohybRak;

    /******* PROMENNE K NEPRATELUM ******/
    short pocetNepratel;
    short **nepratele;
    const short nepritelSirkaDelka = 11;
    interval intPohybNepratel;
    interval intSpawnNepratele;

    /******* PROMENNE KE STRELACH ******/

    /*
     * Příkad k maturitní otázce číslo 7
     * STATICKÉ A DYNAMICKÉ PROMĚNNÉ
    */
    short pocetStrel;
    short **strely;
    const short strelaDelka = 7;
    const short strelaSirka = 3;
    interval intVystrel;
    interval intPohybStrely;

    /********************** PRIVATNI FUNKCE **********************/
    
    /******* OBECNE FUNKCE *******/
    void ovladaniFunkce();
    void konecneHlaseni();
    void vypisSkore();
    void ztizeniHry();
    
    /******* KOLIZE *******/
    bool komplexKontrolaKolizi();
    bool dveUzemiKoliduji(int a1, int a2, int b1, int b2);
    bool kontrolaProniknutiNepritele(int indexNepritele);
    bool kolidujeS(short poziceX, short poziceY, short sCimX, short sCimY);
    bool kolizeStrelyNepritele(int indexStrely, int indexNepritele);
    bool kolizeRaketkyNepritele(int indexNepritele);
    bool komplexKontrolaSestreleniStrelyNepritele();
    
    /****** FUNKCE K RAKETCE *******/
    void vykresliRaketku(uint16_t color);

    /****** FUNKCE K NEPRATELUM *******/
    void vykresliJednohoNepritele(short index, uint16_t color);
    void vytvorNepratele();
    void vytvorJednohoNepritele();
    void vykresliAPosunNepratele(int oKolik);
    void vymazNepratele();
    void vymazJednohoNepritele(short index);

    /****** FUNKCE K STRELE *******/
    void vykresliJednuStrelu(short index, uint16_t color);
    void vytvorStrelu();
    void vykresliAPosunStrely(int oKolik);
    void vymazStrely();
    void vymazJednuStrelu(short index);
    void vymazZmizeleStrely();

protected:
    void jadroHry();

public:
    invasion();
    ~invasion();
    friend void invasionTheGame();
};

/********************************** KOSTRA PROGRAMU **********************************/

invasion::invasion()      // konstruktor
{
    skore = 0;
    tft.fillScreen(ST7735_BLACK);
    tft.setTextSize(1);
    tft.drawLine(X_MIN, Y_MIN, X_MAX, Y_MIN, ST7735_WHITE);
    tft.setCursor(50, 5);
    tft.printf("Skore: %d", skore);

    unsigned long soucasnyCas = millis();
    intVystrel = {700, soucasnyCas};
    intPohybStrely = {30, soucasnyCas};
    intPohybRak = {10, soucasnyCas};
    intPohybNepratel = {75, soucasnyCas};
    intSpawnNepratele = {4000, soucasnyCas};
    intZtizeni = {6000, soucasnyCas};
    
    raketkaPozX = 80;
    pocetNepratel = 0;
    pocetStrel = 0;
    ukonciHru = false;
    nepratele = NULL;
    strely = NULL;
}

invasion::~invasion()       // destruktor
{
    /*
     * vymaz Nepritele a strely
    */

   vymazNepratele();
   vymazStrely();
}

void invasionTheGame()      // funkce, která se volá pro spuštění hry
{
    invasion *hraInvasion = new invasion;

    hraInvasion->jadroHry();

    delete hraInvasion;
}

void invasion::jadroHry()
{
    vykresliRaketku(ST7735_GREEN);      // počáteční vykreslení raketky na display
    
    do      // počátek cyklu v kterém se odehrává celá hra
    {
        ovladaniFunkce();       // zjistí směr kam se má nebo nemá pohnout, jestli má raketka vystřelit nebo ukončit hru

        if (ukonciHru == false)     // pokud NENÍ zvoleno ukončení hry, pokračuje se zde kde prbíhá vnitřní logika programu / vyhodnocování stavu hry
        {
            vytvorNepratele();      // funkce, která v určitém časovém intervalu, tvoří nové nepřátele

            vykresliAPosunStrely(1);        // posune a vykreslí střely na display
            vykresliAPosunNepratele(1);        // posune a vykreslí nepratele na display

            vymazZmizeleStrely();       // pokud nějaká střela netrefí cíl a jde mimo display, program ji vymaže
            
            if (komplexKontrolaKolizi() == true)        // zde se zkontroluje, zda se nesrazila střela s nepřítelem a nebo jestli nepřítel nenarazil do raketky
            {                                           // pokud by nepritel narazil do raketky hra končí, pokud střela do nepřítele přičtou se hráči body
                konecneHlaseni();
            }

            ztizeniHry();       // v určitém časovém intervalu, se hra ztíží: zrychlením nepřátel a zmenšením intervalu, kdy objevují nepřátelé
        }
    } while (ukonciHru == false);  
}

/*********************************************** OVLÁDÁNÍ ***********************************************/

void invasion::ovladaniFunkce()
{
    unsigned long soucasnyCas = millis();
    if (((ovladani & T_DO_PRAVA) == T_DO_PRAVA) && ((soucasnyCas - intPohybRak.soucasnaHodnota) > intPohybRak.interval) && (raketkaPozX < (159 - sirkaRaketky)))
    {
        intPohybRak.soucasnaHodnota = soucasnyCas;
        vykresliRaketku(ST7735_BLACK);
        raketkaPozX++;
        vykresliRaketku(ST77XX_GREEN);
    }
    else if (((ovladani & T_DO_LEVA) == T_DO_LEVA)  && ((soucasnyCas - intPohybRak.soucasnaHodnota) > intPohybRak.interval) && (raketkaPozX > 0))
    {
        intPohybRak.soucasnaHodnota = soucasnyCas;
        vykresliRaketku(ST7735_BLACK);
        raketkaPozX--;
        vykresliRaketku(ST77XX_GREEN);
    }
    
    if (((ovladani & T_1) == T_1) && ((soucasnyCas - intVystrel.soucasnaHodnota) > intVystrel.interval))
    {
        intVystrel.soucasnaHodnota = soucasnyCas;
        vytvorStrelu();
    }
    
    if ((ovladani & T_2) == T_2)
    {
        // ukoncit hru
        ukonciHru = true;
    }
}

/********************************** FUNKCE K OBECNÉ GRAFICE **********************************/

void invasion::vykresliRaketku(uint16_t color)
{
    tft.drawRoundRect(raketkaPozX, raketkaPozY, sirkaRaketky, vyskaRaketky, 2, color);
}

/*********************************************** TESTY KOLIZÍ ***********************************************/

bool invasion::kolidujeS(short poziceX, short poziceY, short sCimX, short sCimY)
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

bool invasion::kolizeRaketkyNepritele(int indexNepritele)
{
    bool shodaXPozic = dveUzemiKoliduji(raketkaPozX, raketkaPozX + sirkaRaketky, nepratele[indexNepritele][0], nepratele[indexNepritele][0] + nepritelSirkaDelka);

    bool shodaYPozic = dveUzemiKoliduji(raketkaPozY, raketkaPozY + vyskaRaketky, nepratele[indexNepritele][1], nepratele[indexNepritele][1] + nepritelSirkaDelka);
    
    if ((shodaXPozic == true) && (shodaYPozic == true))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool invasion::kolizeStrelyNepritele(int indexStrely, int indexNepritele)
{
    bool shodaXPozic = dveUzemiKoliduji(strely[indexStrely][0], strely[indexStrely][0] + strelaSirka, nepratele[indexNepritele][0], nepratele[indexNepritele][0] + nepritelSirkaDelka);

    bool shodaYPozic = dveUzemiKoliduji(strely[indexStrely][1], strely[indexStrely][1] + strelaDelka, nepratele[indexNepritele][1], nepratele[indexNepritele][1] + nepritelSirkaDelka);
    
    if ((shodaXPozic == true) && (shodaYPozic == true))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool invasion::komplexKontrolaSestreleniStrelyNepritele()
{
    for (int nepritelIndex = 0; nepritelIndex < pocetNepratel; nepritelIndex++)
    {
        for (int strelyIndex = 0; strelyIndex < pocetStrel; strelyIndex++)
        {
            if (kolizeStrelyNepritele(strelyIndex, nepritelIndex) == true)
            {
                vykresliJednohoNepritele(nepritelIndex, ST7735_BLACK);
                vykresliJednuStrelu(strelyIndex, ST7735_BLACK);
                vymazJednohoNepritele(nepritelIndex);
                vymazJednuStrelu(strelyIndex);

                skore += 100;
                vypisSkore();
                return true;
            }
        }
    }
    return false;
}

bool invasion::komplexKontrolaKolizi()
{
    while (komplexKontrolaSestreleniStrelyNepritele() == true);

    for (int nepritelIndex = 0; nepritelIndex < pocetNepratel; nepritelIndex++)
    {
        if (kolizeRaketkyNepritele(nepritelIndex) == true)
        {
            ukonciHru = true;
            return true;
        }

        if (kontrolaProniknutiNepritele(nepritelIndex) == true)
        {
            ukonciHru = true;
            return true;
        }
        
        /* mozna tady pridat dalsi kontroly: kolize s Raketkou nebo projiti hranic */
    }

    return false;
}

bool invasion::kontrolaProniknutiNepritele(int indexNepritele)
{
    if (nepratele[indexNepritele][1] > (127 - nepritelSirkaDelka))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool invasion::dveUzemiKoliduji(int a1, int a2, int b1, int b2)
{
    if (a1 > a2)
    {
        std::swap(a1, a2);
    }
    if (b1 > b2)
    {
        std::swap(b1, b2);
    }

    while (true)
    {
        if ((b1 >= a1) && (b1 <= a2))
        {
            return true;
        }

        if (b1 == b2)
        {
            break;
        }
        else
        {
            b1++;
        }
    }
    return false;
}

/*********************************************** STŘELY ***********************************************/

void invasion::vymazStrely()
{
    if (pocetStrel > 0)
    {
        for (int i = 0; i < pocetStrel; i++)
        {
            delete[] strely[i];
            strely[i] = NULL;
        }
        delete[] strely;
        strely = NULL;
    }
}

void invasion::vytvorStrelu()
{
    short **tempStrely = twoDimShortArr(pocetStrel + 1, 2);

    for (int i = 0; i < pocetStrel; i++)
    {
        tempStrely[i][0] = strely[i][0];
        tempStrely[i][1] = strely[i][1];
    }
    
    tempStrely[pocetStrel][0] = raketkaPozX + 3;
    tempStrely[pocetStrel][1] = raketkaPozY - 10;

    vymazStrely();

    strely = tempStrely;
    tempStrely = NULL;

    pocetStrel++;
}

void invasion::vykresliAPosunStrely(int oKolik)
{
    unsigned long soucasnyCas = millis();
    if ((soucasnyCas - intPohybStrely.soucasnaHodnota) > intPohybStrely.interval)
    {
        intPohybStrely.soucasnaHodnota = soucasnyCas;
        for (int i = 0; i < pocetStrel; i++)
        {
            vykresliJednuStrelu(i, ST7735_BLACK);
            strely[i][1] -= oKolik;
            vykresliJednuStrelu(i, ST7735_WHITE);
        }
    }
}

void invasion::vykresliJednuStrelu(short index, uint16_t color)
{
    tft.fillRect(strely[index][0], strely[index][1], strelaSirka, strelaDelka, color);
}

void invasion::vymazJednuStrelu(short index)
{
    if (pocetStrel == 1)
    {
        vymazStrely();
        pocetStrel = 0;
    }
    else
    {
        short **tempStrely = NULL;
        tempStrely = twoDimShortArr(pocetStrel - 1, 2);
        int zapisovaciIndex = 0;
        for (int i = 0; i < pocetStrel; i++)
        {
            if (i != index)
            {
                tempStrely[zapisovaciIndex][0] = strely[i][0];
                tempStrely[zapisovaciIndex][1] = strely[i][1];
                zapisovaciIndex++;
            }
        }
        
        vymazStrely();

        strely = tempStrely;
        tempStrely = NULL;
        
        pocetStrel--;
    }
}

void invasion::vymazZmizeleStrely()
{
    int i = 0;
    while (i < pocetStrel)
    {
        if (strely[i][1] <= (Y_MIN + 1))
        {
            vykresliJednuStrelu(i, ST7735_BLACK);
            vymazJednuStrelu(i);
            i = 0;
        }
        else
        {
            i++;
        }
    }
}

/*********************************************** NEPRATELE ***********************************************/

void invasion::vykresliJednohoNepritele(short index, uint16_t color)
{
    tft.fillTriangle(nepratele[index][0], nepratele[index][1],
    nepratele[index][0] + nepritelSirkaDelka - 1, nepratele[index][1],
    nepratele[index][0] + nepritelSirkaDelka / 2, nepratele[index][1] + nepritelSirkaDelka, color);
}

void invasion::vytvorNepratele()
{
    if (prekrocilCas(&intSpawnNepratele) == true)
    {
        short **tempNepratele = twoDimShortArr(pocetNepratel + 1, 2);

        for (int i = 0; i < pocetNepratel; i++)
        {
            tempNepratele[i][0] = nepratele[i][0];
            tempNepratele[i][1] = nepratele[i][1];
        }
        
        tempNepratele[pocetNepratel][0] = nahodneCislo(nepritelSirkaDelka, X_MAX - 10);
        tempNepratele[pocetNepratel][1] = nahodneCislo(Y_MIN + 1, 20);

        vymazNepratele();

        nepratele = tempNepratele;
        tempNepratele = NULL;

        pocetNepratel++;
    }
}

void invasion::vytvorJednohoNepritele()
{

}

void invasion::vykresliAPosunNepratele(int oKolik)
{
    unsigned long soucasnyCas = millis();
    if ((soucasnyCas - intPohybNepratel.soucasnaHodnota) > intPohybNepratel.interval)
    {
        intPohybNepratel.soucasnaHodnota = soucasnyCas;
        for (int i = 0; i < pocetNepratel; i++)
        {
            tft.fillRect(nepratele[i][0], nepratele[i][1], nepritelSirkaDelka, oKolik, ST7735_BLACK);
            nepratele[i][1] += oKolik;
            vykresliJednohoNepritele(i, ST7735_WHITE);
        }
    }
}

void invasion::vymazNepratele()
{
    if (pocetNepratel > 0)
    {
        for (int i = 0; i < pocetNepratel; i++)
        {
            delete[] nepratele[i];
            nepratele[i] = NULL;
        }
        delete[] nepratele;
        nepratele = NULL;
    }
}

void invasion::vymazJednohoNepritele(short index)
{
    short **tempNepratele = NULL;

    tempNepratele = twoDimShortArr(pocetNepratel - 1, 2);
    int zapisovaciIndex = 0;
    for (int i = 0; i < pocetNepratel; i++)
    {
        if (i != index)
        {
            tempNepratele[zapisovaciIndex][0] = nepratele[i][0];
            tempNepratele[zapisovaciIndex][1] = nepratele[i][1];
            zapisovaciIndex++;
        }
    }
    
    vymazNepratele();

    nepratele = tempNepratele;
    tempNepratele = NULL;

    pocetNepratel--;
}

/*********************************************** VÝPISY ***********************************************/

void invasion::vypisSkore()
{
    tft.fillRect(90, 0, 60, 14, ST7735_BLACK);
    tft.setCursor(90, 5);
    tft.print(skore);
}

void invasion::konecneHlaseni()
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
    
    while (true)
    {
        soucasnyCas = millis();
        if ((ovladani > 0) && ((soucasnyCas - intPrepnuti.soucasnaHodnota) > intPrepnuti.interval))
        {
            break;
        }
    }
}

/*********************************************** ZTÍŽENÍ HRY ***********************************************/

void invasion::ztizeniHry()
{
    if (prekrocilCas(&intZtizeni) == true)
    {
        if (intSpawnNepratele.interval > 700)
        {
            intSpawnNepratele.interval -= 100;
        }

        if (intPohybNepratel.interval > 55)
        {
            intPohybNepratel.interval -= 1;
        }
        Serial.printf("Ztizeni: %lu, Spawn: %lu, Pohyb: %lu\n", intZtizeni.soucasnaHodnota, intSpawnNepratele.interval, intPohybNepratel.interval);
    }
}