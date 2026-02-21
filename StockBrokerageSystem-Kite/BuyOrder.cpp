#include "BuyOrder.h"
#include <iostream>

BuyOrder::BuyOrder(Account& account, Stock& stock, int quantity, double price)
    : Order(account, stock, quantity, price) {}

void BuyOrder::execute() {
    double totalCost = quantity_ * price_;
    try {
        account_.withdraw(totalCost);                        // debit funds
        account_.getPortfolio().addStock(stock_.getSymbol(), quantity_);  // add shares
        status_ = OrderStatus::EXECUTED;
        std::cout << "[BuyOrder] " << orderId_ << " EXECUTED: Bought "
                  << quantity_ << " shares of " << stock_.getSymbol()
                  << " @ $" << price_ << "\n";
    } catch (const InsufficientFundsException& e) {
        status_ = OrderStatus::REJECTED;
        std::cout << "[BuyOrder] " << orderId_ << " REJECTED: " << e.what() << "\n";
    }
    account_.addToHistory(this);
}

std::string BuyOrder::getTypeName() const { return "BUY"; }
