#include "VendingMachineContext.h"
#include "IdleState.h"
#include "HasMoneyState.h"
#include "SelectionState.h"
#include "DispenseState.h"
#include "OutOfStockState.h"

VendingMachineContext::VendingMachineContext()
    : currentState(new IdleState()), inventory(10) {
    std::cout << "Initialized in Idle State\n";
}

VendingMachineContext::~VendingMachineContext() {
    delete currentState;
}

void VendingMachineContext::advanceState() {
    currentState = currentState->next(this);
    std::cout << "State -> " << currentState->getStateName() << "\n";
}

void VendingMachineContext::insertCoin(Coin c) {
    coins.push_back(c);
    std::cout << "Inserted coin: " << coinValue(c) << "\n";
    advanceState();
}

void VendingMachineContext::startSelection(int code) {
    advanceState();
    selectProduct(code);
}

void VendingMachineContext::selectProduct(int code) {
    int balance = getBalance();
    Item item = inventory.getItem(code);

    if (balance < item.price) {
        std::cout << "Insufficient balance\n";
        return;
    }

    setSelectedCode(code);
    advanceState();
    dispenseItem(code);
}

void VendingMachineContext::dispenseItem(int code) {
    Item item = inventory.getItem(code);
    std::cout << "Dispensing: ";

    switch (item.type) {
    case ItemType::COKE: std::cout << "COKE\n"; break;
    case ItemType::PEPSI: std::cout << "PEPSI\n"; break;
    case ItemType::JUICE: std::cout << "JUICE\n"; break;
    case ItemType::SODA: std::cout << "SODA\n"; break;
    }

    inventory.removeItem(code);

    resetCoins();
    resetSelection();
    advanceState();
}
