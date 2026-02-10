#include "../include/PercentageSplit.h"
#include <stdexcept>

std::map<User, double> PercentageSplit::calculateSplit(
    double amount,
    const std::vector<User>& participants,
    const SplitDetails& details) {
    
    std::map<User, double> splits;
    
    // Check if percentages are provided
    if (details.percentages.empty()) {
        throw std::invalid_argument("Percentages not provided in split details");
    }
    
    // Calculate split based on percentages
    for (const User& user : participants) {
        double percentage = 0.0;
        if (details.percentages.find(user) != details.percentages.end()) {
            percentage = details.percentages.at(user);
        }
        splits[user] = amount * percentage / 100.0;
    }
    
    return splits;
}
