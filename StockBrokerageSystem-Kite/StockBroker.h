#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>
#include "User.h"
#include "Account.h"
#include "Stock.h"
#include "Order.h"
#include "OrderFactory.h"

// Singleton Pattern — single broker instance managing the entire system
class StockBroker {
private:
    std::unordered_map<std::string, std::unique_ptr<Account>> accounts_;   // accountId -> Account
    std::unordered_map<std::string, std::unique_ptr<Stock>> stocks_;       // symbol -> Stock
    std::vector<std::unique_ptr<Order>> orders_;
    mutable std::mutex mtx_;

    // Private constructor for Singleton
    StockBroker() = default;
    StockBroker(const StockBroker&) = delete;
    StockBroker& operator=(const StockBroker&) = delete;

public:
    // Thread-safe Meyers' singleton
    static StockBroker& getInstance();

    // Account management
    Account* createAccount(const std::string& accountId, const User& user, double initialBalance);
    Account* getAccount(const std::string& accountId);

    // Stock management
    void addStock(const std::string& symbol, const std::string& name, double price);
    Stock* getStock(const std::string& symbol);

    // Order processing
    void placeOrder(OrderType type, Account& account, Stock& stock, int quantity, double price);

    // Utility
    void printPortfolio(const Account& account) const;
    void printTransactionHistory(const Account& account) const;
};
