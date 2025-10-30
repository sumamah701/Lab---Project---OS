#include "threadpool.h"
#include <iostream>
#include <atomic>
#include <vector>
#include <chrono>
#include <thread>

int main() {
    constexpr size_t NUM_WORKERS = 4;
    constexpr size_t QUEUE_CAP = 64;
    ThreadPool pool(NUM_WORKERS, QUEUE_CAP);

    std::atomic<int> counter{0};
    const int num_tasks = 1000;

    std::vector<std::thread> clients;
    for (int c = 0; c < 4; ++c) {
        clients.emplace_back([&pool, &counter, num_tasks, c](){
            for (int i = 0; i < num_tasks / 4; ++i) {
                bool ok = pool.submit_for([&counter, c, i](){
                    counter.fetch_add(1);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }, std::chrono::milliseconds(200));
                if (!ok) {
                    std::cerr << "Client " << c << " timed out submitting task " << i << "\\n";
                }
            }
        });
    }

    for (auto &t : clients) if (t.joinable()) t.join();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    pool.shutdown();

    std::cout << "All tasks completed. Counter = " << counter.load() << std::endl;
    return 0;
}
