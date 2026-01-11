#pragma once
#include "CommonEnums.h"

// Command Pattern for Request Processing
class ElevatorCommand {
public:
    virtual ~ElevatorCommand() = default;
    
    // Method to execute the command
    virtual void execute() = 0;
};

