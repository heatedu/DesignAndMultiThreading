#pragma once
#include "User.h"

/**
 * Represents a financial transaction between two users.
 */
class Transaction {
private:
    User from;
    User to;
    double amount;

public:
    Transaction(const User& from, const User& to, double amount);
    
    User getFrom() const;
    User getTo() const;
    double getAmount() const;
};
