#pragma once
#include "Task.h"

class TaskCompare {
public:
    bool operator()(const Task* a, const Task* b) {
        return a->nextRun > b->nextRun;   // Min Heap
    }
};
