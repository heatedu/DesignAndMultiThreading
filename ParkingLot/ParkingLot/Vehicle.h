#pragma once

#include <string>
#include <memory>
#include "DurationType.h"
#include "ParkingFeeStrategy.h"

class Vehicle {
protected:
    std::string licensePlate;        // Vehicle's license plate
    std::string vehicleType;         // e.g., car, bike, auto
    std::shared_ptr<ParkingFeeStrategy> feeStrategy; // Strategy pattern

public:
    // Constructor
    Vehicle(const std::string& licensePlate,
        const std::string& vehicleType,
        std::shared_ptr<ParkingFeeStrategy> feeStrategy)
        : licensePlate(licensePlate),
        vehicleType(vehicleType),
        feeStrategy(feeStrategy) {
    }

    virtual ~Vehicle() = default;  // Required for abstract base class

    // Getters
    const std::string& getVehicleType() const {
        return vehicleType;
    }

    const std::string& getLicensePlate() const {
        return licensePlate;
    }

    // Calculate fee using strategy
    double calculateFee(int duration, DurationType durationType) const {
        return feeStrategy->calculateFee(vehicleType, duration, durationType);
    }

    // Pure virtual method → makes this an abstract class in C++
    virtual void displayInfo() const = 0;
};
