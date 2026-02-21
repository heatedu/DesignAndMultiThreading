#pragma once
#include <string>

// Observer interface for stock price changes
class IStockObserver {
public:
    virtual ~IStockObserver() = default;
    virtual void onPriceUpdate(const std::string& stockSymbol, double newPrice) = 0;
};
