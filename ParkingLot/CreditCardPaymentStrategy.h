#pragma once

#include <iostream>
#include "PaymentStrategy.h"

class CreditCardPayment : public PaymentStrategy {
public:
    CreditCardPayment(double fee) {
        // Fee parameter included to match Java, but unused
    }

    void processPayment(double amount) override {
        std::cout << "Processing credit card payment of $" << amount << std::endl;
        // Additional credit card logic can be added here
    }
};
