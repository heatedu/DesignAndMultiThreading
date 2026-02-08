#include "ElevatorDisplay.h"
#include "Elevator.h"

void ElevatorDisplay::onElevatorStateChange(Elevator* elevator, ElevatorState state) {
    std::cout << "Elevator " << elevator->getId() 
              << " state changed to " << stateToString(state) << std::endl;
}

void ElevatorDisplay::onElevatorFloorChange(Elevator* elevator, int floor) {
    std::cout << "Elevator " << elevator->getId() 
              << " moved to floor " << floor << std::endl;
}

std::string ElevatorDisplay::stateToString(ElevatorState state) {
    switch (state) {
        case ElevatorState::IDLE: return "IDLE";
        case ElevatorState::MOVING: return "MOVING";
        case ElevatorState::STOPPED: return "STOPPED";
        case ElevatorState::MAINTENANCE: return "MAINTENANCE";
        default: return "UNKNOWN";
    }
}

