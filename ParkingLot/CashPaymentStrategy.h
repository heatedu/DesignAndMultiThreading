#pragma once

#include <iostream>
#include "PaymentStrategy.h"

class CashPayment : public PaymentStrategy {
public:
    CashPayment(double fee) {
        // You can store `fee` if needed; not used in original Java code
    }

    void processPayment(double amount) override {
        std::cout << "Processing cash payment of $" << amount << std::endl;
        // Additional cash payment logic can go here
    }
};
