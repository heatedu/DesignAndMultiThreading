#pragma once

#include <string>
#include <algorithm>
#include <cctype>
#include "ParkingFeeStrategy.h"
#include "DurationType.h"

class PremiumRateStrategy : public ParkingFeeStrategy {
private:
    // Utility: convert string to lowercase
    std::string toLower(const std::string& str) const {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return result;
    }

public:
    double calculateFee(const std::string& vehicleType,
        int duration,
        DurationType durationType) const override
    {
        std::string type = toLower(vehicleType);

        double hourlyRate;

        if (type == "car") {
            hourlyRate = 15.0;
        }
        else if (type == "bike") {
            hourlyRate = 8.0;
        }
        else if (type == "auto") {
            hourlyRate = 12.0;
        }
        else {
            hourlyRate = 20.0; // default for others
        }

        if (durationType == DurationType::HOURS) {
            return duration * hourlyRate;
        }
        else {
            return duration * hourlyRate * 24.0; // daily rate
        }
    }
};
