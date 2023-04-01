#pragma once
//#include "include.h"
#include "Console/Interval.h"

class Control
{
    /*----------------------------------------------- PRIVATE ----------------------------------------------------*/
private:
    static unsigned int control;
    static const int delayTime = 10;
    // static Interval changeTime;

    static struct
    {
        static const unsigned int up = 1;
        static const unsigned int down = 2;
        static const unsigned int left = 4;
        static const unsigned int right = 8;
        static const unsigned int topRight = 16;
        static const unsigned int topLeft = 32;
    } Button;

    static struct
    {
        static const int up = 13;
        static const int down = 12;
        static const int left = 25;
        static const int right = 27;
        static const int topLeft = 33;
        static const int topRight = 32;
    } Pin;

    /*---------------------------------------------------------------------------------------------------------*/

    static void singlePins(void *pin)
    {
        unsigned int bit = 0;
        switch ((int)pin)
        {
        case Control::Pin.up:
            bit = Control::Button.up;
            break;
        case Control::Pin.down:
            bit = Control::Button.down;
            break;
        case Control::Pin.left:
            bit = Control::Button.left;
            break;
        case Control::Pin.right:
            bit = Control::Button.right;
            break;
        case Control::Pin.topRight:
            bit = Control::Button.topRight;
            break;
        case Control::Pin.topLeft:
            bit = Control::Button.topLeft;
            break;
        }

        invertBit(&Control::control, bit, (int)pin);
    }

    static void invertBit(unsigned int *compared, unsigned int bit, unsigned int pin)
    {
        delay(Control::delayTime);

        if (((*compared & bit) == 0) && (digitalRead(pin) == LOW))
        {
            delay(Control::delayTime);
            if (digitalRead(pin) == LOW)
            {
                *compared |= bit;
            }
        }
        else
        {
            delay(Control::delayTime);
            if (digitalRead(pin) == HIGH)
            {
                *compared &= ~bit;
            }
        }
    }

    /*---------------------------------------------------------------------------------------------------------*/

    static bool pressed(unsigned int button)
    {
        if ((Control::control & button) == button)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    static bool onlyPressed(unsigned int button)
    {
        if (Control::control == button)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /*---------------------------------------------- PUBLIC ---------------------------------------------------*/
public:
    Control()
    {
        attachInterruptArg(digitalPinToInterrupt(Control::Pin.up), singlePins, (void *)Control::Pin.up, CHANGE);
        attachInterruptArg(digitalPinToInterrupt(Control::Pin.down), singlePins, (void *)Control::Pin.down, CHANGE);
        attachInterruptArg(digitalPinToInterrupt(Control::Pin.left), singlePins, (void *)Control::Pin.left, CHANGE);
        attachInterruptArg(digitalPinToInterrupt(Control::Pin.right), singlePins, (void *)Control::Pin.right, CHANGE);

        attachInterruptArg(digitalPinToInterrupt(Control::Pin.topRight), singlePins, (void *)Control::Pin.topRight, CHANGE);
        attachInterruptArg(digitalPinToInterrupt(Control::Pin.topLeft), singlePins, (void *)Control::Pin.topLeft, CHANGE);
    }

    /*---------------------------------------------------------------------------------------------------------*/

    static bool upPressed()
    {
        return false;
    }

    static bool downPressed()
    {
        return false;
    }

    static bool rightPressed()
    {
        return false;
    }

    static bool leftPressed()
    {
        return false;
    }

    static bool topRightPressed()
    {
        return false;
    }

    static bool topLeftPressed()
    {
        return false;
    }

    /*---------------------------------------------------------------------------------------------------------*/
    // only pressed one individual key

    static bool onlyUpPressed()
    {
        return false;
    }

    static bool onlyDownPressed()
    {
        return false;
    }

    static bool onlyRightPressed()
    {
        return false;
    }

    static bool onlyLeftPressed()
    {
        return false;
    }

    static bool onlyTopRightPressed()
    {
        return false;
    }

    static bool onlyTopLeftPressed()
    {
        return false;
    }
};