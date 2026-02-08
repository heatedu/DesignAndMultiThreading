#pragma once
#include "VendingMachineState.h"

class OutOfStockState : public VendingMachineState {
public:
    std::string getStateName() override { return "OutOfStockState"; }
    VendingMachineState* next(VendingMachineContext* context) override;
};
