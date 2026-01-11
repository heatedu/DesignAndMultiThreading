#include <iostream>
#include "Scheduler.h"

void taskA() {
    std::cout << "Task A running every 1 sec\n";
}

void taskB() {
    std::cout << "Task B running every 3 sec\n";
}

int main() {
    Scheduler scheduler; //after this line there will be n worker threads running and 1 timer thread(i.e. scheduler thread) running. Scheduler thread will pop from heap and submit to worker thread's queue where they would be executed after popping from queue.

    scheduler.schedule(taskA, std::chrono::milliseconds(1000));
    scheduler.schedule(taskB, std::chrono::milliseconds(3000));

    std::this_thread::sleep_for(std::chrono::seconds(10));

    scheduler.shutdown();
    return 0;
}
