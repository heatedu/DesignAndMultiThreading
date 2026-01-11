#include "SimpleThreadPool.h"

SimpleThreadPool::SimpleThreadPool(size_t threadCount) : stop(false)
{
    for (size_t i = 0; i < threadCount; ++i) {
        /*
            This tells std::thread:
                Call workerThread
                On this object
                Internally, it becomes:
                (this->*workerThread)();

            Can we ever write it without this?
                ✅ YES — but only if the function is static
                Example:
                    class SimpleThreadPool {
                    public:
                        static void workerThreadStatic(SimpleThreadPool* pool);
                    };
                    
                    workers.emplace_back(&SimpleThreadPool::workerThreadStatic, this);
        */
        workers.emplace_back(&SimpleThreadPool::workerThread, this);
    }
}

void SimpleThreadPool::workerThread()
{
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(mtx);
            while (stop==false && tasks.empty()) {
                cv.wait(lock);
            }

            if (stop && tasks.empty())
                return;

            task = tasks.front();
            tasks.pop();
        }

        task();
    }
}

void SimpleThreadPool::addTask(std::function<void()> task)
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        tasks.push(task);
    }
    cv.notify_one();
}

SimpleThreadPool::~SimpleThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        stop = true;
    }
    cv.notify_all();

    for (auto &worker : workers)
        worker.join();
}
