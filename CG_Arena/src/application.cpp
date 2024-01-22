#include "application.hpp"

#include <iostream>

Application::Application()
    : m_Running(true), m_UserAction(0)
{
}

bool Application::run()
{

    while (m_Running) {

        std::cout << "What do you want to do?" << std::endl;
        std::cout << "  0 - Exit" << std::endl;
        std::cin >> m_UserAction; 

        switch(m_UserAction)
        {
            case 0:
                m_Running = false;
                break;
            default:
                break;

        }



    }

    return false;
}
