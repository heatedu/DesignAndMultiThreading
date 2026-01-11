#pragma once
#include "ATMState.h"
#include "Card.h"
#include "CashDispenser.h"
#include "TransactionLog.h"
#include <memory>

class ATMMachine {
private:
    ATMState* currentState;
    Card* currentCard;
    CashDispenser cashDispenser;
    TransactionLog transactionLog;
    bool maintenanceMode;

public:
    ATMMachine();
    ~ATMMachine();
    
    void setState(ATMState* state);
    ATMState* getState() const;
    
    void setCurrentCard(Card* card);
    Card* getCurrentCard() const;
    
    CashDispenser& getCashDispenser();
    TransactionLog& getTransactionLog();
    
    bool isInMaintenanceMode() const;
    void setMaintenanceMode(bool mode);
    
    // User operations
    void insertCard(Card* card);
    void enterPin(const std::string& pin);
    void selectOperation(int operation);
    void withdrawCash(double amount);
    void checkBalance();
    void ejectCard();
    
    void displayStatus() const;
};

