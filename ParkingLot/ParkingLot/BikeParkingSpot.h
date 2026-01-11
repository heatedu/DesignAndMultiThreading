#pragma once

#include <string>
#include <algorithm>
#include <memory>
#include "ParkingSpot.h"
#include "Vehicle.h"

class BikeParkingSpot : public ParkingSpot {
public:
    BikeParkingSpot(int spotNumber, const std::string& spotType)
        : ParkingSpot(spotNumber, spotType) {
    }

    bool canParkVehicle(std::shared_ptr<Vehicle> vehicle) const override {
        std::string type = vehicle->getVehicleType();
        std::transform(type.begin(), type.end(), type.begin(),
            [](unsigned char c) { return std::tolower(c); });

        return type == "bike";
    }
};
