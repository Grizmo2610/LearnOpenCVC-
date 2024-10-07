// #include <iostream>
// #include <thread>
// #include <vector>
// #include <atomic>
// #include <chrono>
// #include <future>
//
// using namespace std;
//
// std::atomic<bool> stopFlag(false);
//
// bool primeCheck(int threadID, long long number, long long from, long long to) {
//     if (from % 2 == 0) {
//         from++;
//     }
//
//     if (from == to) {
//         return number % from == 0;
//     }
//
//     for (int i = from; i < to; i+=2){
//         if (stopFlag) {
//             return false;
//         }
//         if (number % i == 0) {
//             stopFlag = true;
//             return false;
//         }
//     }
//     return true;
// }
//
// bool isPrime(long long number) {
//     if (number <= 1) {
//         return false;
//     }
//     if (number == 2) {
//         return true;
//     }
//     if (number % 2 == 0) {
//         return false;
//     }
//     const int MAXIMUM_THREAD = 5;
//     long long limit = static_cast<int>(sqrt(number)) + 1;
//     long long range = limit / MAXIMUM_THREAD + 1;
//     int index = 0;
//     vector<thread> threads;
//
//     for (int i = 3; i <= limit; i += range) {
//         threads.emplace_back(primeCheck, index++, number, i, i + range);
//     }
//
//     for (auto& thread : threads) {
//         thread.join();
//     }
//
//     return !stopFlag;
// }
//
// bool primeCheck(long long number) {
//     if (number <= 1) {
//         return false;
//     } else if (number == 2) {
//         return true;
//     } else if (number % 2 == 0) {
//         return false;
//     }
//     for (int i = 3; i <= static_cast<int>(sqrt(number)); i += 2) {
//         if (number % i == 0) {
//             return false;
//         }
//     }
//     return true;
// }
//
//
// void prime() {
//     long long number = 18399999934455433421LL;
//
//     auto start = std::chrono::high_resolution_clock::now();
//     cout << isPrime(number)<< endl;
//     auto end = std::chrono::high_resolution_clock::now();
//
//     std::chrono::duration<double> duration = end - start;
//     std::cout << "Runtime: " << duration.count() << " second." << std::endl;
//
//     start = std::chrono::high_resolution_clock::now();
//     cout << primeCheck(number)<< endl;
//     end = std::chrono::high_resolution_clock::now();
//
//     duration = end - start;
//     std::cout << "Runtime: " << duration.count() << " second." << std::endl;
// }
//
// int sum(int from, int to, int number) {
//     int result = 0;
//     if (to > number) {
//         to = number;
//     }
//     for (int i = from; i <= to; i++) {
//         result += i;
//     }
//     return result;
// }
//
// int sum(int number) {
//     int result = 0;
//     vector<thread> threads;
//     vector<future<int>> futures;
//     int range = 10;
//
//     for (int i = 1; i <= number; i += range) {
//         promise<int> promise;
//         futures.emplace_back(promise.get_future());
//         threads.push_back(thread([&]() {
//             int temp = sum(i, i + range - 1, number);
//             promise.set_value(temp);
//         }));
//     }
//
//     for (auto& thread : threads) {
//         thread.join();
//     }
//
//     for (auto& future : futures) {
//         result += future.get();
//     }
//     return result;
// }
//
// void sum() {
//     cout << sum(1000) << endl;
// }
//
// int main() {
//
//     sum();
//     return 0;
// }
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
