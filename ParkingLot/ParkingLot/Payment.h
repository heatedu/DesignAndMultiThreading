#pragma once

#include <iostream>
#include <memory>
#include "PaymentStrategy.h"

class Payment {
private:
    double amount;
    std::shared_ptr<PaymentStrategy> paymentStrategy;

public:
    Payment(double amount, std::shared_ptr<PaymentStrategy> paymentStrategy)
        : amount(amount), paymentStrategy(paymentStrategy) {
    }

    void processPayment() const {
        if (amount > 0) {
            paymentStrategy->processPayment(amount);  // Delegate to strategy
        }
        else {
            std::cout << "Invalid payment amount." << std::endl;
        }
    }
};
