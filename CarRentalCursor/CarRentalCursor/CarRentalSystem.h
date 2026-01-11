#ifndef CAR_RENTAL_SYSTEM_H
#define CAR_RENTAL_SYSTEM_H

#include "Store.h"
#include "User.h"
#include "Reservation.h"
#include "VehicleFactory.h"
#include "PaymentStrategy.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <ctime>
#include <mutex>

// Singleton Pattern for RentalSystem
// Ensures a single point of control for the entire rental operation
// Maintains consistency across all stores and operations
class CarRentalSystem {
private:
    static CarRentalSystem* instance;
    static std::mutex instanceMutex;

    std::unordered_map<std::string, std::shared_ptr<Store>> stores;
    std::unordered_map<std::string, std::shared_ptr<User>> users;
    std::unordered_map<std::string, std::shared_ptr<Reservation>> reservations;

    int reservationCounter;
    int paymentCounter;

    // Private constructor for Singleton
    CarRentalSystem() : reservationCounter(0), paymentCounter(0) {}

public:
    // Singleton instance getter (thread-safe)
    static CarRentalSystem* getInstance() {
        if (!instance) {
            std::lock_guard<std::mutex> lock(instanceMutex);
            if (!instance) {
                instance = new CarRentalSystem();
            }
        }
        return instance;
    }

    // Prevent copying and assignment (Singleton pattern)
    CarRentalSystem(const CarRentalSystem&) = delete;
    CarRentalSystem& operator=(const CarRentalSystem&) = delete;

    // Store operations
    void addStore(std::shared_ptr<Store> store) {
        stores[store->getLocation()] = store;
    }

    std::shared_ptr<Store> getStore(const std::string& location) {
        auto it = stores.find(location);
        return (it != stores.end()) ? it->second : nullptr;
    }

    // User operations
    void addUser(std::shared_ptr<User> user) {
        users[user->getUserId()] = user;
    }

    std::shared_ptr<User> getUser(const std::string& userId) {
        auto it = users.find(userId);
        return (it != users.end()) ? it->second : nullptr;
    }

    // Search vehicles - KEY FUNCTIONALITY
    std::vector<std::shared_ptr<Vehicle>> searchVehicles(
        const std::string& location,
        VehicleType type,
        time_t startTime,
        time_t endTime) {

        std::vector<std::shared_ptr<Vehicle>> available;

        auto storeIt = stores.find(location);
        if (storeIt == stores.end()) return available;

        auto vehicles = storeIt->second->getAvailableVehicles(type);

        // Filter out vehicles with conflicts
        for (const auto& vehicle : vehicles) {
            if (!hasConflict(vehicle->getRegNumber(), startTime, endTime)) {
                available.push_back(vehicle);
            }
        }

        return available;
    }

    // Search all available vehicles at location
    std::vector<std::shared_ptr<Vehicle>> searchAllVehicles(
        const std::string& location,
        time_t startTime,
        time_t endTime) {

        std::vector<std::shared_ptr<Vehicle>> available;

        auto storeIt = stores.find(location);
        if (storeIt == stores.end()) return available;

        auto vehicles = storeIt->second->getAllAvailableVehicles();

        // Filter out vehicles with conflicts
        for (const auto& vehicle : vehicles) {
            if (!hasConflict(vehicle->getRegNumber(), startTime, endTime)) {
                available.push_back(vehicle);
            }
        }

        return available;
    }

    // Create reservation with payment - Uses Factory and Strategy patterns
    std::shared_ptr<Reservation> createReservation(
        const std::string& userId,
        const std::string& vehicleReg,
        time_t startTime,
        time_t endTime,
        std::unique_ptr<PaymentStrategy> paymentStrategy = nullptr) {

        // Check conflicts
        if (hasConflict(vehicleReg, startTime, endTime)) {
            return nullptr;
        }

        // Find vehicle and calculate price
        auto vehicle = findVehicle(vehicleReg);
        if (!vehicle) return nullptr;

        int days = static_cast<int>((endTime - startTime) / (24 * 3600));
        if (days == 0) days = 1; // Minimum 1 day

        double cost = vehicle->calculatePrice(days);

        std::string resId = "RES" + std::to_string(++reservationCounter);
        auto reservation = std::make_shared<Reservation>(
            resId, userId, vehicleReg, startTime, endTime);

        reservation->setTotalCost(cost);
        
        // Step 1: Attach payment method if user provided one
        if (paymentStrategy) {
            // Generate unique payment ID
            paymentCounter++;
            std::string paymentId = "PAY" + std::to_string(paymentCounter);
            
            // Create payment object with selected strategy
            auto payment = std::make_unique<Payment>(paymentId, resId, std::move(paymentStrategy));
            
            // Link payment to reservation
            reservation->setPayment(std::move(payment));
        }

        // Step 2: Store reservation in system's database
        reservations[resId] = reservation;
        
        // Step 3: Mark vehicle as reserved (prevents double-booking)
        vehicle->setStatus(VehicleStatus::RESERVED);

        // Step 4: Return the created reservation
        return reservation;
    }

    // Process payment for existing reservation
    bool processReservationPayment(const std::string& reservationId) {
        auto reservation = getReservation(reservationId);
        if (!reservation) return false;

        return reservation->processPayment();
    }

    // Start rental (pickup)
    bool startRental(const std::string& reservationId) {
        auto reservation = getReservation(reservationId);
        if (!reservation) return false;

        if (reservation->startRental()) {
            auto vehicle = findVehicle(reservation->getVehicleRegNumber());
            if (vehicle) {
                vehicle->setStatus(VehicleStatus::RENTED);
            }
            return true;
        }
        return false;
    }

    // Cancel reservation
    bool cancelReservation(const std::string& reservationId) {
        auto it = reservations.find(reservationId);
        if (it == reservations.end()) return false;

        it->second->cancel();
        auto vehicle = findVehicle(it->second->getVehicleRegNumber());
        if (vehicle) {
            vehicle->setStatus(VehicleStatus::AVAILABLE);
        }
        return true;
    }

    // Get reservation
    std::shared_ptr<Reservation> getReservation(const std::string& reservationId) {
        auto it = reservations.find(reservationId);
        return (it != reservations.end()) ? it->second : nullptr;
    }

    // Complete rental (return)
    bool completeRental(const std::string& reservationId) {
        auto reservation = getReservation(reservationId);
        if (!reservation) return false;

        if (reservation->complete()) {
            auto vehicle = findVehicle(reservation->getVehicleRegNumber());
            if (vehicle) {
                vehicle->setStatus(VehicleStatus::AVAILABLE);
            }
            return true;
        }
        return false;
    }

    // Add vehicle using Factory Pattern
    bool addVehicleToStore(const std::string& storeLocation, VehicleType type,
                          const std::string& regNum, const std::string& model,
                          double baseRate) {
        auto store = getStore(storeLocation);
        if (!store) return false;

        try {
            auto vehicle = VehicleFactory::createVehicle(type, regNum, model, baseRate);
            store->addVehicle(vehicle);
            return true;
        } catch (const std::exception&) {
            return false;
        }
    }

private:
    // Conflict detection - CRITICAL LOGIC
    bool hasConflict(const std::string& vehicleReg, time_t start, time_t end) {
        for (const auto& pair : reservations) {
            if (pair.second->getVehicleRegNumber() == vehicleReg &&
                pair.second->hasConflict(start, end)) {
                return true;
            }
        }
        return false;
    }

    std::shared_ptr<Vehicle> findVehicle(const std::string& regNumber) {
        for (const auto& pair : stores) {
            auto vehicle = pair.second->getVehicle(regNumber);
            if (vehicle) return vehicle;
        }
        return nullptr;
    }
};

// Initialize static members
CarRentalSystem* CarRentalSystem::instance = nullptr;
std::mutex CarRentalSystem::instanceMutex;

#endif

