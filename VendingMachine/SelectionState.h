#pragma once
#include "VendingMachineState.h"

class SelectionState : public VendingMachineState {
public:
    std::string getStateName() override { return "SelectionState"; }
    VendingMachineState* next(VendingMachineContext* context) override;
};
