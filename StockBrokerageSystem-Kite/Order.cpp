#include "Order.h"

std::atomic<int> Order::nextId_{1};

std::string Order::generateId() {
    return "ORD-" + std::to_string(nextId_.fetch_add(1));
}

Order::Order(Account& account, Stock& stock, int quantity, double price)
    : orderId_(generateId()),
      account_(account),
      stock_(stock),
      quantity_(quantity),
      price_(price),
      status_(OrderStatus::PENDING) {}

std::string Order::getOrderId() const { return orderId_; }
Account& Order::getAccount() const { return account_; }
Stock& Order::getStock() const { return stock_; }
int Order::getQuantity() const { return quantity_; }
double Order::getPrice() const { return price_; }
OrderStatus Order::getStatus() const { return status_; }
void Order::setStatus(OrderStatus status) { status_ = status; }
