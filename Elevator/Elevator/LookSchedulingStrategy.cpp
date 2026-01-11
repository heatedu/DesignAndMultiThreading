#include "LookSchedulingStrategy.h"
#include "Elevator.h"
#include <queue>
#include <climits>

int LookSchedulingStrategy::getNextStop(Elevator* elevator) {
    int currentFloor = elevator->getCurrentFloor();
    std::queue<ElevatorRequest> requests = elevator->getRequestsQueue();
    
    if (requests.empty()) {
        return currentFloor;
    }
    
    // Determine the primary target from the first request
    ElevatorRequest primaryRequest = requests.front();
    int primaryFloor = primaryRequest.getFloor();
    
    // Determine travel direction
    Direction travelDirection;
    if (primaryFloor > currentFloor) {
        travelDirection = Direction::UP;
    } else if (primaryFloor < currentFloor) {
        travelDirection = Direction::DOWN;
    } else {
        return currentFloor; // Already at requested floor
    }
    
    // Look for any request along the journey
    int candidate = -1;
    
    while (!requests.empty()) {
        ElevatorRequest req = requests.front();
        requests.pop();
        int reqFloor = req.getFloor();
        
        // Check if request is within range between currentFloor and primaryFloor
        if (travelDirection == Direction::UP && reqFloor > currentFloor && reqFloor <= primaryFloor) {
            // For internal requests always consider; for external, only if going UP
            if (req.checkIsInternalRequest() || 
                (!req.checkIsInternalRequest() && req.getDirection() == Direction::UP)) {
                // Choose the closest candidate to current floor
                if (candidate == -1 || reqFloor < candidate) {
                    candidate = reqFloor;
                }
            }
        } else if (travelDirection == Direction::DOWN && reqFloor < currentFloor && reqFloor >= primaryFloor) {
            // For downward movement, consider internal or external with DOWN direction
            if (req.checkIsInternalRequest() || 
                (!req.checkIsInternalRequest() && req.getDirection() == Direction::DOWN)) {
                // For downward journey, choose the largest floor less than current
                if (candidate == -1 || reqFloor > candidate) {
                    candidate = reqFloor;
                }
            }
        }
    }
    
    // Return candidate if found, otherwise primary target
    return (candidate != -1) ? candidate : primaryFloor;
}

