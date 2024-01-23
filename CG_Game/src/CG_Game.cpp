#include "CG_Game.hpp"



std::ostream& operator<<(std::ostream& out, const State& s)
{
   return out << s.m_Turns << " / " << s.m_MeScore;
}

State::State()
    : m_Turns(0), m_MeScore(0)
{
}

State::~State()
{
}
