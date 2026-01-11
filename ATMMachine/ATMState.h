#pragma once
#include <string>

class ATMMachine; // Forward declaration

class ATMState {
public:
    virtual ~ATMState() = default;
    
    virtual void insertCard(ATMMachine* atm, class Card* card) = 0;
    virtual void enterPin(ATMMachine* atm, const std::string& pin) = 0;
    virtual void selectOperation(ATMMachine* atm, int operation) = 0;
    virtual void withdrawCash(ATMMachine* atm, double amount) = 0;
    virtual void checkBalance(ATMMachine* atm) = 0;
    virtual void ejectCard(ATMMachine* atm) = 0;
    
    virtual std::string getStateName() const = 0;
};

