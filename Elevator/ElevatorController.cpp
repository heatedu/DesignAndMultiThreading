#include "ElevatorController.h"
#include "ScanSchedulingStrategy.h"
#include <iostream>

ElevatorController::ElevatorController(int numberOfElevators, int numberOfFloors) 
    : currentElevatorId(1) {
    
    // Initialize elevators with unique IDs
    for (int i = 1; i <= numberOfElevators; i++) {
        elevators.push_back(new Elevator(i));
    }
    
    // Initialize floors
    for (int i = 1; i <= numberOfFloors; i++) {
        floors.push_back(new Floor(i));
    }
    
    // Default strategy
    schedulingStrategy = std::make_unique<ScanSchedulingStrategy>();
}

ElevatorController::~ElevatorController() {
    for (auto elevator : elevators) {
        delete elevator;
    }
    for (auto floor : floors) {
        delete floor;
    }
}

void ElevatorController::setSchedulingStrategy(std::unique_ptr<SchedulingStrategy> strategy) {
    schedulingStrategy = std::move(strategy);
}

void ElevatorController::requestElevator(int elevatorId, int floorNumber, Direction direction) {
    std::cout << "External request: Floor " << floorNumber << ", Direction " 
              << (direction == Direction::UP ? "UP" : "DOWN") << std::endl;
    
    Elevator* selectedElevator = getElevatorById(elevatorId);
    if (selectedElevator != nullptr) {
        ElevatorRequest request(elevatorId, floorNumber, false, direction, this);
        selectedElevator->addRequest(request);
        std::cout << "Assigned elevator " << selectedElevator->getId() 
                  << " to floor " << floorNumber << std::endl;
    } else {
        std::cout << "No elevator available for floor " << floorNumber << std::endl;
    }
}

void ElevatorController::requestFloor(int elevatorId, int floorNumber) {
    Elevator* elevator = getElevatorById(elevatorId);
    if (elevator == nullptr) {
        return;
    }
    
    std::cout << "Internal request: Elevator " << elevator->getId() 
              << " to floor " << floorNumber << std::endl;
    
    Direction direction = floorNumber > elevator->getCurrentFloor() ? Direction::UP : Direction::DOWN;
    ElevatorRequest request(elevatorId, floorNumber, true, direction, this);
    elevator->addRequest(request);
}

Elevator* ElevatorController::getElevatorById(int elevatorId) {
    for (auto elevator : elevators) {
        if (elevator->getId() == elevatorId) {
            return elevator;
        }
    }
    return nullptr;
}

void ElevatorController::step() {
    // Process each elevator - one floor movement per step
    for (auto elevator : elevators) 
    {
        if (!elevator->getRequestsQueue().empty()) 
        {
            int nextStop = schedulingStrategy->getNextStop(elevator);
            
            // ✅ Move one floor per step (more realistic simulation)
            elevator->moveOneFloor(nextStop);
        }
    }
}

