#ifndef TASK_H
#define TASK_H

#include <chrono>

class Task {
public:
    int priority;
    std::chrono::steady_clock::time_point execTime;
    void (*func)();

    Task(int p, int delaySec, void (*f)());
};

#endif
