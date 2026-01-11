#include "Elevator.h"
#include <algorithm>

Elevator::Elevator(int id) 
    : id(id), currentFloor(1), direction(Direction::IDLE), state(ElevatorState::IDLE) {
}

void Elevator::addObserver(ElevatorObserver* observer) {
    observers.push_back(observer);
}

void Elevator::removeObserver(ElevatorObserver* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Elevator::notifyStateChange(ElevatorState state) {
    for (auto observer : observers) {
        observer->onElevatorStateChange(this, state);
    }
}

void Elevator::notifyFloorChange(int floor) {
    for (auto observer : observers) {
        observer->onElevatorFloorChange(this, floor);
    }
}

void Elevator::setState(ElevatorState newState) {
    this->state = newState;
    notifyStateChange(newState);
}

void Elevator::setDirection(Direction newDirection) {
    this->direction = newDirection;
}

void Elevator::addRequest(const ElevatorRequest& elevatorRequest) {
    // Check if request already exists
    std::queue<ElevatorRequest> tempQueue = requests;
    bool exists = false;
    
    while (!tempQueue.empty()) 
    {
        if (tempQueue.front() == elevatorRequest) 
        {
            exists = true;
            break;
        }
        tempQueue.pop();
    }
    
    if (!exists) {
        requests.push(elevatorRequest);
    }
    
    int requestedFloor = elevatorRequest.getFloor();
    
    // If elevator is idle, determine direction and start moving
    if (state == ElevatorState::IDLE && !requests.empty()) {
        if (requestedFloor > currentFloor) {
            direction = Direction::UP;
        } else if (requestedFloor < currentFloor) {
            direction = Direction::DOWN;
        }
        setState(ElevatorState::MOVING);
    }
}

void Elevator::moveToNextStop(int nextStop) {
    if (state != ElevatorState::MOVING) {
        return;
    }
    
    while (currentFloor != nextStop) {
        // Update floor based on direction
        if (direction == Direction::UP) {
            currentFloor++;
        } else if (direction == Direction::DOWN) {
            currentFloor--;
        }
        
        // Notify observers about the floor change
        notifyFloorChange(currentFloor);
        
        // Complete arrival once the target floor is reached
        if (currentFloor == nextStop) {
            completeArrival();
            return;
        }
    }
}

void Elevator::moveOneFloor(int targetFloor) {
    // If currently idle, start moving
    if (state == ElevatorState::IDLE) {
        setState(ElevatorState::MOVING);
    }
    
    // Already at target floor
    if (currentFloor == targetFloor) {
        completeArrival();
        return;
    }
    
    // Move one floor in the appropriate direction
    if (targetFloor > currentFloor) {
        currentFloor++;
        direction = Direction::UP;
    } else if (targetFloor < currentFloor) {
        currentFloor--;
        direction = Direction::DOWN;
    }
    
    // Notify observers about the floor change
    notifyFloorChange(currentFloor);
    
    // Check if reached the target floor
    if (currentFloor == targetFloor) {
        completeArrival();
    }
}

void Elevator::completeArrival() {
    // Stop the elevator and notify observers
    setState(ElevatorState::STOPPED);
    
    // Remove the current floor from the requests queue
    std::queue<ElevatorRequest> tempQueue;
    while (!requests.empty()) {
        if (requests.front().getFloor() != currentFloor) {
            tempQueue.push(requests.front());
        }
        requests.pop();
    }
    requests = tempQueue;
    
    // If no more requests, set state to IDLE
    if (requests.empty()) {
        direction = Direction::IDLE;
        setState(ElevatorState::IDLE);
    } else {
        // Otherwise, continue moving after a brief stop
        setState(ElevatorState::MOVING);
    }
}

std::vector<int> Elevator::getDestinationFloors() const {
    std::vector<int> destinations;
    std::queue<ElevatorRequest> tempQueue = requests;
    
    while (!tempQueue.empty()) {
        destinations.push_back(tempQueue.front().getFloor());
        tempQueue.pop();
    }
    
    return destinations;
}

