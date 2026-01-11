#pragma once
#include <queue>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class WorkerPool {
    std::queue<std::function<void()> > jobs;
    std::vector<std::thread> workers;
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> stop;

    void workerThread() {
        while (!stop) {
            std::function<void()> job;
            std::unique_lock<std::mutex> lock(mtx);
            while (jobs.empty() && !stop)
                cv.wait(lock);

            if (stop) return;

            job = jobs.front();
            jobs.pop();
            lock.unlock();

            job();
        }
    }

public:
    WorkerPool(int n = 2) : stop(false) {
        for (int i = 0; i < n; ++i)
            workers.emplace_back(&WorkerPool::workerThread, this);
    }

    void submit(std::function<void()> job) {
        std::lock_guard<std::mutex> lock(mtx);
        jobs.push(job);
        cv.notify_one();
    }

    void shutdown() {
        stop = true;
        cv.notify_all();
        for (size_t i = 0; i < workers.size(); ++i)
            workers[i].join();
    }
};
