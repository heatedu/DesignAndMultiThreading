#pragma once
#include <functional>
#include <chrono>

class Task {
public:
    Task(std::function<void()> f,
        std::chrono::milliseconds inter,
        std::chrono::steady_clock::time_point next)
        : func(f), interval(inter), nextRun(next) {
    }

    std::function<void()> func;
    std::chrono::milliseconds interval;
    std::chrono::steady_clock::time_point nextRun;
};
