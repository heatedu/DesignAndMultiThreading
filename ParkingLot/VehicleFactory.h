#pragma once

#include <string>
#include <memory>
#include <algorithm>

#include "CarVehicle.h"
#include "BikeVehicle.h"
#include "OtherVehicle.h"
#include "ParkingFeeStrategy.h"

class VehicleFactory {
private:
    // helper to lowercase (like Java's equalsIgnoreCase)
    static std::string toLower(const std::string& s) {
        std::string result = s;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return result;
    }

public:
    static std::shared_ptr<Vehicle> createVehicle(
        const std::string& vehicleType,
        const std::string& licensePlate,
        std::shared_ptr<ParkingFeeStrategy> feeStrategy)
    {
        std::string type = toLower(vehicleType);

        if (type == "car") {
            return std::make_shared<CarVehicle>(licensePlate, vehicleType, feeStrategy);
        }
        else if (type == "bike") {
            return std::make_shared<BikeVehicle>(licensePlate, vehicleType, feeStrategy);
        }

        // Default: Other vehicle
        return std::make_shared<OtherVehicle>(licensePlate, vehicleType, feeStrategy);
    }
};
