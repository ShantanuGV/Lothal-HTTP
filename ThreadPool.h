#pragma once

#include <vector>
#include <thread>
#include <functional>
#include <atomic>

#include "ThreadSafeQueue.h"

class ThreadPool
{
public:

    using Task = std::function<void()>;

    explicit ThreadPool(size_t threadCount);

    ~ThreadPool();

    void submit(Task task);

    void shutdown();

    size_t workerCount() const;

private:

    ThreadSafeQueue<Task> taskQueue;

    std::vector<std::thread> workers;

    std::atomic<bool> running;

    void workerLoop();
};