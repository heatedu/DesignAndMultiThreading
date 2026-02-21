#pragma once
#include <string>
#include <atomic>
#include "OrderStatus.h"
#include "Account.h"
#include "Stock.h"

class Order {
protected:
    std::string orderId_;
    Account& account_;
    Stock& stock_;
    int quantity_;
    double price_;
    OrderStatus status_;

    static std::atomic<int> nextId_;
    static std::string generateId();

public:
    Order(Account& account, Stock& stock, int quantity, double price);
    virtual ~Order() = default;

    std::string getOrderId() const;
    Account& getAccount() const;
    Stock& getStock() const;
    int getQuantity() const;
    double getPrice() const;
    OrderStatus getStatus() const;
    void setStatus(OrderStatus status);

    virtual void execute() = 0;  // pure virtual — implemented by BuyOrder/SellOrder
    virtual std::string getTypeName() const = 0;
};
