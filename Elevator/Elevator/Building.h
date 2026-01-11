#pragma once
#include "ElevatorController.h"
#include <string>

// Represents a building with elevators and multiple floors
class Building {
private:
    std::string name;                        // Name of the building
    int numberOfFloors;                      // Total number of floors
    ElevatorController* elevatorController;  // Controller to manage elevators

public:
    Building(const std::string& name, int numberOfFloors, int numberOfElevators);
    ~Building();
    
    // Getters
    std::string getName() const { return name; }
    int getNumberOfFloors() const { return numberOfFloors; }
    ElevatorController* getElevatorController() const { return elevatorController; }
};

