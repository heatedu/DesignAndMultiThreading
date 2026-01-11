#include "TaskScheduler.h"
#include <thread>
#include <chrono>
#include <iostream>

void taskA() { std::cout << "Task A executed\n"; }
void taskB() { std::cout << "Task B executed\n"; }
void taskC() { std::cout << "Task C executed\n"; }

int main() {
    TaskScheduler scheduler(3);

    scheduler.schedule(2, 0, taskA);
    scheduler.schedule(1, 3, taskB);
    scheduler.schedule(0, 1, taskC);

    std::this_thread::sleep_for(std::chrono::seconds(6));
    scheduler.shutdown();
    return 0;
}
