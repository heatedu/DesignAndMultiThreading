#include "../include/PercentageSplit.h"
#include <stdexcept>

std::map<User, double> PercentageSplit::calculateSplit(
    double amount,
    const std::vector<User>& participants,
    const std::map<std::string, std::any>& splitDetails) {
    
    std::map<User, double> splits;
    
    // Get the percentages map from splitDetails
    if (splitDetails.find("percentages") == splitDetails.end()) {
        throw std::invalid_argument("Percentages not provided in split details");
    }
    
    auto percentages = std::any_cast<std::map<User, double>>(splitDetails.at("percentages"));
    
    for (const User& user : participants) {
        double percentage = 0.0;
        if (percentages.find(user) != percentages.end()) {
            percentage = percentages.at(user);
        }
        splits[user] = amount * percentage / 100.0;
    }
    
    return splits;
}
