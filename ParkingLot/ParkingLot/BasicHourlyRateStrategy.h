#pragma once

#include <string>
#include <algorithm>
#include <cctype>
#include "ParkingFeeStrategy.h"
#include "DurationType.h"

class BasicHourlyRateStrategy : public ParkingFeeStrategy {
private:
    // Utility function: lowercase a string (like Java's toLowerCase)
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
            hourlyRate = 10.0;
        }
        else if (type == "bike") {
            hourlyRate = 5.0;
        }
        else if (type == "auto") {
            hourlyRate = 8.0;
        }
        else {
            hourlyRate = 15.0; // default
        }

        if (durationType == DurationType::HOURS) {
            return duration * hourlyRate;
        }
        else {
            return duration * hourlyRate * 24.0; // daily rate
        }
    }
};
