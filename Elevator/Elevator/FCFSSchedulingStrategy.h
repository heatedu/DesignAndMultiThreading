#pragma once
#include "SchedulingStrategy.h"

// First-Come-First-Served Scheduling Strategy
class FCFSSchedulingStrategy : public SchedulingStrategy {
public:
    int getNextStop(Elevator* elevator) override;
};

