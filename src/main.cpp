

#include <iostream>

#include "app_manager.hpp"

int main()
{
    std::cout << "Program starting..." << std::endl;

    AppManager envelope_generator;
    envelope_generator.run();    

    std::cout << "Shutting down. Beep boop..." << std::endl;

    return 0;
}