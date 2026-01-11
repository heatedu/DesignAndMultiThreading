#pragma once
#include "ATMState.h"

class SelectOperationState : public ATMState {
public:
    void insertCard(ATMMachine* atm, Card* card) override;
    void enterPin(ATMMachine* atm, const std::string& pin) override;
    void selectOperation(ATMMachine* atm, int operation) override;
    void withdrawCash(ATMMachine* atm, double amount) override;
    void checkBalance(ATMMachine* atm) override;
    void ejectCard(ATMMachine* atm) override;
    
    std::string getStateName() const override;
};

