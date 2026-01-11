#pragma once
#include <string>
#include "DurationType.h"

class ParkingFeeStrategy {
public:
    virtual ~ParkingFeeStrategy() = default;

    /**
     * Calculate parking fee based on vehicle type and duration.
     *
     * @param vehicleType Type of vehicle being parked
     * @param duration Duration of parking (hours or days)
     * @param durationType Type of duration (HOURS or DAYS)
     * @return Calculated parking fee
     */
    virtual double calculateFee(
        const std::string& vehicleType,
        int duration,
        DurationType durationType
    ) const = 0;
};
