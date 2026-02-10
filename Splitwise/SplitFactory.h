#pragma once
#include <string>
#include <memory>
#include "Split.h"

/**
 * Factory class for creating instances of different Split types.
 * Implements the Factory Design Pattern.
 */
class SplitFactory {
public:
    /**
     * Factory method to create a Split instance based on the specified split type.
     * @param splitType The type of split to create ("EQUAL", "PERCENTAGE")
     * @return A unique pointer to the corresponding Split implementation
     */
    static std::unique_ptr<Split> createSplit(const std::string& splitType);
};
