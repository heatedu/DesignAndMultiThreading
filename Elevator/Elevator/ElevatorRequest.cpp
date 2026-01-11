#include "ElevatorRequest.h"
#include "ElevatorController.h"

ElevatorRequest::ElevatorRequest(int elevatorId, int floor, bool isInternalRequest,
                                Direction direction, ElevatorController* controller)
    : elevatorId(elevatorId), floor(floor), isInternalRequest(isInternalRequest),
      requestDirection(direction), controller(controller) {
}

void ElevatorRequest::execute() {
    if (controller) {
        if (isInternalRequest) {
            controller->requestFloor(elevatorId, floor);
        } else {
            controller->requestElevator(elevatorId, floor, requestDirection);
        }
    }
}

