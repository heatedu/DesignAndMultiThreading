#pragma once
#include <map>
#include "User.h"

/**
 * Structure to hold split-specific details
 * Replaces std::any for type safety and clarity
 */
struct SplitDetails {
    // For percentage-based splits
    std::map<User, double> percentages;
    
    // For exact amount splits (future extensibility)
    std::map<User, double> exactAmounts;
    
    // Default constructor
    SplitDetails() = default;
    
    // Constructor for percentage split
    explicit SplitDetails(const std::map<User, double>& percentages)
        : percentages(percentages) {}
};
