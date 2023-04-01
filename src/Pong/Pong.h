#pragma once
#include <vector>
#include "Bat.h"
#include "Game.h"
#include "General/Simulatable.h"

using namespace std;

class Pong
{
private:
    vector<Displayable *> entities;

public:
    Pong()
    {
        entities.push_back(new Bat(50, 50, 0, 0));
        entities.push_back(new Bat(100, 50, 0, 0));
    }

    ~Pong()
    {
        for (Displayable *entity : this->entities)
        {
            delete entity;
        }
    }

    void core()
    {
    }
};
