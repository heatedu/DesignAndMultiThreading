#include "Building.h"

Building::Building(const std::string& name, int numberOfFloors, int numberOfElevators)
    : name(name), numberOfFloors(numberOfFloors) {
    elevatorController = new ElevatorController(numberOfElevators, numberOfFloors);
}

Building::~Building() {
    delete elevatorController;
}

