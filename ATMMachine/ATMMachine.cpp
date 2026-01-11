#include "ATMMachine.h"
#include "IdleState.h"
#include <iostream>

ATMMachine::ATMMachine() 
    : currentCard(nullptr), maintenanceMode(false) {
    currentState = new IdleState();
}

ATMMachine::~ATMMachine() {
    delete currentState;
}

void ATMMachine::setState(ATMState* state) {
    delete currentState;
    currentState = state;
}

ATMState* ATMMachine::getState() const {
    return currentState;
}

void ATMMachine::setCurrentCard(Card* card) {
    currentCard = card;
}

Card* ATMMachine::getCurrentCard() const {
    return currentCard;
}

CashDispenser& ATMMachine::getCashDispenser() {
    return cashDispenser;
}

TransactionLog& ATMMachine::getTransactionLog() {
    return transactionLog;
}

bool ATMMachine::isInMaintenanceMode() const {
    return maintenanceMode;
}

void ATMMachine::setMaintenanceMode(bool mode) {
    maintenanceMode = mode;
    if (mode) {
        std::cout << "\n*** ATM is now in MAINTENANCE MODE ***\n";
    } else {
        std::cout << "\n*** ATM is now OPERATIONAL ***\n";
    }
}

void ATMMachine::insertCard(Card* card) {
    if (maintenanceMode) {
        std::cout << "ATM is under maintenance. Please try again later.\n";
        return;
    }
    currentState->insertCard(this, card);
}

void ATMMachine::enterPin(const std::string& pin) {
    currentState->enterPin(this, pin);
}

void ATMMachine::selectOperation(int operation) {
    currentState->selectOperation(this, operation);
}

void ATMMachine::withdrawCash(double amount) {
    currentState->withdrawCash(this, amount);
}

void ATMMachine::checkBalance() {
    currentState->checkBalance(this);
}

void ATMMachine::ejectCard() {
    currentState->ejectCard(this);
}

void ATMMachine::displayStatus() const {
    std::cout << "\n=== ATM Status ===\n";
    std::cout << "Current State: " << currentState->getStateName() << "\n";
    std::cout << "Maintenance Mode: " << (maintenanceMode ? "YES" : "NO") << "\n";
}

