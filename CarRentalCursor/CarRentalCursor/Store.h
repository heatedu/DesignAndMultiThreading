#ifndef STORE_H
#define STORE_H

#include "Vehicle.h"
#include "VehicleFactory.h"
#include <unordered_map>
#include <vector>
#include <memory>

class Store {
private:
    std::string storeId;
    std::string location;
    std::string address;
    std::unordered_map<std::string, std::shared_ptr<Vehicle>> inventory;

public:
    Store(const std::string& id, const std::string& loc, const std::string& addr = "")
        : storeId(id), location(loc), address(addr) {}

    // Add vehicle to inventory
    void addVehicle(std::shared_ptr<Vehicle> vehicle) {
        inventory[vehicle->getRegNumber()] = vehicle;
    }

    // Create and add vehicle using Factory Pattern
    bool createAndAddVehicle(VehicleType type, const std::string& regNum,
                            const std::string& model, double baseRate) {
        try {
            auto vehicle = VehicleFactory::createVehicle(type, regNum, model, baseRate);
            addVehicle(vehicle);
            return true;
        } catch (const std::exception&) {
            // Invalid vehicle type or parameters
            return false;
        }
    }

    std::shared_ptr<Vehicle> getVehicle(const std::string& regNumber) {
        auto it = inventory.find(regNumber);
        return (it != inventory.end()) ? it->second : nullptr;
    }

    // Search available vehicles by type
    std::vector<std::shared_ptr<Vehicle>> getAvailableVehicles(VehicleType type) {
        std::vector<std::shared_ptr<Vehicle>> result;
        for (const auto& pair : inventory) {
            if (pair.second->getStatus() == VehicleStatus::AVAILABLE &&
                pair.second->getType() == type) {
                result.push_back(pair.second);
            }
        }
        return result;
    }

    std::vector<std::shared_ptr<Vehicle>> getAllAvailableVehicles() {
        std::vector<std::shared_ptr<Vehicle>> result;
        for (const auto& pair : inventory) {
            if (pair.second->getStatus() == VehicleStatus::AVAILABLE) {
                result.push_back(pair.second);
            }
        }
        return result;
    }

    // Get vehicles by status (using enum)
    std::vector<std::shared_ptr<Vehicle>> getVehiclesByStatus(VehicleStatus status) {
        std::vector<std::shared_ptr<Vehicle>> result;
        for (const auto& pair : inventory) {
            if (pair.second->getStatus() == status) {
                result.push_back(pair.second);
            }
        }
        return result;
    }

    size_t getInventoryCount() const { return inventory.size(); }
    std::string getLocation() const { return location; }
    std::string getStoreId() const { return storeId; }
    std::string getAddress() const { return address; }
};

#endif

