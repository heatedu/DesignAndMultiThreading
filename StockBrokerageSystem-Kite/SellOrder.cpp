#include "SellOrder.h"
#include <iostream>

SellOrder::SellOrder(Account& account, Stock& stock, int quantity, double price)
    : Order(account, stock, quantity, price) {}

void SellOrder::execute() {
    try {
        account_.getPortfolio().removeStock(stock_.getSymbol(), quantity_);  // remove shares
        double totalCredit = quantity_ * price_;
        account_.deposit(totalCredit);                                       // credit funds
        status_ = OrderStatus::EXECUTED;
        std::cout << "[SellOrder] " << orderId_ << " EXECUTED: Sold "
                  << quantity_ << " shares of " << stock_.getSymbol()
                  << " @ $" << price_ << "\n";
    } catch (const InsufficientStockException& e) {
        status_ = OrderStatus::REJECTED;
        std::cout << "[SellOrder] " << orderId_ << " REJECTED: " << e.what() << "\n";
    }
    account_.addToHistory(this);
}

std::string SellOrder::getTypeName() const { return "SELL"; }
