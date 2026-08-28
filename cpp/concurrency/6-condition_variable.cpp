#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex mutex;
std::condition_variable cv;

bool dataReady = false;

void producer()
{
    {
        std::lock_guard lock(mutex);
        dataReady = true;
    }

    cv.notify_one();
}

void consumer()
{
    std::unique_lock lock(mutex);

    cv.wait(lock, [] {
        return dataReady;
    });

    std::cout << "Received data!" << std::endl;
}

int main()
{
    std::jthread consumerThread(consumer);
    std::jthread producerThread(producer);

    return 0;
}