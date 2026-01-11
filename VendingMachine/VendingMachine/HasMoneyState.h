#pragma once
#include "VendingMachineState.h"

class HasMoneyState : public VendingMachineState {
public:
    std::string getStateName() override { return "HasMoneyState"; }
    VendingMachineState* next(VendingMachineContext* context) override;
};
