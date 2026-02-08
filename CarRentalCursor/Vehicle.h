#ifndef VEHICLE_H
#define VEHICLE_H

#include "VehicleTypes.h"
#include <string>

class Vehicle {
protected:
    std::string registrationNumber;
    std::string model;
    VehicleType type;
    double dailyRate;
    VehicleStatus status;

public:
    Vehicle(const std::string& regNum, const std::string& model,
        VehicleType type, double rate)
        : registrationNumber(regNum), model(model), type(type),
        dailyRate(rate), status(VehicleStatus::AVAILABLE) {}

    virtual ~Vehicle() = default;

    // Polymorphic pricing
    virtual double calculatePrice(int days) const = 0;

    // Getters
    std::string getRegNumber() const { return registrationNumber; }
    VehicleType getType() const { return type; }
    VehicleStatus getStatus() const { return status; }
    double getDailyRate() const { return dailyRate; }
    std::string getModel() const { return model; }

    void setStatus(VehicleStatus newStatus) { status = newStatus; }
};

class EconomyVehicle : public Vehicle {
public:
    EconomyVehicle(const std::string& regNum, const std::string& model, double rate)
        : Vehicle(regNum, model, VehicleType::ECONOMY, rate) {}

    double calculatePrice(int days) const override {
        return dailyRate * days;
    }
};

class LuxuryVehicle : public Vehicle {
public:
    LuxuryVehicle(const std::string& regNum, const std::string& model, double rate)
        : Vehicle(regNum, model, VehicleType::LUXURY, rate) {}

    double calculatePrice(int days) const override {
        return dailyRate * days * 1.5; // 50% premium
    }
};

class SUVVehicle : public Vehicle {
public:
    SUVVehicle(const std::string& regNum, const std::string& model, double rate)
        : Vehicle(regNum, model, VehicleType::SUV, rate) {}

    double calculatePrice(int days) const override {
        return dailyRate * days * 1.3; // 30% premium
    }
};

#endif

