#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H

#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "Task.h"
#include "TaskComparator.h"

class TaskScheduler {
private:
    std::priority_queue<Task, std::vector<Task>, TaskComparator> pq;
    std::mutex mtx;
    std::condition_variable cv;
    std::vector<std::thread> workers;
    std::atomic<bool> stop;

    void workerThread();

public:
    TaskScheduler(int threads);
    void schedule(int priority, int delaySec, void (*func)());
    void shutdown();
};

#endif
