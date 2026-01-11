#include "SelectOperationState.h"
#include "CashWithdrawalState.h"
#include "CheckBalanceState.h"
#include "IdleState.h"
#include "ATMMachine.h"
#include <iostream>

void SelectOperationState::insertCard(ATMMachine* atm, Card* card) {
    std::cout << "Card already inserted. Please select an operation.\n";
}

void SelectOperationState::enterPin(ATMMachine* atm, const std::string& pin) {
    std::cout << "Already authenticated. Please select an operation.\n";
}

void SelectOperationState::selectOperation(ATMMachine* atm, int operation) {
    switch (operation) {
        case 1:
            atm->setState(new CashWithdrawalState());
            std::cout << "Enter amount to withdraw: ";
            break;
        case 2:
            atm->setState(new CheckBalanceState());
            atm->checkBalance();
            break;
        case 3:
            ejectCard(atm);
            break;
        default:
            std::cout << "Invalid operation. Please try again.\n";
    }
}

void SelectOperationState::withdrawCash(ATMMachine* atm, double amount) {
    std::cout << "Please select operation 1 for withdrawal.\n";
}

void SelectOperationState::checkBalance(ATMMachine* atm) {
    std::cout << "Please select operation 2 for balance check.\n";
}

void SelectOperationState::ejectCard(ATMMachine* atm) {
    std::cout << "Thank you for using our ATM. Card ejected.\n";
    atm->setCurrentCard(nullptr);
    atm->setState(new IdleState());
}

std::string SelectOperationState::getStateName() const {
    return "Select Operation";
}

