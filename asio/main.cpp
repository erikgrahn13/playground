#include <iostream>
#include "asio.h"
#include "asiodrivers.h"

static AsioDrivers drivers;

int main() {

    std::cout << "Hello World!X" << std::endl;
    drivers.removeCurrentDriver();
    char *names[32];

    auto status = drivers.getDriverNames(names, 10);

    if(status == 0)
    {
        std::cout << "erik1" << std::endl;
    }
    else
    {
        std::cout << "erik2" << std::endl;
        for(int i = 0; i < 10; ++i)
        {
            std::cout << names[i] << std::endl;
        }
    }
    // auto test = asioDrivers->asioGetNumDev();

    // std::cout << test << std::endl;


    // if(loadAsioDriver())




    

    // std::cout << names << std::endl;


    // ASIODriverInfo erik;

    // auto hej = ASIOInit(&erik);

    // std::cout << hej << std::endl;
    // std::cout << erik.asioVersion << std::endl;
    // std::cout << erik.driverVersion << std::endl;
    // std::cout << erik.errorMessage << std::endl;
    // std::cout << erik.name << std::endl;

    return 0;
}
