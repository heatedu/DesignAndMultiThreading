#pragma once
#include "SchedulingStrategy.h"

// SCAN Scheduling Strategy - elevator continues in one direction until no more requests
class ScanSchedulingStrategy : public SchedulingStrategy {
public:
    int getNextStop(Elevator* elevator) override;

private:
    int switchDirection(Elevator* elevator, int fallbackFloor);
};

