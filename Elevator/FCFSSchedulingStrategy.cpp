#include "FCFSSchedulingStrategy.h"
#include "Elevator.h"
#include <queue>

int FCFSSchedulingStrategy::getNextStop(Elevator* elevator) {
    Direction elevatorDirection = elevator->getDirection();
    int currentFloor = elevator->getCurrentFloor();
    
    std::queue<ElevatorRequest> requestQueue = elevator->getRequestsQueue();
    
    // If the request queue is empty, stay on the current floor
    if (requestQueue.empty()) {
        return currentFloor;
    }
    
    // Fetch the next requested floor
    ElevatorRequest nextRequest = requestQueue.front();
    int nextRequestedFloor = nextRequest.getFloor();
    
    // If the next floor is the current floor, return it
    if (nextRequestedFloor == currentFloor) {
        return currentFloor;
    }
    
    // Set elevator's direction based on its current state and next floor
    if (elevatorDirection == Direction::IDLE) {
        elevator->setDirection(nextRequestedFloor > currentFloor ? Direction::UP : Direction::DOWN);
    } else if (elevatorDirection == Direction::UP && nextRequestedFloor < currentFloor) {
        elevator->setDirection(Direction::DOWN);
    } else if (elevatorDirection == Direction::DOWN && nextRequestedFloor > currentFloor) {
        elevator->setDirection(Direction::UP);
    }
    
    return nextRequestedFloor;
}

