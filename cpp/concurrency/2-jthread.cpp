#include <chrono>
#include <iostream>
#include <thread>

void worker()
{
    for(int i = 0; i < 5; ++i)
    {
        std::cout << "Worker: " << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

int main()
{
    std::jthread workerThread(worker);

    std::cout << "Main continues..." << std::endl;

    return 0;
}