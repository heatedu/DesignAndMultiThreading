#include "Account.h"
#include <iostream>

Account::Account(const std::string& accountId, const User& user, double initialBalance)
    : accountId_(accountId), user_(user), balance_(initialBalance) {}

std::string Account::getAccountId() const { return accountId_; }
const User& Account::getUser() const { return user_; }
double Account::getBalance() const { return balance_; }

void Account::deposit(double amount) {
    std::lock_guard<std::mutex> lock(mtx_);
    balance_ += amount;
    std::cout << "[Account " << accountId_ << "] Deposited " << amount
              << ", new balance: " << balance_ << "\n";
}

void Account::withdraw(double amount) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (balance_ < amount) {
        throw InsufficientFundsException(
            "Insufficient funds in account " + accountId_ +
            " (have " + std::to_string(balance_) +
            ", need " + std::to_string(amount) + ")");
    }
    balance_ -= amount;
    std::cout << "[Account " << accountId_ << "] Withdrew " << amount
              << ", new balance: " << balance_ << "\n";
}

Portfolio& Account::getPortfolio() { return portfolio_; }
const Portfolio& Account::getPortfolio() const { return portfolio_; }

void Account::addToHistory(Order* order) {
    std::lock_guard<std::mutex> lock(mtx_);
    transactionHistory_.push_back(order);
}

const std::vector<Order*>& Account::getTransactionHistory() const {
    return transactionHistory_;
}

void Account::onPriceUpdate(const std::string& stockSymbol, double newPrice) {
    int qty = portfolio_.getStockQuantity(stockSymbol);
    if (qty > 0) {
        std::cout << "[Observer] Account " << accountId_
                  << " notified: " << stockSymbol
                  << " now at $" << newPrice
                  << " | holding " << qty << " shares"
                  << " | value $" << qty * newPrice << "\n";
    }
}
