#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <vector>
#include <thread>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>

class ThreadPool {
public:
    explicit ThreadPool(size_t num_workers, size_t queue_capacity = 1024);
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    // Submit a task; blocks if full, returns false if shutting down
    bool submit(std::function<void()> task);

    // Submit with timeout
    bool submit_for(std::function<void()> task, std::chrono::milliseconds timeout);

    // Graceful shutdown
    void shutdown();

private:
    void worker_loop();

    std::vector<std::thread> workers_;
    std::deque<std::function<void()>> queue_;
    const size_t capacity_;

    std::mutex mtx_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;

    std::atomic<bool> stopping_;
};

#endif // THREADPOOL_H
