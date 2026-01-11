#pragma once

// Enum to represent the direction of the elevator
enum class Direction {
    UP,      // The elevator is moving upward
    DOWN,    // The elevator is moving downward
    IDLE     // The elevator is stationary, not moving
};

// Enum to represent the state of the elevator
enum class ElevatorState {
    IDLE,         // The elevator is not moving, waiting for requests
    MOVING,       // The elevator is in motion (either up or down)
    STOPPED,      // The elevator has temporarily stopped (e.g., at a floor)
    MAINTENANCE   // The elevator is out of service and undergoing maintenance
};

