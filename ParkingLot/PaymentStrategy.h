#pragma once

class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;

    // Pure virtual function → makes this an interface-like class in C++
    virtual void processPayment(double amount) = 0;
};