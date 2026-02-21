#pragma once
#include <string>
#include <unordered_map>
#include <mutex>
#include "Exceptions.h"

class Portfolio {
private:
    std::unordered_map<std::string, int> holdings_;  // stockSymbol -> quantity
    mutable std::mutex mtx_;

public:
    void addStock(const std::string& symbol, int quantity);
    void removeStock(const std::string& symbol, int quantity);  // throws InsufficientStockException
    int getStockQuantity(const std::string& symbol) const;
    const std::unordered_map<std::string, int>& getHoldings() const;
};
