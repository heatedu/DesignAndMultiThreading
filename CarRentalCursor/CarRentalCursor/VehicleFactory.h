#ifndef VEHICLE_FACTORY_H
#define VEHICLE_FACTORY_H

#include "Vehicle.h"
#include <memory>
#include <stdexcept>

// Factory Pattern for Vehicle Creation
// Encapsulates vehicle creation logic and allows easy addition of new vehicle types
class VehicleFactory {
public:
    // Static factory method to create vehicles based on type
    static std::shared_ptr<Vehicle> createVehicle(
        VehicleType type,
        const std::string& regNum,
        const std::string& model,
        const std::string& make,
        int year,
        double baseRate) {
        
        switch (type) {
            case VehicleType::ECONOMY:
                return std::make_shared<EconomyVehicle>(regNum, model, baseRate);
                
            case VehicleType::LUXURY:
                return std::make_shared<LuxuryVehicle>(regNum, model, baseRate);
                
            case VehicleType::SUV:
                return std::make_shared<SUVVehicle>(regNum, model, baseRate);
                
            default:
                throw std::invalid_argument("Unknown vehicle type");
        }
    }

    // Overloaded method with just essential parameters
    static std::shared_ptr<Vehicle> createVehicle(
        VehicleType type,
        const std::string& regNum,
        const std::string& model,
        double baseRate) {
        
        return createVehicle(type, regNum, model, "", 2023, baseRate);
    }

    // Method to validate vehicle creation parameters
    static bool validateVehicleParams(const std::string& regNum, double baseRate) {
        return !regNum.empty() && baseRate > 0;
    }
};

#endif

