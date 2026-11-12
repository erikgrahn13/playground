#include <chrono>
#include <iostream>
#include <thread>

void analyzeFile(int fileId, int durationMs)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(durationMs));
    std::cout << "Worker" << fileId << " sleep for " << durationMs << "ms" << std::endl; 
}

int main()
{
    const auto start = std::chrono::steady_clock::now();

    std::jthread worker1(analyzeFile, 1, 500);
    std::jthread worker2(analyzeFile, 2, 1000);
    std::jthread worker3(analyzeFile, 3, 1500);

    worker1.join();
    worker2.join();
    worker3.join();

    const auto end = std::chrono::steady_clock::now();

    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Total time: " << elapsed.count() << " ms" << std::endl;

    return 0;
}