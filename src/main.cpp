#include "core/core.h"

#include <iostream>

int main(int argc, char * argv[])
{
    try
    {
        evnt::Core::startUp();

        evnt::Core::instance().appCreate<evnt::App>();
        if(!evnt::Core::instance().appInit(argc, argv))
        {
            std::cout << "Errror! Application couldn't be initialized" << std::endl;
            return 1;
        }

        // evnt::Core::instance().enterMainLoop();

        evnt::Core::shutDown();
    }
    catch(const std::exception & ex)
    {
        std::cout << "ERROR: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
