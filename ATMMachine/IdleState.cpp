#include "IdleState.h"
#include "HasCardState.h"
#include "ATMMachine.h"
#include <iostream>

void IdleState::insertCard(ATMMachine* atm, Card* card) {
    std::cout << "Card inserted: " << card->getCardNumber() << "\n";
    atm->setCurrentCard(card);
    atm->setState(new HasCardState());
    std::cout << "Please enter your PIN.\n";
}

void IdleState::enterPin(ATMMachine* atm, const std::string& pin) {
    std::cout << "Please insert your card first.\n";
}

void IdleState::selectOperation(ATMMachine* atm, int operation) {
    std::cout << "Please insert your card first.\n";
}

void IdleState::withdrawCash(ATMMachine* atm, double amount) {
    std::cout << "Please insert your card first.\n";
}

void IdleState::checkBalance(ATMMachine* atm) {
    std::cout << "Please insert your card first.\n";
}

void IdleState::ejectCard(ATMMachine* atm) {
    std::cout << "No card inserted.\n";
}

std::string IdleState::getStateName() const {
    return "Idle";
}

