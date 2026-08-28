#include <chrono>
#include <thread>
#include <iostream>

void worker()
{
    for(int i = 0; i < 5; ++i)
    {
        std::cout << "Worker: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

int main()
{
    std::thread t(worker);

    for(int i = 0; i < 5; ++i)
    {
        std::cout << "Main:   " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    t.join();

    return 0;
}