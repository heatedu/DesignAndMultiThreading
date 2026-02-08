#pragma once

#include <string>
#include <memory>
#include<iostream>
#include "Vehicle.h"
#include "ParkingFeeStrategy.h"

class CarVehicle : public Vehicle {
public:
    CarVehicle(const std::string& licensePlate,
        const std::string& vehicleType,
        std::shared_ptr<ParkingFeeStrategy> feeStrategy)
        : Vehicle(licensePlate, vehicleType, feeStrategy) {
    }

    // Implement the abstract method from Vehicle
    void displayInfo() const override {
        // You can customize this as needed
        std::cout << "Car - License Plate: " << licensePlate
            << ", Type: " << vehicleType << std::endl;
    }
};
