#include <iostream>
#include <mutex>
#include <thread>

int counter = 0;
std::mutex counterMutex;

void increment()
{
    for(int i = 0; i < 100000; ++i)
    {
        std::lock_guard<std::mutex> lock(counterMutex);
        ++counter;
    }
}

int main()
{
    {
        std::jthread t1(increment);
        std::jthread t2(increment);
    }
    std::cout << counter << std::endl;

    return 0;
}