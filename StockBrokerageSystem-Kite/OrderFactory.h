#pragma once
#include <memory>
#include "OrderStatus.h"
#include "BuyOrder.h"
#include "SellOrder.h"

// Factory Pattern — creates concrete Order objects based on OrderType
class OrderFactory {
public:
    static std::unique_ptr<Order> createOrder(
        OrderType type, Account& account, Stock& stock, int quantity, double price)
    {
        switch (type) {
            case OrderType::BUY:
                return std::make_unique<BuyOrder>(account, stock, quantity, price);
            case OrderType::SELL:
                return std::make_unique<SellOrder>(account, stock, quantity, price);
            default:
                return nullptr;
        }
    }
};
