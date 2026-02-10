#include "../include/Expense.h"

Expense::Expense(const std::string& id, const std::string& description, double amount,
                 const User& payer, const std::vector<User>& participants,
                 const std::map<User, double>& shares)
    : id(id), description(description), amount(amount), payer(payer),
      participants(participants), shares(shares) {}

std::string Expense::getId() const {
    return id;
}

std::string Expense::getDescription() const {
    return description;
}

double Expense::getAmount() const {
    return amount;
}

User Expense::getPayer() const {
    return payer;
}

std::vector<User> Expense::getParticipants() const {
    return participants;
}

std::map<User, double> Expense::getShares() const {
    return shares;
}
