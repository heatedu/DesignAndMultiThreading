#include "../include/EqualSplit.h"

std::map<User, double> EqualSplit::calculateSplit(
    double amount,
    const std::vector<User>& participants,
    const std::map<std::string, std::any>& splitDetails) {
    
    double amountPerPerson = amount / participants.size();
    std::map<User, double> splits;
    
    for (const User& user : participants) {
        splits[user] = amountPerPerson;
    }
    
    return splits;
}
