#pragma once

#include <string>
#include <memory>
#include <iostream>
#include "Vehicle.h"
#include "ParkingFeeStrategy.h"

class OtherVehicle : public Vehicle {
public:
    OtherVehicle(const std::string& licensePlate,
        const std::string& vehicleType,
        std::shared_ptr<ParkingFeeStrategy> feeStrategy)
        : Vehicle(licensePlate, vehicleType, feeStrategy) {
    }

    // Implement required abstract method
    void displayInfo() const override {
        std::cout << "Other Vehicle - License Plate: " << licensePlate
            << ", Type: " << vehicleType << std::endl;
    }
};
