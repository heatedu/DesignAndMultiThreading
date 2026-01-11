#include "Task.h"

Task::Task(int p, int delaySec, void (*f)())
    : priority(p), func(f) {
    execTime = std::chrono::steady_clock::now() + std::chrono::seconds(delaySec);
}
