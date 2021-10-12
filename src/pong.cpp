/* *
 * Soubor pong.cpp obsahuje kód ke hře PONG
 * */

#include <include.h>

extern unsigned int ovladani;

extern Adafruit_ST7735 tft;

class pong
{
private:
    const short delkaPloch = 32;

    unsigned short hracBody;
    unsigned short pcBody;
    bool ukonciHru;

    short hracPoz[2];        // interval y1, y2
    short hracPredeslaPoz;
    short pcPoz[2];      // interval y1, y2
    short pcPredeslaPoz;
    short ballPoz[2];    // x, y

    bool kolidujeS(short poziceX, short poziceY, short sCimX, short sCimY);
    bool kolidujeSPc();
    bool kolidujeSHracem();
    
    void vypisBody();
    void vykresliCaru();
    void vykresliHrace();
    void vykresliPc();
    void vykresliBalonek();

    void vypocitejPozicePc();
    void vypocitejPoziceBalonku();

    void ovladaniFunkce();

    void drawline(int x0, int y0, int x1, int y1);

protected:
    void jadroHry();

public:
    pong();
    ~pong();
    friend void pongTheGame();
};

pong::pong()
{
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextSize(2);

    ukonciHru = false;
    hracBody = 0;
    pcBody = 0;

    hracPoz[0] = 16 * 4;
    hracPoz[1] = hracPoz[0] + delkaPloch;
    hracPredeslaPoz = 0;

    pcPoz[0] = 16 * 4;
    pcPoz[1] = pcPoz[0] + delkaPloch;
    pcPredeslaPoz = 0;
}

pong::~pong()
{

}

void pongTheGame()
{
    pong *hraPong = new pong;

    hraPong->jadroHry();

    delete hraPong;
}

void pong::jadroHry()
{
    do
    {
        vypisBody();
        vykresliCaru();

        ovladaniFunkce();
        vykresliHrace();
        vykresliPc();

        //delay();
    } while (ukonciHru == false);
}

bool pong::kolidujeS(short poziceX, short poziceY, short sCimX, short sCimY)
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

void pong::vypisBody()
{
    tft.setCursor(44, 10);
    tft.print(hracBody);

    tft.setCursor(106, 10);
    tft.print(pcBody);
}

void pong::vykresliCaru()
{
    tft.drawLine(80, 0, 80, 128, ST7735_WHITE);
}

void pong::vykresliHrace()
{
    tft.fillRect(10, hracPoz[0], -5, delkaPloch, ST7735_WHITE);
    tft.drawLine(10, hracPredeslaPoz, 5, hracPredeslaPoz, ST7735_BLACK);
}

void pong::drawline(int x0, int y0, int x1, int y1)
{
    int dx, dy, p, x, y;
 
	dx=x1-x0;
	dy=y1-y0;
 
	x=x0;
	y=y0;
    //auto prvni = std::chrono::high_resolution_clock::now();
	p=2*dy-dx;
 
	while(x<x1)
	{
		if(p>=0)
		{
            tft.drawPixel(x, y, ST7735_WHITE);
			y=y+1;
			p=p+2*dy-2*dx;
		}
		else
		{
            tft.drawPixel(x, y, ST7735_WHITE);
			p=p+2*dy;
		}
		x=x+1;
	}
    //auto druhy = std::chrono::high_resolution_clock::now();
    //int nanosek = std::chrono::duration_cast<std::chrono::nanoseconds>(prvni - druhy);
}

void pong::ovladaniFunkce()
{
    if (((ovladani & T_DO_PREDU) == T_DO_PREDU) && (hracPoz[0] > 0))
    {
        hracPredeslaPoz = hracPoz[1];
        hracPoz[0]--;
        hracPoz[1]--;

        delay(10);
    }
    else if (((ovladani & T_DO_ZADU) == T_DO_ZADU)  && (hracPoz[1] < 128))
    {
        hracPredeslaPoz = hracPoz[0];
        hracPoz[0]++;
        hracPoz[1]++;

        delay(10);
    }
    
    if ((ovladani & T_2) == T_2)
    {
        // ukoncit hru
        ukonciHru = true;
    }
}

void pong::vykresliPc()
{
    tft.fillRect(150, pcPoz[0], 5, delkaPloch, ST7735_WHITE);
    tft.drawLine(150, pcPredeslaPoz, 155, pcPredeslaPoz, ST7735_BLACK);
}
