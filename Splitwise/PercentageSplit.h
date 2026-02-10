#pragma once
#include "Split.h"

/**
 * Implementation of Split interface for percentage-based split.
 */
class PercentageSplit : public Split {
public:
    std::map<User, double> calculateSplit(
        double amount,
        const std::vector<User>& participants,
        const std::map<std::string, std::any>& splitDetails) override;
};
