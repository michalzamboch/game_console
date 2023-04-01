#pragma once

class Simulatable
{
private:
    short speedX = 0;
    short speedY = 0;

public:
    virtual void simulate() = 0;

    void setSpeed(short speedX, short speedY)
    {
        this->speedX = speedX;
        this->speedY = speedY;
    }
    
    /*--------------------------------------------------*/

    short getSpeedX()
    {
        return this->speedX;
    }

    void setSpeedX(short speedX)
    {
        this->speedX = speedX;
    }

    void invertSpeedX()
    {
        this->speedX *= -1;
    }

    /*--------------------------------------------------*/

    short getSpeedY()
    {
        return this->speedY;
    }

    void setSpeedY(short speedY)
    {
        this->speedY = speedY;
    }

    void invertSpeedY()
    {
        this->speedY *= -1;
    }

    /*
    virtual bool higher(uint16_t val) = 0;
    virtual bool lower(uint16_t val) = 0;
    virtual bool tooLeft(uint16_t val) = 0;
    virtual bool tooRight(uint16_t val) = 0;
    */
};
