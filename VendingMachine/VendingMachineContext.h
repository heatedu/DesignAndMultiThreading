#pragma once
#include <vector>
#include <iostream>
#include "Inventory.h"
#include "Coin.h"
#include "VendingMachineState.h"
#include "IdleState.h" 

class HasMoneyState;
class SelectionState;
class DispenseState;
class OutOfStockState;

class VendingMachineContext
{
private:
	VendingMachineState* currentState;
	Inventory inventory;
	std::vector<Coin> coins;
	int selectedItemCode = 0;
public:
	VendingMachineContext();
	~VendingMachineContext();

    void advanceState();

    // actions
    void insertCoin(Coin c);
    void startSelection(int code);
    void selectProduct(int code);
    void dispenseItem(int code);

    // getters
    Inventory& getInventory() { return inventory; }
    VendingMachineState* getState() { return currentState; }
    std::vector<Coin>& getCoins() { return coins; }
    int getBalance() {
        int sum = 0;
        for (auto& c : coins) sum += coinValue(c);
        return sum;
    }

    // utils
    void resetCoins() { coins.clear(); }
    void resetSelection() { selectedItemCode = 0; }
    void setSelectedCode(int c) { selectedItemCode = c; }
    int getSelectedCode() const { return selectedItemCode; }
};
