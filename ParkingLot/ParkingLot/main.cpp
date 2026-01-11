//ToDo: Extend ParkingLot to multiple floors using builder pattern

#include <iostream>
#include <vector>
#include <memory>

#include "ParkingLot.h"
#include "CarParkingSpot.h"
#include "BikeParkingSpot.h"

#include "BasicHourlyRateStrategy.h"
#include "PremiumRateStrategy.h"

#include "VehicleFactory.h"
#include "Payment.h"
#include "CreditCardPaymentStrategy.h"
#include "CashPaymentStrategy.h"
#include "DurationType.h"

// Helper function like Java getPaymentStrategy()
std::shared_ptr<PaymentStrategy> getPaymentStrategy(int paymentMethod, double fee) {
    switch (paymentMethod) {
    case 1:
        return std::make_shared<CreditCardPayment>(fee);
    case 2:
        return std::make_shared<CashPayment>(fee);
    default:
        std::cout << "Invalid choice! Defaulting to Credit Card payment.\n";
        return std::make_shared<CreditCardPayment>(fee);
    }
}

int main() {
    // Initialize parking spots
    std::vector<std::shared_ptr<ParkingSpot>> parkingSpots;
    parkingSpots.push_back(std::make_shared<CarParkingSpot>(1, "Car"));
    parkingSpots.push_back(std::make_shared<CarParkingSpot>(2, "Car"));
    parkingSpots.push_back(std::make_shared<BikeParkingSpot>(3, "Bike"));
    parkingSpots.push_back(std::make_shared<BikeParkingSpot>(4, "Bike"));

    // Initialize parking lot
    ParkingLot parkingLot(parkingSpots);

    // Fee strategies
    auto basicHourlyRateStrategy = std::make_shared<BasicHourlyRateStrategy>();
    auto premiumRateStrategy = std::make_shared<PremiumRateStrategy>();

    // Vehicles using Factory + Fee strategies
    auto car1 = VehicleFactory::createVehicle("Car", "CAR123", basicHourlyRateStrategy);
    auto car2 = VehicleFactory::createVehicle("Car", "CAR345", basicHourlyRateStrategy);
    auto bike1 = VehicleFactory::createVehicle("Bike", "BIKE456", premiumRateStrategy);
    auto bike2 = VehicleFactory::createVehicle("Bike", "BIKE123", premiumRateStrategy);

    // Try parking the vehicles
    auto carSpot = parkingLot.parkVehicle(car1);
    auto bikeSpot = parkingLot.parkVehicle(bike1);

    // Ask user for payment choice
    int paymentMethod;
    std::cout << "Select payment method for your vehicle:\n";
    std::cout << "1. Credit Card\n";
    std::cout << "2. Cash\n";
    std::cin >> paymentMethod;

    // Process Car Payment
    if (carSpot != nullptr) {
        double fee = car1->calculateFee(2, DurationType::HOURS);
        auto payStrategy = getPaymentStrategy(paymentMethod, fee);

        payStrategy->processPayment(fee);
        parkingLot.vacateSpot(carSpot, car1);
    }

    // Process Bike Payment
    if (bikeSpot != nullptr) {
        double fee = bike1->calculateFee(3, DurationType::HOURS);
        auto payStrategy = getPaymentStrategy(paymentMethod, fee);

        payStrategy->processPayment(fee);
        parkingLot.vacateSpot(bikeSpot, bike1);
    }

    return 0;
}

