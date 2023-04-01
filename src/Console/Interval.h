#pragma once
#include <Arduino.h>

class Interval
{
private:
    unsigned long length;
    unsigned long currentVal;

public:
    Interval(unsigned long length)
    {
        this->length = length;
        this->currentVal = millis();
    }

    ~Interval()
    {
    }

    bool overcame()
    {
        if ((millis() - this->currentVal) > this->length)
        {
            setCurrentVal();
            return true;
        }
        else
        {
            return false;
        }
    }

    void setCurrentVal()
    {
        this->currentVal = millis();
    }

    unsigned long getLength()
    {
        return this->length;
    }

    void setLength(unsigned long length)
    {
        this->length = length;
    }
};
