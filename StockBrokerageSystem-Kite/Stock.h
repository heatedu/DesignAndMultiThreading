#pragma once
#include <string>
#include <vector>
#include <mutex>
#include "IStockObserver.h"

// Stock acts as the Subject in Observer pattern
class Stock {
private:
    std::string symbol_;
    std::string name_;
    double price_;
    std::vector<IStockObserver*> observers_;
    mutable std::mutex mtx_;

    void notifyObservers();

public:
    Stock(const std::string& symbol, const std::string& name, double price);

    std::string getSymbol() const;
    std::string getName() const;
    double getPrice() const;

    void updatePrice(double newPrice);

    void registerObserver(IStockObserver* observer);
    void removeObserver(IStockObserver* observer);
};
