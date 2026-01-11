#ifndef TASK_COMPARATOR_H
#define TASK_COMPARATOR_H

#include "Task.h"

class TaskComparator {
public:
    bool operator()(const Task& a, const Task& b);
};

#endif
