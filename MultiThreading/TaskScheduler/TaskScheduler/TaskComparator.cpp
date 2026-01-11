#include "TaskComparator.h"

bool TaskComparator::operator()(const Task& a, const Task& b) {
    if (a.execTime == b.execTime)
        return a.priority > b.priority;
    return a.execTime > b.execTime;
}
