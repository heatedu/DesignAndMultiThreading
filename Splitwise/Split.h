#pragma once
#include <map>
#include <vector>
#include <string>
#include <any>
#include "User.h"

/**
 * Interface defining the behavior for all types of splits.
 */
class Split {
public:
    virtual ~Split() = default;
    
    /**
     * Calculates the split for the given amount among participants.
     * @param amount The total amount to split
     * @param participants The list of users participating in the split
     * @param splitDetails Additional details required for the specific split type
     * @return A map where the key is the User and the value is the amount they owe
     */
    virtual std::map<User, double> calculateSplit(
        double amount,
        const std::vector<User>& participants,
        const std::map<std::string, std::any>& splitDetails) = 0;
};
