#include "CarRentalSystem.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

void printSeparator() {
    cout << "\n" << string(70, '=') << "\n\n";
}

void printVehicle(const shared_ptr<Vehicle>& vehicle, int days = 1) {
    cout << "  - " << vehicle->getModel()
        << " (" << vehicle->getRegNumber() << ")"
        << " | Rate: Rs." << vehicle->getDailyRate() << "/day"
        << " | " << days << " days: Rs." << vehicle->calculatePrice(days)
        << "\n";
}

int main() {
    // Seed for random transaction IDs
    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "*** CAR RENTAL SYSTEM - DESIGN PATTERNS DEMO ***\n";
    cout << "    Demonstrating: Singleton, Factory, Strategy Patterns\n";
    printSeparator();

    // ========== SINGLETON PATTERN ==========
    cout << "1. SINGLETON PATTERN - Creating system instance...\n";
    auto system = CarRentalSystem::getInstance();
    auto system2 = CarRentalSystem::getInstance();
    cout << "   System instance 1: " << system << "\n";
    cout << "   System instance 2: " << system2 << "\n";
    cout << "   Both instances are same (Singleton verified)\n";

    // ========== SETUP STORES ==========
    printSeparator();
    cout << "2. Setting up stores...\n";
    auto mumbaiStore = make_shared<Store>("S1", "Mumbai", "Andheri East");
    auto delhiStore = make_shared<Store>("S2", "Delhi", "Connaught Place");
    system->addStore(mumbaiStore);
    system->addStore(delhiStore);
    cout << "   Created stores: Mumbai, Delhi\n";

    // ========== FACTORY PATTERN ==========
    printSeparator();
    cout << "3. FACTORY PATTERN - Creating vehicles...\n\n";
    
    cout << "   Using VehicleFactory to create vehicles:\n";
    
    // Create vehicles using Factory Pattern
    auto swift = VehicleFactory::createVehicle(VehicleType::ECONOMY, "MH01AB1234", "Swift", 50);
    auto i10 = VehicleFactory::createVehicle(VehicleType::ECONOMY, "MH01AB5678", "i10", 45);
    auto merc = VehicleFactory::createVehicle(VehicleType::LUXURY, "MH01CD1234", "Mercedes S-Class", 300);
    auto fortuner = VehicleFactory::createVehicle(VehicleType::SUV, "MH01EF1234", "Fortuner", 120);
    auto innova = VehicleFactory::createVehicle(VehicleType::SUV, "MH01EF5678", "Innova Crysta", 100);
    
    cout << "    Created Economy: " << swift->getModel() << "\n";
    cout << "    Created Economy: " << i10->getModel() << "\n";
    cout << "    Created Luxury: " << merc->getModel() << "\n";
    cout << "    Created SUV: " << fortuner->getModel() << "\n";
    cout << "    Created SUV: " << innova->getModel() << "\n";
    
    // Add to stores
    mumbaiStore->addVehicle(swift);
    mumbaiStore->addVehicle(i10);
    mumbaiStore->addVehicle(merc);
    mumbaiStore->addVehicle(fortuner);
    mumbaiStore->addVehicle(innova);
    
    // Using Factory method directly on Store
    cout << "\n   Using Store's factory method:\n";
    delhiStore->createAndAddVehicle(VehicleType::ECONOMY, "DL01AB1234", "Alto", 40);
    delhiStore->createAndAddVehicle(VehicleType::LUXURY, "DL01CD1234", "BMW 7 Series", 350);
    cout << "    Created and added vehicles to Delhi store\n";
    
    cout << "\n   Total: 5 vehicles in Mumbai, 2 vehicles in Delhi\n";

    // ========== ADD USERS ==========
    printSeparator();
    cout << "4. Registering users...\n";
    
    auto user1 = make_shared<User>("U001", "Rahul Sharma", "DL123456");
    auto user2 = make_shared<User>("U002", "Priya Patel", "DL789012");
    auto user3 = make_shared<User>("U003", "Amit Kumar", "DL345678");
    system->addUser(user1);
    system->addUser(user2);
    system->addUser(user3);
    
    cout << "    Registered: " << user1->getName() << " (ID: " << user1->getUserId() << ")\n";
    cout << "    Registered: " << user2->getName() << " (ID: " << user2->getUserId() << ")\n";
    cout << "    Registered: " << user3->getName() << " (ID: " << user3->getUserId() << ")\n";

    // ========== SEARCH VEHICLES ==========
    printSeparator();
    cout << "5. Searching available vehicles...\n\n";

    time_t now = time(nullptr);
    time_t start = now + 86400;        // Tomorrow
    time_t end = start + 3 * 86400;    // 3 days rental

    cout << "   Search: Mumbai | Economy | 3 days\n\n";
    auto economyCars = system->searchVehicles("Mumbai", VehicleType::ECONOMY, start, end);
    cout << "   Found " << economyCars.size() << " economy vehicles:\n";
    for (const auto& car : economyCars) {
        printVehicle(car, 3);
    }

    cout << "\n   Search: Mumbai | SUV | 3 days\n\n";
    auto suvCars = system->searchVehicles("Mumbai", VehicleType::SUV, start, end);
    cout << "   Found " << suvCars.size() << " SUV vehicles:\n";
    for (const auto& car : suvCars) {
        printVehicle(car, 3);
    }

    // ========== STRATEGY PATTERN - PAYMENT ==========
    printSeparator();
    cout << "6. STRATEGY PATTERN - Creating reservations with different payments...\n";

    // Reservation 1: Credit Card Payment
    if (!economyCars.empty()) {
        cout << "\n   Reservation 1: Economy car with CREDIT CARD payment\n";
        auto selectedCar = economyCars[0];
        
        auto creditCardStrategy = make_unique<CreditCardPayment>(
            "1234567890123456", "123", "12/25", "Rahul Sharma");
        
        auto reservation1 = system->createReservation(
            user1->getUserId(),
            selectedCar->getRegNumber(),
            start,
            end,
            std::move(creditCardStrategy)
        );

        if (reservation1) {
            cout << "\n    Reservation created: " << reservation1->getReservationId() << "\n";
            cout << "   Status: " << reservation1->getStatusString() << "\n";
            cout << "   Vehicle: " << selectedCar->getModel() << "\n";
            cout << "   Cost: Rs." << fixed << setprecision(2) << reservation1->getTotalCost() << "\n";
            
            // Process payment
            cout << "\n   Processing payment...\n";
            if (system->processReservationPayment(reservation1->getReservationId())) {
                cout << "    Payment successful!\n";
                cout << "   Status updated: " << reservation1->getStatusString() << "\n";
            }
        }
    }

    // Reservation 2: UPI Payment
    if (!economyCars.empty() && economyCars.size() > 1) {
        printSeparator();
        cout << "   Reservation 2: Economy car with UPI payment\n";
        auto selectedCar = economyCars[1];
        
        auto upiStrategy = make_unique<UPIPayment>("priya@upi", "PhonePe");
        
        auto reservation2 = system->createReservation(
            user2->getUserId(),
            selectedCar->getRegNumber(),
            start,
            end,
            std::move(upiStrategy)
        );

        if (reservation2) {
            cout << "\n    Reservation created: " << reservation2->getReservationId() << "\n";
            cout << "   Status: " << reservation2->getStatusString() << "\n";
            cout << "   Vehicle: " << selectedCar->getModel() << "\n";
            cout << "   Cost: Rs." << fixed << setprecision(2) << reservation2->getTotalCost() << "\n";
            
            cout << "\n   Processing payment...\n";
            if (system->processReservationPayment(reservation2->getReservationId())) {
                cout << "    Payment successful!\n";
                cout << "   Status updated: " << reservation2->getStatusString() << "\n";
            }
        }
    }

    // Reservation 3: Cash Payment for SUV
    if (!suvCars.empty()) {
        printSeparator();
        cout << "   Reservation 3: SUV with CASH payment\n";
        
        time_t longStart = now + 10 * 86400;
        time_t longEnd = longStart + 7 * 86400;
        
        auto selectedSuv = suvCars[0];
        auto cashStrategy = make_unique<CashPayment>("Counter-Mumbai");
        
        auto reservation3 = system->createReservation(
            user3->getUserId(),
            selectedSuv->getRegNumber(),
            longStart,
            longEnd,
            std::move(cashStrategy)
        );

        if (reservation3) {
            cout << "\n    Reservation created: " << reservation3->getReservationId() << "\n";
            cout << "   Status: " << reservation3->getStatusString() << "\n";
            cout << "   Vehicle: " << selectedSuv->getModel() << "\n";
            cout << "   Duration: 7 days\n";
            cout << "   Cost: Rs." << fixed << setprecision(2) << reservation3->getTotalCost() << "\n";
            
            cout << "\n   Processing payment...\n";
            if (system->processReservationPayment(reservation3->getReservationId())) {
                cout << "    Payment successful!\n";
                cout << "   Status updated: " << reservation3->getStatusString() << "\n";
            }
        }
    }

    // ========== TEST CONFLICT PREVENTION ==========
    printSeparator();
    cout << "7. Testing conflict prevention...\n\n";

    if (!economyCars.empty()) {
        cout << "   Attempting to book same vehicle for overlapping dates...\n";
        cout << "   Vehicle: " << economyCars[0]->getRegNumber() << "\n";
        
        auto conflictRes = system->createReservation(
            user3->getUserId(),
            economyCars[0]->getRegNumber(),
            start + 86400,  // 1 day later (overlaps)
            end + 86400,
            make_unique<UPIPayment>("amit@upi", "GooglePay")
        );

        if (conflictRes) {
            cout << "   ✗ ERROR: Conflict not detected!\n";
        }
        else {
            cout << "    SUCCESS: Booking rejected due to conflict (as expected)\n";
        }
    }

    // ========== STATE MANAGEMENT WITH ENUMS ==========
    printSeparator();
    cout << "8. STATE MANAGEMENT - Reservation lifecycle with enums...\n\n";

    auto res = system->getReservation("RES1");
    if (res) {
        cout << "   Reservation: " << res->getReservationId() << "\n";
        cout << "   Current State: " << res->getStatusString() << "\n\n";
        
        // State transition: Start rental (pickup)
        cout << "   → Starting rental (pickup)...\n";
        if (system->startRental("RES1")) {
            cout << "    State changed to: " << res->getStatusString() << "\n\n";
        }
        
        // State transition: Complete rental (return)
        cout << "   → Completing rental (return)...\n";
        if (system->completeRental("RES1")) {
            cout << "    State changed to: " << res->getStatusString() << "\n";
        }
    }

    // ========== CANCEL RESERVATION ==========
    printSeparator();
    cout << "9. Testing cancellation (state transition)...\n\n";

    auto res2 = system->getReservation("RES2");
    if (res2) {
        cout << "   Reservation: " << res2->getReservationId() << "\n";
        cout << "   Current State: " << res2->getStatusString() << "\n";
        
        cout << "   → Cancelling reservation...\n";
        if (system->cancelReservation("RES2")) {
            cout << "    State changed to: " << res2->getStatusString() << "\n";
            cout << "    Vehicle is now available again\n";
        }
    }

    // ========== SEARCH AFTER STATE CHANGES ==========
    printSeparator();
    cout << "10. Vehicle availability after state changes...\n\n";

    auto availableNow = system->searchVehicles("Mumbai", VehicleType::ECONOMY, start, end);
    cout << "   Available economy cars now: " << availableNow.size() << "\n";
    for (const auto& car : availableNow) {
        cout << "     - " << car->getModel() << " (" << car->getRegNumber() << ")\n";
    }

    // ========== SUMMARY ==========
    printSeparator();
    cout << "*** DEMO COMPLETED SUCCESSFULLY ***\n\n";
    cout << "Design Patterns Demonstrated:\n";
    cout << "   SINGLETON    - CarRentalSystem single instance\n";
    cout << "   FACTORY      - VehicleFactory for creating vehicles\n";
    cout << "   STRATEGY     - Multiple payment strategies (Credit/Debit/UPI/Cash)\n";
    cout << "   STATE        - Reservation lifecycle with enums\n";
    cout << "   TEMPLATE     - Vehicle hierarchy with polymorphic pricing\n";
    printSeparator();

    return 0;
}

