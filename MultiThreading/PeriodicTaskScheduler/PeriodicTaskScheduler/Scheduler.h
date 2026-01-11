#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include "Task.h"
#include "TaskCompare.h"
#include "WorkerPool.h"

class Scheduler {
    std::priority_queue<Task*, std::vector<Task*>, TaskCompare> heap;
    std::mutex mtx;
    std::condition_variable cv;
    std::thread timerThread;
    std::atomic<bool> stop;
    WorkerPool pool;

    void run() {
        while (!stop) {
            std::unique_lock<std::mutex> lock(mtx);

            while (heap.empty() && !stop)
                cv.wait(lock);

            if (stop) break;

            Task* task = heap.top();
            auto now = std::chrono::steady_clock::now();

            if (now < task->nextRun) {
                cv.wait_until(lock, task->nextRun);
                continue;
            }

            heap.pop();
            lock.unlock();

            pool.submit(task->func);

            task->nextRun += task->interval;

            lock.lock();
            heap.push(task);
            lock.unlock();
        }
    }

public:
    Scheduler() : stop(false), pool(3) {
        timerThread = std::thread(&Scheduler::run, this);
    }

    void schedule(std::function<void()> f, std::chrono::milliseconds interval) {
        auto now = std::chrono::steady_clock::now();
        Task* t = new Task(f, interval, now + interval);

        std::lock_guard<std::mutex> lock(mtx);
        heap.push(t);
        cv.notify_all();
    }

    void shutdown() {
        stop = true;
        cv.notify_all();
        timerThread.join();
        pool.shutdown();
    }
};
