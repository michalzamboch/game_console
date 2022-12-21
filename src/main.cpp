#include <include.h>

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

unsigned int ovladani;

extern void vypisBity(int x, int y);
extern void inicializaceOvladani();
extern short menuMain();

void chyboveHlaseni();
void inicializujDisplay();
int nahodneCislo(int from, int to);
short **twoDimShortArr(int x, int y);
bool prekrocilCas(interval *porovnavany);

/****************************** HRY ********************************/

/*
 * Příklad pro 13. maturitní otázku
 * procedury
 */
extern void snakeTheGame();
extern void invasionTheGame();
extern void towerOfHanoiTheGame();

/*******************************************************************/

// startujici funkce, opakuje se jen jednou
void setup()
{
    srand((unsigned int)time(NULL));
    Serial.begin(9600);

    inicializujDisplay();
    inicializaceOvladani();
}

// funkce, která se opakuje neustále do kola ve smyčce
void loop()
{
    delay(100);
    ovladani = 0;
    short zvolenaHra = menuMain();
    ovladani = 0;
    delay(100);

    /*
     * Příklad pro 10. maturitní otázku
     * příkaz switch
     */
    switch (zvolenaHra)
    {
    case 1:
        snakeTheGame();
        break;
    case 2:
        towerOfHanoiTheGame();
        break;
    case 3:
        invasionTheGame();
        break;
    default:
        chyboveHlaseni();
        break;
    }
    zvolenaHra = 0;
}

/*******************************************************************/

void inicializujDisplay() // inicializace pro diplay
{
    tft.initR(INITR_BLACKTAB);    // Initialize a ST7735S chip, black tab
    tft.fillScreen(ST7735_BLACK); // něco jako clearScreen, vyčistí obrazovku
    tft.setTextWrap(true);        //  text nepoleze mimo display
    tft.setRotation(1);
}

void chyboveHlaseni()
{
    tft.fillScreen(ST7735_BLACK);
    tft.setTextSize(1);
    tft.setTextColor(0xf800);

    tft.setCursor(0, 0);
    tft.println("Chyba!!!");
    tft.println("Restartuj konzoli.");

    while (true)
        ; // lolin se musi restartovat
}

/***********************************************************/

int nahodneCislo(int from, int to) // příklad k maturitní otázce číslo 1 - algoritmus
{
    if (from > to) // ochrana pro správné fungování algoritmu
    {
        std::swap(from, to); // prohodí se from a to
    }

    int tempFrom = from;
    int posunuti = abs(from); // menší hodnota muže být v mínusu a tímto ji překlápíme na
    to++;

    if (tempFrom < 0)
    {
        from += posunuti;
        to += posunuti;
    }

    int generovaneCislo;
    do
    {
        generovaneCislo = rand() % to;
    } while (generovaneCislo < from);

    if (tempFrom < 0)
    {
        generovaneCislo -= posunuti;
    }

    return generovaneCislo;
}

short **twoDimShortArr(int x, int y)
{
    short **vysledek;
    vysledek = new short *[x];

    for (int i = 0; i < x; i++)
    {
        vysledek[i] = new short[y];
    }

    return vysledek;
}

bool prekrocilCas(interval *porovnavany)
{
    if ((millis() - porovnavany->soucasnaHodnota) > porovnavany->interval)
    {
        porovnavany->soucasnaHodnota = millis();
        return true;
    }
    else
    {
        return false;
    }
}

bool opakovatHru()
{
    unsigned long soucasnyCas = millis();
    interval intPrepnuti = {200, soucasnyCas};
    ovladani = 0;

    while (true)
    {
        soucasnyCas = millis();
        if ((ovladani != 0) && ((ovladani & T_1) != T_1) && ((soucasnyCas - intPrepnuti.soucasnaHodnota) > intPrepnuti.interval))
        {
            return false;
        }
        else if ((ovladani > 0) && ((ovladani & T_1) == T_1) && ((soucasnyCas - intPrepnuti.soucasnaHodnota) > intPrepnuti.interval))
        {
            return true;
        }
    }
}