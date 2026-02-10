#include "../include/EqualSplit.h"

std::map<User, double> EqualSplit::calculateSplit(
    double amount,
    const std::vector<User>& participants,
    const SplitDetails& details) {
    
    // Equal split doesn't need any details
    double amountPerPerson = amount / participants.size();
    std::map<User, double> splits;
    
    for (const User& user : participants) {
        splits[user] = amountPerPerson;
    }
    
    return splits;
}
