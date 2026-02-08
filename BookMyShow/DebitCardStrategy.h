#pragma once
#include "PaymentStrategy.h"

class DebitCardStrategy : public PaymentStrategy {
public:
    bool processPayment() override;
};

