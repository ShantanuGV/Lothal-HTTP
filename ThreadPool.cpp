#include "ThreadPool.h"

using namespace std;

ThreadPool::ThreadPool(size_t threadCount)
    : running(true)
{
    for(size_t i = 0; i < threadCount; i++)
    {
        workers.emplace_back(
            &ThreadPool::workerLoop,
            this
        );
    }
}

ThreadPool::~ThreadPool()
{
    shutdown();
}

void ThreadPool::submit(Task task)
{
    if(!running)
    {
        return;
    }

    taskQueue.push(std::move(task));
}

void ThreadPool::shutdown()
{
    if(!running.exchange(false))
    {
        return;
    }

    taskQueue.stop();

    for(thread& worker : workers)
    {
        if(worker.joinable())
        {
            worker.join();
        }
    }
}

size_t ThreadPool::workerCount() const
{
    return workers.size();
}

void ThreadPool::workerLoop()
{
    Task task;

    while(taskQueue.pop(task))
    {
        try
        {
            task();
        }
        catch(...)
        {
            // Prevent worker thread from terminating
            // Future: log exception
        }
    }
}