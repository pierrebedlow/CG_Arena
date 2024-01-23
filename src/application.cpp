#include "application.hpp"
#include "../../CG_Game/src/CG_Game.hpp"

#include <iostream>

Application::Application()
    : m_Running(true), m_UserAction(0)
{
}

bool Application::run()
{
    State state;

    while (m_Running) {

        std::cout << "What do you want to do?" << std::endl;
        std::cout << "  0 - Exit" << std::endl;
        std::cout << "  1 - Display State" << std::endl;
        std::cin >> m_UserAction; 

        switch(m_UserAction)
        {
            case 0:
                m_Running = false;
                break;
            case 1:
                std::cout << "Cuurent State\n" << state << std::endl;
                break;
            default:
                break;

        }



    }

    return false;
}
