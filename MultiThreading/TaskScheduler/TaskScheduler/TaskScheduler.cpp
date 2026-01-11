#include "TaskScheduler.h"
#include <iostream>

TaskScheduler::TaskScheduler(int threads) : stop(false) {
    for (int i = 0; i < threads; i++)
        workers.emplace_back(&TaskScheduler::workerThread, this);
}

void TaskScheduler::workerThread() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);

        while (!stop && pq.empty())
            cv.wait(lock);

        if (stop && pq.empty())
            return;

        Task task = pq.top();
        auto now = std::chrono::steady_clock::now();

        if (task.execTime > now) {
            cv.wait_until(lock, task.execTime);
            continue;
        }

        pq.pop();
        lock.unlock();

        task.func();
    }
}

void TaskScheduler::schedule(int priority, int delaySec, void (*func)()) {
    std::lock_guard<std::mutex> lock(mtx);
    pq.emplace(priority, delaySec, func);
    cv.notify_all();
}

void TaskScheduler::shutdown() {
    stop = true;
    cv.notify_all();
    for (auto& t : workers)
        t.join();
}
