#include "Portfolio.h"

void Portfolio::addStock(const std::string& symbol, int quantity) {
    std::lock_guard<std::mutex> lock(mtx_);
    holdings_[symbol] += quantity;
}

void Portfolio::removeStock(const std::string& symbol, int quantity) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = holdings_.find(symbol);
    if (it == holdings_.end() || it->second < quantity) {
        throw InsufficientStockException(
            "Insufficient stock: " + symbol +
            " (have " + std::to_string(it == holdings_.end() ? 0 : it->second) +
            ", need " + std::to_string(quantity) + ")");
    }
    it->second -= quantity;
    if (it->second == 0) {
        holdings_.erase(it);
    }
}

int Portfolio::getStockQuantity(const std::string& symbol) const {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = holdings_.find(symbol);
    return (it != holdings_.end()) ? it->second : 0;
}

const std::unordered_map<std::string, int>& Portfolio::getHoldings() const {
    return holdings_;
}
