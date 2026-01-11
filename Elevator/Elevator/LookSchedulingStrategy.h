#pragma once
#include "SchedulingStrategy.h"

// LOOK Scheduling Strategy - optimized SCAN that only goes as far as the last request
class LookSchedulingStrategy : public SchedulingStrategy {
public:
    int getNextStop(Elevator* elevator) override;
};

