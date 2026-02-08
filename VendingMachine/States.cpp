#include "IdleState.h"
#include "HasMoneyState.h"
#include "SelectionState.h"
#include "DispenseState.h"
#include "OutOfStockState.h"
#include "VendingMachineContext.h"

VendingMachineState* IdleState::next(VendingMachineContext* context) {
    if (!context->getInventory().hasItems())
        return new OutOfStockState();
    if (!context->getCoins().empty())
        return new HasMoneyState();
    return this;
}

VendingMachineState* HasMoneyState::next(VendingMachineContext* context) 
{
    if (!context->getInventory().hasItems())
        return new OutOfStockState();

    if (context->getCoins().empty())
        return new IdleState();

    return new SelectionState();
}

VendingMachineState* SelectionState::next(VendingMachineContext* context) {
    if (!context->getInventory().hasItems())
        return new OutOfStockState();

    if (context->getCoins().empty())
        return new IdleState();

    if (context->getSelectedCode() > 0)
        return new DispenseState();

    return this;
}

VendingMachineState* DispenseState::next(VendingMachineContext* context) {
    return new IdleState();
}

VendingMachineState* OutOfStockState::next(VendingMachineContext* context) {
    if (context->getInventory().hasItems())
        return new IdleState();

    return this;
}

