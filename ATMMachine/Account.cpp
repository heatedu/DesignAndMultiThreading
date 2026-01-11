#include "Account.h"

Account::Account(const std::string& accNum, double bal) 
    : accountNumber(accNum), balance(bal) {}

std::string Account::getAccountNumber() const {
    return accountNumber;
}

double Account::getBalance() const {
    return balance;
}

bool Account::withdraw(double amount) {
    if (balance >= amount) {
        balance -= amount;
        return true;
    }
    return false;
}

void Account::deposit(double amount) {
    balance += amount;
}

