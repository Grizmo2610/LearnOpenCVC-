#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <future>
#include <vector>

std::atomic<bool> stopFlag(false);

int printNumbers(int threadID) {
    std::vector<int> results;
    for (int i = 0; i < 10; ++i) {
        if (stopFlag) {
            std::cout << "Thread " << threadID << " have stopped." << std::endl;
            return -1;
        }
        std::cout << "Thread " << threadID << ": " << i << std::endl;
        results.push_back(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return results.size();
}

int main() {
    std::promise<int> promise1;
    std::promise<int> promise2;
    std::future<int> future1 = promise1.get_future();
    std::future<int> future2 = promise2.get_future();

    std::thread thread1([&]() {
        int result = printNumbers(1);
        promise1.set_value(result);
    });

    std::thread thread2([&]() {
        int result = printNumbers(2);
        promise2.set_value(result);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    stopFlag = true;

    thread1.join();
    thread2.join();

    int result1 = future1.get();
    int result2 = future2.get();

    std::cout << "Thread 1: " << result1 << std::endl;
    std::cout << "Thread 2: " << result2 << std::endl;
    std::cout << "All finished." << std::endl;

    return 0;
}
