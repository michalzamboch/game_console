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

extern void snakeTheGame();
extern void invasionTheGame();
extern void towerOfHanoiTheGame();

void setup()
{
    srand((unsigned int)time(NULL));
    Serial.begin(9600);

    inicializujDisplay();
    inicializaceOvladani();
}

void loop()
{
    delay(100);
    ovladani = 0;
    short zvolenaHra = menuMain();
    ovladani = 0;
    delay(100);

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

void inicializujDisplay()
{
    tft.initR(INITR_BLACKTAB);
    tft.fillScreen(ST7735_BLACK);
    tft.setTextWrap(true);
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
        ;
}

int nahodneCislo(int from, int to)
{
    if (from > to)
    {
        std::swap(from, to);
    }

    int tempFrom = from;
    int posunuti = abs(from);
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