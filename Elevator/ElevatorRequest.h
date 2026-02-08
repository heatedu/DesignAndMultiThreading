#pragma once
#include "ElevatorCommand.h"
#include "CommonEnums.h"

class ElevatorController; // Forward declaration

// Concrete implementation of ElevatorCommand
class ElevatorRequest : public ElevatorCommand {
private:
    int elevatorId;              // ID of the elevator involved in the request
    int floor;                   // Floor where the request is made
    Direction requestDirection;  // The direction of the elevator request
    ElevatorController* controller; // Reference to the ElevatorController
    bool isInternalRequest;      // Distinguishes internal vs external requests

public:
    ElevatorRequest(int elevatorId, int floor, bool isInternalRequest, 
                   Direction direction, ElevatorController* controller = nullptr);
    
    // Execute method to process the request via the controller
    void execute() override;
    
    // Getters
    Direction getDirection() const { return requestDirection; }
    int getFloor() const { return floor; }
    bool checkIsInternalRequest() const { return isInternalRequest; }
    
    // For comparison in queue
    bool operator==(const ElevatorRequest& other) const {
        return floor == other.floor && 
               elevatorId == other.elevatorId && 
               isInternalRequest == other.isInternalRequest;
    }
};

