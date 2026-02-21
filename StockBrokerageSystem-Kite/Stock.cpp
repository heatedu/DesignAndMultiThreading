#include "Stock.h"
#include <algorithm>
#include <iostream>

Stock::Stock(const std::string& symbol, const std::string& name, double price)
    : symbol_(symbol), name_(name), price_(price) {}

std::string Stock::getSymbol() const { return symbol_; }
std::string Stock::getName() const { return name_; }
double Stock::getPrice() const { return price_; }

void Stock::updatePrice(double newPrice) {
    std::lock_guard<std::mutex> lock(mtx_);
    price_ = newPrice;
    std::cout << "[Stock] " << symbol_ << " price updated to " << newPrice << "\n";
    notifyObservers();
}

void Stock::notifyObservers() {
    for (auto* obs : observers_) {
        obs->onPriceUpdate(symbol_, price_);
    }
}

void Stock::registerObserver(IStockObserver* observer) {
    std::lock_guard<std::mutex> lock(mtx_);
    observers_.push_back(observer);
}

void Stock::removeObserver(IStockObserver* observer) {
    std::lock_guard<std::mutex> lock(mtx_);
    observers_.erase(
        std::remove(observers_.begin(), observers_.end(), observer),
        observers_.end());
}
