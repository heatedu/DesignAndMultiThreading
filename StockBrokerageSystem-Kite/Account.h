#pragma once
#include <string>
#include <vector>
#include <mutex>
#include "User.h"
#include "Portfolio.h"
#include "IStockObserver.h"
#include "Exceptions.h"

class Order;  // forward declaration

// Account implements IStockObserver to get notified of price changes
class Account : public IStockObserver {//Important account must be observer to track all stocks' prices in the portfolio,Subject:Stock
private:
    std::string accountId_;
    User user_;
    double balance_;
    Portfolio portfolio_;
    std::vector<Order*> transactionHistory_;
    mutable std::mutex mtx_;

public:
    Account(const std::string& accountId, const User& user, double initialBalance);

    std::string getAccountId() const;
    const User& getUser() const;
    double getBalance() const;

    void deposit(double amount);
    void withdraw(double amount);  // throws InsufficientFundsException

    Portfolio& getPortfolio();
    const Portfolio& getPortfolio() const;
    void addToHistory(Order* order);
    const std::vector<Order*>& getTransactionHistory() const;

    // Observer callback
    void onPriceUpdate(const std::string& stockSymbol, double newPrice) override;//###Important it is notifying for individual stock
};
