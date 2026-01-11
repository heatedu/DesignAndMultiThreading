#pragma once
#include "ElevatorObserver.h"
#include <iostream>
#include <string>

// Concrete implementation of the Observer interface
class ElevatorDisplay : public ElevatorObserver {
public:
    void onElevatorStateChange(Elevator* elevator, ElevatorState state) override;
    void onElevatorFloorChange(Elevator* elevator, int floor) override;

private:
    std::string stateToString(ElevatorState state);
};

