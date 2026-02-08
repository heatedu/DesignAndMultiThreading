#pragma once

class Elevator; // Forward declaration

// Strategy Pattern for Scheduling
class SchedulingStrategy {
public:
    virtual ~SchedulingStrategy() = default;
    
    // Determines the next stop for the given elevator
    virtual int getNextStop(Elevator* elevator) = 0;
};

