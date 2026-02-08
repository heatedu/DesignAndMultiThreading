#pragma once

// Strategy Pattern for payment methods
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual bool processPayment() = 0;
};

