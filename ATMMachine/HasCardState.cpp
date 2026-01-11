#include "HasCardState.h"
#include "SelectOperationState.h"
#include "IdleState.h"
#include "ATMMachine.h"
#include <iostream>

HasCardState::HasCardState() : pinAttempts(0) {}

void HasCardState::insertCard(ATMMachine* atm, Card* card) {
    std::cout << "Card already inserted. Please enter PIN or eject card.\n";
}

void HasCardState::enterPin(ATMMachine* atm, const std::string& pin) {
    Card* card = atm->getCurrentCard();
    
    if (card->validatePin(pin)) {
        std::cout << "PIN validated successfully!\n";
        atm->setState(new SelectOperationState());
        std::cout << "\nSelect operation:\n1. Withdraw Cash\n2. Check Balance\n3. Exit\n";
    } else {
        pinAttempts++;
        std::cout << "Incorrect PIN. Attempts remaining: " << (MAX_PIN_ATTEMPTS - pinAttempts) << "\n";
        
        if (pinAttempts >= MAX_PIN_ATTEMPTS) {
            std::cout << "Maximum PIN attempts exceeded. Card ejected.\n";
            atm->setCurrentCard(nullptr);
            atm->setState(new IdleState());
        }
    }
}

void HasCardState::selectOperation(ATMMachine* atm, int operation) {
    std::cout << "Please enter your PIN first.\n";
}

void HasCardState::withdrawCash(ATMMachine* atm, double amount) {
    std::cout << "Please enter your PIN first.\n";
}

void HasCardState::checkBalance(ATMMachine* atm) {
    std::cout << "Please enter your PIN first.\n";
}

void HasCardState::ejectCard(ATMMachine* atm) {
    std::cout << "Card ejected.\n";
    atm->setCurrentCard(nullptr);
    atm->setState(new IdleState());
}

std::string HasCardState::getStateName() const {
    return "Has Card";
}

