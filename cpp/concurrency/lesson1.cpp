#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

void worker(int id)
{
    std::cout << "Thread id: " << id << std::endl;
    std::this_thread::sleep_for(1000ms);
    std::cout << "Worker" << id << " is finished" << std::endl;
}

int main()
{
    {
        std::jthread thread1(worker, 1);
        std::jthread thread2(worker, 2);
        std::jthread thread3(worker, 3);
    }

    std::cout << "All workers finished" << std::endl;

    return 0;
}