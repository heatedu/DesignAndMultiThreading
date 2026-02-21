#pragma once
#include "Order.h"

class BuyOrder : public Order {
public:
    BuyOrder(Account& account, Stock& stock, int quantity, double price);
    void execute() override;
    std::string getTypeName() const override;
};
