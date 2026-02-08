#pragma once

#include <string>
#include <memory>
#include <stdexcept>
#include "Vehicle.h"

class ParkingSpot {
protected:
    int spotNumber;
    bool occupied;
    std::shared_ptr<Vehicle> vehicle;
    std::string spotType;

public:
    // Constructor
    ParkingSpot(int spotNumber, const std::string& spotType)
        : spotNumber(spotNumber), occupied(false), spotType(spotType) {
    }

    virtual ~ParkingSpot() = default;

    // Check if occupied
    bool isOccupied() const {
        return occupied;
    }

    // Abstract method to check whether a vehicle can park here
    virtual bool canParkVehicle(std::shared_ptr<Vehicle> vehicle) const = 0;

    // Park a vehicle
    void parkVehicle(std::shared_ptr<Vehicle> vehiclePtr) {
        if (occupied) {
            throw std::runtime_error("Spot is already occupied.");
        }

        if (!canParkVehicle(vehiclePtr)) {
            throw std::runtime_error(
                "This spot is not suitable for " + vehiclePtr->getVehicleType()
            );
        }

        vehicle = vehiclePtr;
        occupied = true;
    }

    // Vacate the spot
    void vacate() {
        if (!occupied) {
            throw std::runtime_error("Spot is already vacant.");
        }

        vehicle.reset();
        occupied = false;
    }

    // Getters
    int getSpotNumber() const {
        return spotNumber;
    }

    std::shared_ptr<Vehicle> getVehicle() const {
        return vehicle;
    }

    const std::string& getSpotType() const {
        return spotType;
    }
};
