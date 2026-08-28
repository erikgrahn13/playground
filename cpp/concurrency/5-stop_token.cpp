#include <chrono>
#include <iostream>
#include <stop_token>
#include <thread>

using namespace std::chrono_literals;

void worker(std::stop_token token)
{
    int iteration = 0;

    while(!token.stop_requested())
    {
        std::cout << "Iteration " << iteration++ << std::endl;
        std::this_thread::sleep_for(200ms);
    }

    std::cout << "Worker cleaner up and stopped" << std::endl;
}

int main()
{
    std::jthread thread(worker);

    std::this_thread::sleep_for(1s);

    std::cout << "Requesting stop..." << std::endl;
    thread.request_stop();

    // Explicitly joining makes the ordering clear.
    thread.join();

    std::cout << "Worker has finished" << std::endl;
}