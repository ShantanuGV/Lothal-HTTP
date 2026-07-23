#pragma once

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <utility>
using namespace std;

template<typename T>
class ThreadSafeQueue{
    public:
        ThreadSafeQueue();

        void push(const T &value);

        void push(T &&value);

        bool pop(T &value);

        void stop();

        bool empty() const;

        size_t size() const;

    private:
        queue<T> queue;

        mutable mutex mutex;

        condition_variable condition;

        bool stopped;
};

// =============================
// Implementation
// =============================

template<typename T>
ThreadSafeQueue<T>::ThreadSafeQueue()
    : stopped(false)
{
}

template<typename T>
void ThreadSafeQueue<T>::push(const T& value)
{
    {
        std::lock_guard<std::mutex> lock(mutex);

        if(stopped)
            return;

        queue.push(value);
    }

    condition.notify_one();
}

template<typename T>
void ThreadSafeQueue<T>::push(T&& value)
{
    {
        std::lock_guard<std::mutex> lock(mutex);

        if(stopped)
            return;

        queue.push(std::move(value));
    }

    condition.notify_one();
}

template<typename T>
bool ThreadSafeQueue<T>::pop(T& value)
{
    std::unique_lock<std::mutex> lock(mutex);

    condition.wait(lock,
    [this]()
    {
        return stopped || !queue.empty();
    });

    if(stopped && queue.empty())
    {
        return false;
    }

    value = std::move(queue.front());

    queue.pop();

    return true;
}

template<typename T>
void ThreadSafeQueue<T>::stop()
{
    {
        std::lock_guard<std::mutex> lock(mutex);

        stopped = true;
    }

    condition.notify_all();
}

template<typename T>
bool ThreadSafeQueue<T>::empty() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return queue.empty();
}

template<typename T>
size_t ThreadSafeQueue<T>::size() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return queue.size();
}
