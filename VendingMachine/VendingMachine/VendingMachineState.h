#pragma once
#include <string>

class VendingMachineContext;

class VendingMachineState
{
public:
    virtual ~VendingMachineState() = default;

    virtual std::string getStateName() = 0;
    virtual VendingMachineState* next(VendingMachineContext* context) = 0;
};