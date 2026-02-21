#include "StockBroker.h"
#include <iostream>

StockBroker& StockBroker::getInstance() {
    static StockBroker instance;  // thread-safe in C++11+
    return instance;
}

Account* StockBroker::createAccount(const std::string& accountId, const User& user, double initialBalance) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto acc = std::make_unique<Account>(accountId, user, initialBalance);
    Account* ptr = acc.get();
    accounts_[accountId] = std::move(acc);
    std::cout << "[StockBroker] Account created: " << accountId
              << " for user " << user.getName() << "\n";
    return ptr;
}

Account* StockBroker::getAccount(const std::string& accountId) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = accounts_.find(accountId);
    return (it != accounts_.end()) ? it->second.get() : nullptr;
}

void StockBroker::addStock(const std::string& symbol, const std::string& name, double price) {
    std::lock_guard<std::mutex> lock(mtx_);
    stocks_[symbol] = std::make_unique<Stock>(symbol, name, price);
    std::cout << "[StockBroker] Stock added: " << symbol << " (" << name << ") @ $" << price << "\n";
}

Stock* StockBroker::getStock(const std::string& symbol) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = stocks_.find(symbol);
    return (it != stocks_.end()) ? it->second.get() : nullptr;
}

void StockBroker::placeOrder(OrderType type, Account& account, Stock& stock, int quantity, double price) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto order = OrderFactory::createOrder(type, account, stock, quantity, price);
    if (!order) {
        std::cerr << "[StockBroker] Invalid order type\n";
        return;
    }
    std::cout << "[StockBroker] Processing order " << order->getOrderId()
              << " (" << order->getTypeName() << ")\n";
    order->execute();   // executes buy/sell logic
    orders_.push_back(std::move(order));
}

void StockBroker::printPortfolio(const Account& account) const {
    std::cout << "\n=== Portfolio for Account " << account.getAccountId() << " ===\n";
    std::cout << "Balance: $" << account.getBalance() << "\n";
    const auto& holdings = account.getPortfolio().getHoldings();
    if (holdings.empty()) {
        std::cout << "(no holdings)\n";
    }
    for (const auto& [symbol, qty] : holdings) {
        std::cout << "  " << symbol << ": " << qty << " shares\n";
    }
    std::cout << "=======================================\n";
}

void StockBroker::printTransactionHistory(const Account& account) const {
    std::cout << "\n=== Transaction History for Account " << account.getAccountId() << " ===\n";
    const auto& history = account.getTransactionHistory();
    if (history.empty()) {
        std::cout << "(no transactions)\n";
    }
    for (const auto* order : history) {
        std::string statusStr;
        switch (order->getStatus()) {
            case OrderStatus::PENDING:  statusStr = "PENDING";  break;
            case OrderStatus::EXECUTED: statusStr = "EXECUTED"; break;
            case OrderStatus::REJECTED: statusStr = "REJECTED"; break;
        }
        std::cout << "  " << order->getOrderId()
                  << " | " << order->getTypeName()
                  << " | " << order->getStock().getSymbol()
                  << " | qty=" << order->getQuantity()
                  << " | price=$" << order->getPrice()
                  << " | " << statusStr << "\n";
    }
    std::cout << "=================================================\n";
}
