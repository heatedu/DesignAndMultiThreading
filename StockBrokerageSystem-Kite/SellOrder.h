#pragma once
#include "Order.h"

class SellOrder : public Order {
public:
    SellOrder(Account& account, Stock& stock, int quantity, double price);
    void execute() override;
    std::string getTypeName() const override;
};
