#pragma once
#include "Split.h"

/**
 * Implementation of Split interface for equal split.
 */
class EqualSplit : public Split {
public:
    std::map<User, double> calculateSplit(
        double amount,
        const std::vector<User>& participants,
        const SplitDetails& details) override;
};
