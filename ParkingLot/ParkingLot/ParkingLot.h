#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <string>

#include "ParkingSpot.h"
#include "Vehicle.h"

class ParkingLot {
private:
    std::vector<std::shared_ptr<ParkingSpot>> parkingSpots;

public:
    // Constructor
    ParkingLot(const std::vector<std::shared_ptr<ParkingSpot>>& spots)
        : parkingSpots(spots) {
    }

    // Find an available spot based on vehicle type
    std::shared_ptr<ParkingSpot> findAvailableSpot(const std::string& vehicleType) {
        for (auto& spot : parkingSpots) {
            if (!spot->isOccupied() && spot->getSpotType() == vehicleType) {
                return spot;
            }
        }
        return nullptr; // No available spot found
    }

    // Park a vehicle
    std::shared_ptr<ParkingSpot> parkVehicle(std::shared_ptr<Vehicle> vehicle) {
        auto spot = findAvailableSpot(vehicle->getVehicleType());
        if (spot != nullptr) {
            spot->parkVehicle(vehicle);
            std::cout << "Vehicle parked successfully in spot: "
                << spot->getSpotNumber() << std::endl;
            return spot;
        }
        std::cout << "No parking spots available for "
            << vehicle->getVehicleType() << "!" << std::endl;
        return nullptr;
    }

    // Vacate a parking spot
    void vacateSpot(std::shared_ptr<ParkingSpot> spot,
        std::shared_ptr<Vehicle> vehicle) {

        if (spot != nullptr &&
            spot->isOccupied() &&
            spot->getVehicle() == vehicle) {

            spot->vacate();
            std::cout << vehicle->getVehicleType()
                << " vacated the spot: "
                << spot->getSpotNumber() << std::endl;
        }
        else {
            std::cout << "Invalid operation! Either the spot is already "
                "vacant or the vehicle does not match."
                << std::endl;
        }
    }

    // Find a spot by number
    std::shared_ptr<ParkingSpot> getSpotByNumber(int spotNumber) {
        for (auto& spot : parkingSpots) {
            if (spot->getSpotNumber() == spotNumber) {
                return spot;
            }
        }
        return nullptr;
    }

    // Getter for parking spots
    const std::vector<std::shared_ptr<ParkingSpot>>& getParkingSpots() const {
        return parkingSpots;
    }
};
