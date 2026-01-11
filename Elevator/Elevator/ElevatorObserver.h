#pragma once
#include "CommonEnums.h"

class Elevator; // Forward declaration

// Observer interface for handling elevator events (Observer Pattern)
class ElevatorObserver {
public:
    virtual ~ElevatorObserver() = default;
    
    // Called when an elevator's state changes
    virtual void onElevatorStateChange(Elevator* elevator, ElevatorState state) = 0;
    
    // Called when an elevator changes its current floor
    virtual void onElevatorFloorChange(Elevator* elevator, int floor) = 0;
};

