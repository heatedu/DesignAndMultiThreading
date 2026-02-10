#include "../include/SplitFactory.h"
#include "../include/EqualSplit.h"
#include "../include/PercentageSplit.h"
#include <stdexcept>

std::unique_ptr<Split> SplitFactory::createSplit(const std::string& splitType) {
    if (splitType == "EQUAL") {
        return std::make_unique<EqualSplit>();
    } else if (splitType == "PERCENTAGE") {
        return std::make_unique<PercentageSplit>();
    } else {
        throw std::invalid_argument("Unknown split type: " + splitType);
    }
}
