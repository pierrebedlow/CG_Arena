#pragma once

#include <iostream>

class State
{
public:
    int m_Turns;
    int m_MeScore;

    State();
    ~State();


};

std::ostream& operator<<(std::ostream&, const State&);

class GameAPI
{
public:
    int Game;
    

};

