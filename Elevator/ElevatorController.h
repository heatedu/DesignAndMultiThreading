#pragma once
#include "Elevator.h"
#include "Floor.h"
#include "SchedulingStrategy.h"
#include <vector>
#include <memory>

// Elevator Controller class to manage elevators and floor requests
class ElevatorController {
private:
    std::vector<Elevator*> elevators;                    // List of all elevators
    std::vector<Floor*> floors;                          // List of all floors
    std::unique_ptr<SchedulingStrategy> schedulingStrategy; // Strategy for scheduling
    int currentElevatorId;                               // ID of current elevator

    Elevator* getElevatorById(int elevatorId);

public:
    ElevatorController(int numberOfElevators, int numberOfFloors);
    ~ElevatorController();
    
    // Strategy management
    void setSchedulingStrategy(std::unique_ptr<SchedulingStrategy> strategy);
    
    // Request handling
    void requestElevator(int elevatorId, int floorNumber, Direction direction);
    void requestFloor(int elevatorId, int floorNumber);
    
    // Simulation
    void step();
    
    // Getters
    std::vector<Elevator*> getElevators() const { return elevators; }
    std::vector<Floor*> getFloors() const { return floors; }
    
    // Setter for current elevator
    void setCurrentElevator(int elevatorId) { currentElevatorId = elevatorId; }
};

