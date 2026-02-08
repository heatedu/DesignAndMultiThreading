#pragma once
#include "VendingMachineState.h"

class IdleState : public VendingMachineState {
public:
    std::string getStateName() override { return "IdleState"; }
    VendingMachineState* next(VendingMachineContext* context) override;
};
