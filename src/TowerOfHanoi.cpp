#include <include.h>

#define DELKA_KOSTKY 5

extern unsigned int ovladani;
extern Adafruit_ST7735 tft;
extern void vypisBity(int x, int y);
extern bool opakovatHru();
extern bool prekrocilCas(interval *porovnavany);

class towerOfHanoi
{
private:
    unsigned int veze[3];
    unsigned int cilovaHodnota;
    int vezePoz[3][2];
    bool ukonciHru;
    short from, to;
    const unsigned int pocetPater = 6;
    int pocetTahu;
    interval intPrepnuti;

    /**** PRIVÁTNÍ FUNKCE ****/
    void vyhodnotHru();

    unsigned int hodnotaPatra(unsigned int indexPatra);
    unsigned int nejensiPatro(int index);
    void presunPatro();

    void vykresliVsechnyVeze();
    void vykresliVez(short x, short y, int indexVeze);
    void vykresliJednoPatro(short x, short y, int indexVeze, unsigned int patro);
    void vypisPocetTahu();
    
    void volba();
    void ukazatel(int pozice, uint16_t color);
    void obratitBit(unsigned int *porovnanany, unsigned int bit);

protected:
    void jadroHry();

public:
    towerOfHanoi();
    ~towerOfHanoi();
    friend void towerOfHanoiTheGame();
};

towerOfHanoi::towerOfHanoi()      // konstruktor
{
    tft.fillScreen(ST7735_BLACK);
    tft.setTextSize(1);
    tft.drawLine(0, 15, 159, 15, ST7735_WHITE);
    tft.setCursor(40, 5);
    tft.print("Pocet tahu: 0");
    
    veze[0] = 0;
    veze[1] = 0;
    veze[2] = 0;
    for (int i = 1; i <= hodnotaPatra(pocetPater); i <<= 1)
    {
        veze[0] |= i;
    }  
    cilovaHodnota = veze[0];
    
    vezePoz[0][0] = 4;
    vezePoz[0][1] = 20;
    vezePoz[1][0] = 52;
    vezePoz[1][1] = 70; 
    vezePoz[2][0] = 100;
    vezePoz[2][1] = 20;

    intPrepnuti = { 200, millis() };

    ukonciHru = false;
    pocetTahu = 0;
}

towerOfHanoi::~towerOfHanoi()       // destruktor
{

}

void towerOfHanoiTheGame()      // funkce, která se volá pro spuštění hry
{
    towerOfHanoi *hratowerOfHanoi = new towerOfHanoi;

    hratowerOfHanoi->jadroHry();

    delete hratowerOfHanoi;
}

void towerOfHanoi::jadroHry()
{
    do
    {
        tft.fillRect(0, 16, 159, 112, ST7735_BLACK);        // vymázání věží z obrazovky
        vykresliVsechnyVeze();      // vykreslení všech tří věží
        
        volba();        // zde se zvolí odkud a kam se bude nějaké patro přesouvat
        if (ukonciHru == false)     // zkontrolu je se jestli hráč nechce odejít ze hry
        {
            presunPatro();      // zde se přesune kotouč/patro na zvolené místo
            vyhodnotHru();      // zkontroluje se, jestli hráč již nevyhrál
        }
    } while (ukonciHru == false);       // cyklus se opakuje dokud funkce vyhodnotHru(), neřekne, že je konec hry a hráč vyhrál
}

/*********************************************** VYKRESLOVACÍ FUNKCE ***********************************************/

void towerOfHanoi::vykresliVsechnyVeze()
{
    vykresliVez(vezePoz[0][0], vezePoz[0][1], 0);
    vykresliVez(vezePoz[1][0], vezePoz[1][1], 1);
    vykresliVez(vezePoz[2][0], vezePoz[2][1], 2);
}

void towerOfHanoi::vykresliVez(short x, short y, int indexVeze)
{
    y += pocetPater * (DELKA_KOSTKY + 1);
    int index;
    for (index = hodnotaPatra(pocetPater); index >= 1; index >>= 1)
    {
        if ((index & veze[indexVeze]) == index)
        {
            vykresliJednoPatro(x, y, indexVeze, index);
            y -= DELKA_KOSTKY + 1;
        }
    }

    for (index = hodnotaPatra(pocetPater); index >= 1; index >>= 1)
    {
        if ((index & veze[indexVeze]) != index)
        {
            vykresliJednoPatro(x, y, indexVeze, index);
            y -= DELKA_KOSTKY + 1;
        }
    }
}

void towerOfHanoi::vykresliJednoPatro(short x, short y, int indexVeze, unsigned int patro)
{
    short cerne = pocetPater - 1;
    short bile = 0;

    if ((veze[indexVeze] & patro) == patro)
    {
        unsigned int temp = 1;
        bile = 1;

        while ((temp & patro) != patro)
        {
            temp <<= 1;
            bile += 2;
            cerne--;
        }

        tft.fillRect(x + cerne * DELKA_KOSTKY, y, bile * DELKA_KOSTKY, DELKA_KOSTKY, ST7735_WHITE);
    }
    else
    {
        int xPozice = x + cerne * DELKA_KOSTKY + DELKA_KOSTKY / 2;
        tft.drawLine(xPozice, y, xPozice, y + DELKA_KOSTKY - 1, ST7735_WHITE);
    }
}

void towerOfHanoi::vypisPocetTahu()
{
    tft.fillRect(112, 0, 60, 14, ST7735_BLACK);
    tft.setCursor(112, 5);
    tft.print(pocetTahu);
}

/*********************************************** OVLÁDÁNÍ - VOLBA VĚŽÍ ***********************************************/

void towerOfHanoi::volba()
{
    short temp[2] = { -1, -1 };
    
    for (int i = 0; i < 2; i++)    
    {
        double soucasnyCas = 0;
        
        while (true)
        {
            soucasnyCas = millis();

            if ((ovladani == T_DO_LEVA) && ((soucasnyCas - intPrepnuti.soucasnaHodnota) > intPrepnuti.interval))
            {
                intPrepnuti.soucasnaHodnota = soucasnyCas;
                
                if (temp[0] == 0)
                {
                    ukazatel(0, ST7735_BLACK);
                    temp[0] = -1;
                    i = -1;
                }
                else
                {
                    ukazatel(0, ST7735_WHITE);
                    temp[i] = 0;
                }

                Serial.printf("Temp[%d] = %d\n", i, temp[i]);
                break;
            }
            else if ((ovladani == T_DO_ZADU) && ((soucasnyCas - intPrepnuti.soucasnaHodnota) > intPrepnuti.interval))
            {
                intPrepnuti.soucasnaHodnota = soucasnyCas;
                
                if (temp[0] == 1)
                {
                    ukazatel(1, ST7735_BLACK);
                    temp[0] = -1;
                    i = -1;
                }
                else
                {
                    ukazatel(1, ST7735_WHITE);
                    temp[i] = 1;
                }
                
                Serial.printf("Temp[%d] = %d\n", i, temp[i]);
                break;
            }
            else if ((ovladani == T_DO_PRAVA) && ((soucasnyCas - intPrepnuti.soucasnaHodnota) > intPrepnuti.interval))
            {
                intPrepnuti.soucasnaHodnota = soucasnyCas;
                
                if (temp[0] == 2)
                {
                    ukazatel(2, ST7735_BLACK);
                    temp[0] = -1;
                    i = -1;
                }
                else
                {
                    ukazatel(2, ST7735_WHITE);
                    temp[i] = 2;
                }

                Serial.printf("Temp[%d] = %d\n", i, temp[i]);
                break;
            }
            else if (ovladani == T_2)
            {
                intPrepnuti.soucasnaHodnota = soucasnyCas;
                ukonciHru = true;
                i = 2;

                break;
            }
        }
    }
    
    from = temp[0];
    to = temp[1];
}

void towerOfHanoi::ukazatel(int pozice, uint16_t color)
{
    int delka = 7 * DELKA_KOSTKY;
    int xPoz = vezePoz[pozice][0] + 2 * DELKA_KOSTKY;
    int yPoz = vezePoz[pozice][1] + (pocetPater + 3) * DELKA_KOSTKY;

    tft.fillRect(xPoz, yPoz, delka, 3, color);
}

/*********************************************** VYHODNOCENÍ HRY ***********************************************/

void towerOfHanoi::vyhodnotHru()
{
    if (veze[2] == cilovaHodnota)
    {
        tft.fillScreen(ST7735_BLACK);
        vykresliVsechnyVeze();
        delay(500);
        ukonciHru = true;
        tft.fillScreen(ST7735_BLACK);

        tft.setTextSize(2);
        tft.setCursor(15, 40);
        tft.setTextColor(ST7735_WHITE);
        tft.printf("VICTORY!");

        tft.setCursor(15, 60);
        tft.printf("POCET TAHU:");
        tft.setCursor(15, 80);
        tft.printf("%d", pocetTahu);
        
        double soucasnyCas = millis();
        intPrepnuti.soucasnaHodnota = soucasnyCas;
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
}

/*********************************************** PATRA ***********************************************/

unsigned int towerOfHanoi::hodnotaPatra(unsigned int indexPatra)
{
    if (indexPatra > 31 || indexPatra < 1)
    {
        return 0;
    }
    indexPatra--;
    
    return (1 << indexPatra);
}

unsigned int towerOfHanoi::nejensiPatro(int index)
{
    for (int i = 1; i <= hodnotaPatra(pocetPater); i <<= 1)
    {
        if ((i & veze[index]) == i)
        {
            return i;
        }
    }
    
    return 0;
}

void towerOfHanoi::presunPatro()
{
    unsigned int tempPatroFrom = nejensiPatro(from);
    unsigned int tempPatroTo = nejensiPatro(to);

    if (( (tempPatroFrom < tempPatroTo) || ((tempPatroTo == 0) && (tempPatroFrom > 0)) ) && (from != to)&& (tempPatroFrom > 0))
    {
        Serial.printf("From: %d, To: %d\n", tempPatroFrom, tempPatroTo);
        obratitBit(&veze[from], tempPatroFrom);
        obratitBit(&veze[to], tempPatroFrom);
        pocetTahu++;
        vypisPocetTahu();
    }
}

void towerOfHanoi::obratitBit(unsigned int *porovnanany, unsigned int bit)
{
    if ((*porovnanany & bit) == 0)
    {
        *porovnanany |= bit;
    }
    else
    {
        *porovnanany &= ~bit;
    }
}
