#pragma once
#include <string>
#include "Shapes/Rect.h"

using namespace std;

class Game
{
private:
    string name = "";
    bool stopGame = false;

protected:
    string getName()
    {
        return this->name;
    }

    string setName(string name)
    {
        this->name = name;
    }

    bool getStopGame()
    {
        return this->stopGame;
    }

    void setStopGame(bool stopGame)
    {
        this->stopGame = stopGame;
    }

    /*-------------------------------------------------------*/

    bool pointIn(int x, int y, Rect r)
    {
        if ((x >= r.getX() && y >= r.getY()) &&
            (x <= r.getX2() && y >= r.getY()) &&
            (x >= r.getX() && y <= r.getY2()) &&
            (x <= r.getX2() && y <= r.getY2()))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool rectInRect(Rect r1, Rect r2)
    {
        if (pointIn(r1.getX(), r1.getY(), r2) == true ||
            pointIn(r1.getX2(), r1.getY(), r2) == true ||
            pointIn(r1.getX(), r1.getY2(), r2) == true ||
            pointIn(r1.getX2(), r1.getY2(), r2) == true)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool collidesRect(Rect r1, Rect r2)
    {
        if (rectInRect(r1, r2) == true)
        {
            return true;
        }
        else if (rectInRect(r2, r1) == true)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /*-------------------------------------------------------*/

    /*
    Rect convertToRect(Circle circle)
    {
        return nullptr;
    }

    Rect convertToRect(Triangle triangle)
    {
        return nullptr;
    }
    */

    /*-------------------------------------------------------*/

    virtual void print() = 0;
    virtual void simulate() = 0;
    virtual void erase() = 0;

public:
    virtual void play() = 0;
    virtual void result() = 0;
};
