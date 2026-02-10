#include "../include/Transaction.h"

Transaction::Transaction(const User& from, const User& to, double amount)
    : from(from), to(to), amount(amount) {}

User Transaction::getFrom() const {
    return from;
}

User Transaction::getTo() const {
    return to;
}

double Transaction::getAmount() const {
    return amount;
}
