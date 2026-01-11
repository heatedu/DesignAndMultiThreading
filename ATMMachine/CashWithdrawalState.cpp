#include "CashWithdrawalState.h"
#include "SelectOperationState.h"
#include "ATMMachine.h"
#include <iostream>

void CashWithdrawalState::insertCard(ATMMachine* atm, Card* card) {
    std::cout << "Transaction in progress. Please wait.\n";
}

void CashWithdrawalState::enterPin(ATMMachine* atm, const std::string& pin) {
    std::cout << "Transaction in progress. Please wait.\n";
}

void CashWithdrawalState::selectOperation(ATMMachine* atm, int operation) {
    std::cout << "Transaction in progress. Please wait.\n";
}

void CashWithdrawalState::withdrawCash(ATMMachine* atm, double amount) {
    Card* card = atm->getCurrentCard();
    Account* account = card->getAccount();
    
    // Validation checks
    if (amount <= 0) {
        std::cout << "Invalid amount.\n";
        atm->getTransactionLog().logTransaction(card->getCardNumber(), "WITHDRAWAL", amount, false);
        atm->setState(new SelectOperationState());
        return;
    }
    
    if (amount > account->getBalance()) {
        std::cout << "Insufficient funds in account.\n";
        atm->getTransactionLog().logTransaction(card->getCardNumber(), "WITHDRAWAL", amount, false);
        atm->setState(new SelectOperationState());
        return;
    }
    
    if (amount > atm->getCashDispenser().getTotalCash()) {
        std::cout << "Insufficient cash in ATM.\n";
        atm->getTransactionLog().logTransaction(card->getCardNumber(), "WITHDRAWAL", amount, false);
        atm->setState(new SelectOperationState());
        return;
    }
    
    // Process withdrawal
    if (account->withdraw(amount) && atm->getCashDispenser().dispenseCash(amount)) {
        std::cout << "Withdrawal successful! Amount: $" << amount << "\n";
        std::cout << "Remaining balance: $" << account->getBalance() << "\n";
        atm->getTransactionLog().logTransaction(card->getCardNumber(), "WITHDRAWAL", amount, true);
    } else {
        std::cout << "Transaction failed.\n";
        atm->getTransactionLog().logTransaction(card->getCardNumber(), "WITHDRAWAL", amount, false);
    }
    
    atm->setState(new SelectOperationState());
    std::cout << "\nSelect operation:\n1. Withdraw Cash\n2. Check Balance\n3. Exit\n";
}

void CashWithdrawalState::checkBalance(ATMMachine* atm) {
    std::cout << "Transaction in progress. Please wait.\n";
}

void CashWithdrawalState::ejectCard(ATMMachine* atm) {
    std::cout << "Transaction in progress. Please wait.\n";
}

std::string CashWithdrawalState::getStateName() const {
    return "Cash Withdrawal";
}

