#pragma once
#include <string>
#include <vector>
#include <map>
#include "User.h"

/**
 * Represents an expense shared between users.
 */
class Expense {
private:
    std::string id;
    std::string description;
    double amount;
    User payer;
    std::vector<User> participants;
    std::map<User, double> shares;

public:
    Expense(const std::string& id, const std::string& description, double amount,
            const User& payer, const std::vector<User>& participants,
            const std::map<User, double>& shares);
    
    std::string getId() const;
    std::string getDescription() const;
    double getAmount() const;
    User getPayer() const;
    std::vector<User> getParticipants() const;
    std::map<User, double> getShares() const;
};
