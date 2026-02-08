#include "ScanSchedulingStrategy.h"
#include "Elevator.h"
#include <queue>
#include <vector>
#include <algorithm>
#include <cmath>

int ScanSchedulingStrategy::getNextStop(Elevator* elevator) {
    Direction elevatorDirection = elevator->getDirection();
    int currentFloor = elevator->getCurrentFloor();
    std::queue<ElevatorRequest> requests = elevator->getRequestsQueue();
    
    if (requests.empty()) {
        return currentFloor;
    }
    
    // Separate requests into up and down queues
    std::vector<int> upFloors;
    std::vector<int> downFloors;
    
    while (!requests.empty()) {
        ElevatorRequest req = requests.front();
        requests.pop();
        int floor = req.getFloor();
        
        if (floor > currentFloor) {
            upFloors.push_back(floor);
        } else if (floor < currentFloor) {
            downFloors.push_back(floor);
        } else {
            return currentFloor; // Already at requested floor
        }
    }
    
    // Sort the floors
    std::sort(upFloors.begin(), upFloors.end()); // Ascending for up
    std::sort(downFloors.begin(), downFloors.end(), std::greater<int>()); // Descending for down
    
    // Handle IDLE state
    if (elevatorDirection == Direction::IDLE) {
        if (upFloors.empty() && downFloors.empty()) {
            return currentFloor;
        } else if (upFloors.empty()) {
            elevator->setDirection(Direction::DOWN);
            return downFloors[0];
        } else if (downFloors.empty()) {
            elevator->setDirection(Direction::UP);
            return upFloors[0];
        } else {
            // Choose the closest request
            int nearestUp = upFloors[0];
            int nearestDown = downFloors[0];
            
            if (std::abs(nearestUp - currentFloor) < std::abs(nearestDown - currentFloor)) {
                elevator->setDirection(Direction::UP);
                return nearestUp;
            } else {
                elevator->setDirection(Direction::DOWN);
                return nearestDown;
            }
        }
    }
    
    // Handle UP direction
    if (elevatorDirection == Direction::UP) {
        if (!upFloors.empty()) {
            return upFloors[0];
        } else {
            return switchDirection(elevator, downFloors.empty() ? currentFloor : downFloors[0]);
        }
    }
    
    // Handle DOWN direction
    if (elevatorDirection == Direction::DOWN) {
        if (!downFloors.empty()) {
            return downFloors[0];
        } else {
            return switchDirection(elevator, upFloors.empty() ? currentFloor : upFloors[0]);
        }
    }
    
    return currentFloor;
}

int ScanSchedulingStrategy::switchDirection(Elevator* elevator, int fallbackFloor) {
    Direction currentDir = elevator->getDirection();
    elevator->setDirection(currentDir == Direction::UP ? Direction::DOWN : Direction::UP);
    return fallbackFloor;
}

