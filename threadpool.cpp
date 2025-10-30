#include "threadpool.h"
#include <iostream>

ThreadPool::ThreadPool(size_t num_workers, size_t queue_capacity)
    : capacity_(queue_capacity), stopping_(false)
{
    for (size_t i = 0; i < num_workers; ++i) {
        workers_.emplace_back([this]{ worker_loop(); });
    }
}

ThreadPool::~ThreadPool()
{
    shutdown();
    for (auto &t : workers_) {
        if (t.joinable()) t.join();
    }
}

bool ThreadPool::submit(std::function<void()> task)
{
    std::unique_lock<std::mutex> lk(mtx_);
    not_full_.wait(lk, [this]{ return queue_.size() < capacity_ || stopping_.load(); });
    if (stopping_.load()) return false;
    queue_.push_back(std::move(task));
    lk.unlock();
    not_empty_.notify_one();
    return true;
}

bool ThreadPool::submit_for(std::function<void()> task, std::chrono::milliseconds timeout)
{
    std::unique_lock<std::mutex> lk(mtx_);
    if (!not_full_.wait_for(lk, timeout, [this]{ return queue_.size() < capacity_ || stopping_.load(); }))
        return false;
    if (stopping_.load()) return false;
    queue_.push_back(std::move(task));
    lk.unlock();
    not_empty_.notify_one();
    return true;
}

void ThreadPool::shutdown()
{
    bool expected = false;
    if (!stopping_.compare_exchange_strong(expected, true))
        return;

    {
        std::lock_guard<std::mutex> lk(mtx_);
    }

    not_full_.notify_all();
    not_empty_.notify_all();
}

void ThreadPool::worker_loop()
{
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lk(mtx_);
            not_empty_.wait(lk, [this]{ return !queue_.empty() || stopping_.load(); });
            if (queue_.empty() && stopping_.load())
                return;
            if (!queue_.empty()) {
                task = std::move(queue_.front());
                queue_.pop_front();
            }
        }
        not_full_.notify_one();
        if (task) {
            try {
                task();
            } catch (const std::exception &e) {
                std::cerr << "[ThreadPool] Task threw exception: " << e.what() << "\n";
            } catch (...) {
                std::cerr << "[ThreadPool] Task threw unknown exception\n";
            }
        }
    }
}
