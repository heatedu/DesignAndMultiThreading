#pragma once
#include "CommonEnums.h"
#include "ElevatorObserver.h"
#include "ElevatorRequest.h"
#include <vector>
#include <queue>

// Core Elevator class with simple queue for managing requests
class Elevator {
private:
    int id;                                      // Unique ID for the elevator
    int currentFloor;                            // Current floor where the elevator is located
    Direction direction;                         // Current direction (UP, DOWN, or IDLE)
    ElevatorState state;                         // Current operational state
    std::vector<ElevatorObserver*> observers;    // List of observers
    std::queue<ElevatorRequest> requests;        // Queue to manage requests

    // Notify methods
    void notifyStateChange(ElevatorState state);
    void notifyFloorChange(int floor);
    void completeArrival();

public:
    Elevator(int id);
    
    // Observer management
    void addObserver(ElevatorObserver* observer);
    void removeObserver(ElevatorObserver* observer);
    
    // State management
    void setState(ElevatorState newState);
    void setDirection(Direction newDirection);
    
    // Request management
    void addRequest(const ElevatorRequest& elevatorRequest);
    void moveToNextStop(int nextStop);  // Old: moves all the way
    void moveOneFloor(int targetFloor); // New: moves one floor per call
    
    // Getters
    int getId() const { return id; }
    int getCurrentFloor() const { return currentFloor; }
    Direction getDirection() const { return direction; }
    ElevatorState getState() const { return state; }
    std::queue<ElevatorRequest> getRequestsQueue() const { return requests; }
    std::vector<int> getDestinationFloors() const;
};

