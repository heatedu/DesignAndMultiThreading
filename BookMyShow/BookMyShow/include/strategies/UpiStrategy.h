#pragma once
#include "PaymentStrategy.h"

class UpiStrategy : public PaymentStrategy {
public:
    bool processPayment() override;
};

