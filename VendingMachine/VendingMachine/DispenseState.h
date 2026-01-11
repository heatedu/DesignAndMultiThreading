#pragma once
#include "VendingMachineState.h"

class DispenseState : public VendingMachineState {
public:
    std::string getStateName() override { return "DispenseState"; }
    VendingMachineState* next(VendingMachineContext* context) override;
};
