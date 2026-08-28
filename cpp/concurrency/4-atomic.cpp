#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> counter{0};

void increment()
{
    for(int i = 0; i < 100000; ++i)
    {
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