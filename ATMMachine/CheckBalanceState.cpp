#include "CheckBalanceState.h"
#include "SelectOperationState.h"
#include "ATMMachine.h"
#include <iostream>

void CheckBalanceState::insertCard(ATMMachine* atm, Card* card) {
    std::cout << "Transaction in progress. Please wait.\n";
}

void CheckBalanceState::enterPin(ATMMachine* atm, const std::string& pin) {
    std::cout << "Transaction in progress. Please wait.\n";
}

void CheckBalanceState::selectOperation(ATMMachine* atm, int operation) {
    std::cout << "Transaction in progress. Please wait.\n";
}

void CheckBalanceState::withdrawCash(ATMMachine* atm, double amount) {
    std::cout << "Transaction in progress. Please wait.\n";
}

void CheckBalanceState::checkBalance(ATMMachine* atm) {
    Card* card = atm->getCurrentCard();
    Account* account = card->getAccount();
    
    std::cout << "\n=== Balance Inquiry ===\n";
    std::cout << "Account: " << account->getAccountNumber() << "\n";
    std::cout << "Available Balance: $" << account->getBalance() << "\n";
    
    atm->getTransactionLog().logTransaction(card->getCardNumber(), "BALANCE_CHECK", 0, true);
    
    atm->setState(new SelectOperationState());
    std::cout << "\nSelect operation:\n1. Withdraw Cash\n2. Check Balance\n3. Exit\n";
}

void CheckBalanceState::ejectCard(ATMMachine* atm) {
    std::cout << "Transaction in progress. Please wait.\n";
}

std::string CheckBalanceState::getStateName() const {
    return "Check Balance";
}

