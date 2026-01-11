// CarRentalSystem.cpp
#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <map>
#include <chrono>
#include <ctime>
#include <stdexcept>
#include <iomanip>
#include <sstream>

// --- Enums ---
enum class VehicleType { ECONOMY, LUXURY, SUV, BIKE, AUTO, VAN };
enum class VehicleStatus { AVAILABLE, RESERVED, RENTED, MAINTENANCE, OUT_OF_SERVICE };
enum class ReservationStatus { PENDING, CONFIRMED, IN_PROGRESS, COMPLETED, CANCELED };

// --- Helper: days between two time_points (inclusive) ---
inline int daysBetweenInclusive(const std::chrono::system_clock::time_point& start,
    const std::chrono::system_clock::time_point& end) {
    using namespace std::chrono;
    if (end < start) return 0;
    auto diff = duration_cast<hours>(end - start).count();
    int days = static_cast<int>(diff / 24) + 1; // inclusive
    return days;
}

// --- Base Vehicle ---
class Vehicle {
protected:
    std::string registrationNumber;
    std::string model;
    VehicleType type;
    VehicleStatus status;
    double baseRentalPrice;

public:
    Vehicle(std::string reg, std::string mdl, VehicleType t, double basePrice)
        : registrationNumber(std::move(reg)),
        model(std::move(mdl)),
        type(t),
        status(VehicleStatus::AVAILABLE),
        baseRentalPrice(basePrice) {
    }

    virtual ~Vehicle() = default;
    virtual double calculateRentalFee(int days) const = 0;

    // getters / setters
    const std::string& getRegistrationNumber() const { return registrationNumber; }
    const std::string& getModel() const { return model; }
    VehicleType getType() const { return type; }
    VehicleStatus getStatus() const { return status; }
    void setStatus(VehicleStatus s) { status = s; }
    double getBaseRentalPrice() const { return baseRentalPrice; }
};

// --- Concrete Vehicles ---
class EconomyVehicle : public Vehicle {
    static constexpr double RATE_MULTIPLIER = 1.0;
public:
    EconomyVehicle(const std::string& reg, const std::string& mdl, double basePrice)
        : Vehicle(reg, mdl, VehicleType::ECONOMY, basePrice) {
    }
    double calculateRentalFee(int days) const override {
        return getBaseRentalPrice() * days * RATE_MULTIPLIER;
    }
};

class LuxuryVehicle : public Vehicle {
    static constexpr double RATE_MULTIPLIER = 2.5;
    static constexpr double PREMIUM_FEE = 50.0;
public:
    LuxuryVehicle(const std::string& reg, const std::string& mdl, double basePrice)
        : Vehicle(reg, mdl, VehicleType::LUXURY, basePrice) {
    }
    double calculateRentalFee(int days) const override {
        return (getBaseRentalPrice() * days * RATE_MULTIPLIER) + PREMIUM_FEE;
    }
};

class SUVVehicle : public Vehicle {
    static constexpr double RATE_MULTIPLIER = 1.5;
public:
    SUVVehicle(const std::string& reg, const std::string& mdl, double basePrice)
        : Vehicle(reg, mdl, VehicleType::SUV, basePrice) {
    }
    double calculateRentalFee(int days) const override {
        return getBaseRentalPrice() * days * RATE_MULTIPLIER;
    }
};

class BikeVehicle : public Vehicle {
    static constexpr double RATE_MULTIPLIER = 0.5;
public:
    BikeVehicle(const std::string& reg, const std::string& mdl, double basePrice)
        : Vehicle(reg, mdl, VehicleType::BIKE, basePrice) {
    }
    double calculateRentalFee(int days) const override {
        return getBaseRentalPrice() * days * RATE_MULTIPLIER;
    }
};

class AutoVehicle : public Vehicle {
    static constexpr double RATE_MULTIPLIER = 1.2;
public:
    AutoVehicle(const std::string& reg, const std::string& mdl, double basePrice)
        : Vehicle(reg, mdl, VehicleType::AUTO, basePrice) {
    }
    double calculateRentalFee(int days) const override {
        return getBaseRentalPrice() * days * RATE_MULTIPLIER;
    }
};

class VanVehicle : public Vehicle {
    static constexpr double RATE_MULTIPLIER = 1.7;
public:
    VanVehicle(const std::string& reg, const std::string& mdl, double basePrice)
        : Vehicle(reg, mdl, VehicleType::VAN, basePrice) {
    }
    double calculateRentalFee(int days) const override {
        return getBaseRentalPrice() * days * RATE_MULTIPLIER;
    }
};

// --- Vehicle Factory ---
class VehicleFactory {
public:
    static std::unique_ptr<Vehicle> createVehicle(VehicleType vt, const std::string& reg,
        const std::string& model, double basePrice) {
        switch (vt) {
        case VehicleType::ECONOMY:
            return std::make_unique<EconomyVehicle>(reg, model, basePrice);
        case VehicleType::LUXURY:
            return std::make_unique<LuxuryVehicle>(reg, model, basePrice);
        case VehicleType::SUV:
            return std::make_unique<SUVVehicle>(reg, model, basePrice);
        case VehicleType::BIKE:
            return std::make_unique<BikeVehicle>(reg, model, basePrice);
        case VehicleType::AUTO:
            return std::make_unique<AutoVehicle>(reg, model, basePrice);
        case VehicleType::VAN:
            return std::make_unique<VanVehicle>(reg, model, basePrice);
        default:
            throw std::invalid_argument("Unsupported vehicle type");
        }
    }
};

// --- Location ---
struct Location {
    std::string address;
    std::string city;
    std::string state;
    std::string zipCode;
    Location(std::string a, std::string c, std::string s, std::string z)
        : address(std::move(a)), city(std::move(c)), state(std::move(s)), zipCode(std::move(z)) {
    }
};

// --- RentalStore ---
class RentalStore {
    int id;
    std::string name;
    Location location;
    std::unordered_map<std::string, std::shared_ptr<Vehicle>> vehicles; // reg -> vehicle
public:
    RentalStore(int id_, std::string name_, Location loc) : id(id_), name(std::move(name_)), location(std::move(loc)) {}

    int getId() const { return id; }
    const std::string& getName() const { return name; }

    void addVehicle(const std::shared_ptr<Vehicle>& v) {
        vehicles[v->getRegistrationNumber()] = v;
    }

    void removeVehicle(const std::string& reg) {
        vehicles.erase(reg);
    }

    std::shared_ptr<Vehicle> getVehicle(const std::string& reg) {
        auto it = vehicles.find(reg);
        return (it == vehicles.end()) ? nullptr : it->second;
    }

    std::vector<std::shared_ptr<Vehicle>> getAvailableVehicles() const {
        std::vector<std::shared_ptr<Vehicle>> out;
        for (auto& p : vehicles) {
            if (p.second->getStatus() == VehicleStatus::AVAILABLE) out.push_back(p.second);
        }
        return out;
    }
};

// --- Forward declare ---
class Reservation;

// --- User ---
class User {
    int id;
    std::string name;
    std::string email;
    std::vector<std::shared_ptr<Reservation>> reservations;
public:
    User(int id_, std::string name_, std::string email_) : id(id_), name(std::move(name_)), email(std::move(email_)) {}
    int getId() const { return id; }
    const std::string& getName() const { return name; }
    void addReservation(const std::shared_ptr<Reservation>& r) { reservations.push_back(r); }
    void deleteReservation(int reservationId);
};

// --- Reservation ---
class Reservation : public std::enable_shared_from_this<Reservation> {
    int id;
    std::shared_ptr<User> user;
    std::shared_ptr<Vehicle> vehicle;
    std::shared_ptr<RentalStore> pickupStore;
    std::shared_ptr<RentalStore> returnStore;
    std::chrono::system_clock::time_point startDate;
    std::chrono::system_clock::time_point endDate;
    ReservationStatus status;
    double totalAmount;

public:
    Reservation(int id_,
        std::shared_ptr<User> u,
        std::shared_ptr<Vehicle> v,
        std::shared_ptr<RentalStore> pickup,
        std::shared_ptr<RentalStore> ret,
        std::chrono::system_clock::time_point start,
        std::chrono::system_clock::time_point end)
        : id(id_), user(std::move(u)), vehicle(std::move(v)), pickupStore(std::move(pickup)),
        returnStore(std::move(ret)), startDate(start), endDate(end), status(ReservationStatus::PENDING)
    {
        int days = daysBetweenInclusive(startDate, endDate);
        totalAmount = vehicle->calculateRentalFee(days);
    }

    int getId() const { return id; }
    double getTotalAmount() const { return totalAmount; }
    ReservationStatus getStatus() const { return status; }

    void confirmReservation() {
        if (status == ReservationStatus::PENDING) {
            status = ReservationStatus::CONFIRMED;
            vehicle->setStatus(VehicleStatus::RESERVED);
        }
    }
    void startRental() {
        if (status == ReservationStatus::CONFIRMED) {
            status = ReservationStatus::IN_PROGRESS;
            vehicle->setStatus(VehicleStatus::RENTED);
        }
    }
    void completeRental() {
        if (status == ReservationStatus::IN_PROGRESS) {
            status = ReservationStatus::COMPLETED;
            vehicle->setStatus(VehicleStatus::AVAILABLE);
        }
    }
    void cancelReservation() {
        if (status == ReservationStatus::PENDING || status == ReservationStatus::CONFIRMED) {
            status = ReservationStatus::CANCELED;
            vehicle->setStatus(VehicleStatus::AVAILABLE);
        }
    }
};

// Implement deletion for User (simple remove by id)
void User::deleteReservation(int reservationId) {
    for (auto it = reservations.begin(); it != reservations.end(); ++it) {
        if ((*it)->getId() == reservationId) {
            reservations.erase(it);
            break;
        }
    }
}

// --- Reservation Manager ---
class ReservationManager {
    std::map<int, std::shared_ptr<Reservation>> reservations;
    int nextReservationId = 1;
public:
    std::shared_ptr<Reservation> createReservation(std::shared_ptr<User> user,
        std::shared_ptr<Vehicle> vehicle,
        std::shared_ptr<RentalStore> pickupStore,
        std::shared_ptr<RentalStore> returnStore,
        std::chrono::system_clock::time_point startDate,
        std::chrono::system_clock::time_point endDate) {
        int id = nextReservationId++;
        auto r = std::make_shared<Reservation>(id, user, vehicle, pickupStore, returnStore, startDate, endDate);
        reservations[id] = r;
        user->addReservation(r);
        return r;
    }

    std::shared_ptr<Reservation> getReservation(int id) {
        auto it = reservations.find(id);
        return (it == reservations.end()) ? nullptr : it->second;
    }

    void confirmReservation(int id) {
        if (auto r = getReservation(id)) r->confirmReservation();
    }
    void startRental(int id) {
        if (auto r = getReservation(id)) r->startRental();
    }
    void completeRental(int id) {
        if (auto r = getReservation(id)) r->completeRental();
    }
    void cancelReservation(int id) {
        if (auto r = getReservation(id)) r->cancelReservation();
    }
};

// --- Payment Strategy ---
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual bool processPayment(double amount) = 0;
};

class CreditCardPayment : public PaymentStrategy {
    std::string cardNumber;
    std::string name;
    std::string expiry;
public:
    CreditCardPayment(std::string c, std::string n, std::string e) : cardNumber(std::move(c)), name(std::move(n)), expiry(std::move(e)) {}
    static std::string maskCard(const std::string& card) {
        if (card.size() <= 4) return card;
        std::string out(card.size() - 4, 'X');
        out += card.substr(card.size() - 4);
        return out;
    }
    bool processPayment(double amount) override {
        std::cout << "Processing credit card payment of $" << amount << "\n";
        std::cout << "Card: " << maskCard(cardNumber) << ", Name: " << name << ", Expiry: " << expiry << "\n";
        // simulate success
        return true;
    }
};

class CashPayment : public PaymentStrategy {
public:
    bool processPayment(double amount) override {
        std::cout << "Processing cash payment of $" << amount << "\n";
        return true;
    }
};

class PayPalPayment : public PaymentStrategy {
    std::string email;
public:
    PayPalPayment(std::string e) : email(std::move(e)) {}
    bool processPayment(double amount) override {
        std::cout << "Processing PayPal payment of $" << amount << " from " << email << "\n";
        return true;
    }
};

// --- RentalSystem (Singleton) ---
class RentalSystem {
    std::vector<std::shared_ptr<RentalStore>> stores;
    ReservationManager reservationManager;
    std::unordered_map<int, std::shared_ptr<User>> users;

    RentalSystem() = default;
public:
    RentalSystem(const RentalSystem&) = delete;
    RentalSystem& operator=(const RentalSystem&) = delete;

    static RentalSystem& getInstance() {
        static RentalSystem instance;
        return instance;
    }

    void addStore(const std::shared_ptr<RentalStore>& s) { stores.push_back(s); }
    std::shared_ptr<RentalStore> getStore(int id) {
        for (auto& s : stores) if (s->getId() == id) return s;
        return nullptr;
    }

    void registerUser(const std::shared_ptr<User>& u) {
        if (users.count(u->getId())) {
            std::cout << "User with id: " << u->getId() << " already exists\n";
            return;
        }
        users[u->getId()] = u;
    }
    std::shared_ptr<User> getUser(int id) {
        auto it = users.find(id);
        return (it == users.end()) ? nullptr : it->second;
    }

    std::shared_ptr<Reservation> createReservation(int userId, const std::string& vehicleRegistration,
        int pickupStoreId, int returnStoreId,
        std::chrono::system_clock::time_point startDate,
        std::chrono::system_clock::time_point endDate) {
        auto user = getUser(userId);
        auto pickup = getStore(pickupStoreId);
        auto ret = getStore(returnStoreId);
        if (!user || !pickup || !ret) return nullptr;
        auto vehicle = pickup->getVehicle(vehicleRegistration);
        if (!vehicle) return nullptr;
        return reservationManager.createReservation(user, vehicle, pickup, ret, startDate, endDate);
    }

    bool processPayment(int reservationId, PaymentStrategy& strategy) {
        auto r = reservationManager.getReservation(reservationId);
        if (!r) return false;
        double amount = r->getTotalAmount();
        bool ok = strategy.processPayment(amount);
        if (ok) reservationManager.confirmReservation(reservationId);
        return ok;
    }

    void startRental(int reservationId) { reservationManager.startRental(reservationId); }
    void completeRental(int reservationId) { reservationManager.completeRental(reservationId); }
    void cancelReservation(int reservationId) { reservationManager.cancelReservation(reservationId); }
};

// --- Utility: parse a simple date yyyy-mm-dd to time_point ---
std::chrono::system_clock::time_point parseDate(const std::string& ymd) {
    std::tm tm = {};
    std::istringstream ss(ymd);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    if (ss.fail()) throw std::runtime_error("Failed to parse date");
    std::time_t time = std::mktime(&tm);
    return std::chrono::system_clock::from_time_t(time);
}

// --- Main (example usage) ---
int main() {
    using namespace std::chrono;

    RentalSystem& rentalSystem = RentalSystem::getInstance();

    auto store1 = std::make_shared<RentalStore>(1, "Downtown Rentals", Location("123 Main St", "New York", "NY", "10001"));
    auto store2 = std::make_shared<RentalStore>(2, "Airport Rentals", Location("456 Airport Rd", "Los Angeles", "CA", "90045"));
    rentalSystem.addStore(store1);
    rentalSystem.addStore(store2);

    // Create vehicles via factory, then store them as shared_ptr (factory returns unique_ptr)
    auto ev = VehicleFactory::createVehicle(VehicleType::ECONOMY, "EC001", "Toyota", 50.0);
    auto lv = VehicleFactory::createVehicle(VehicleType::LUXURY, "LX001", "Mercedes", 200.0);
    auto sv = VehicleFactory::createVehicle(VehicleType::SUV, "SV001", "Honda", 75.0);

    // move to shared_ptr for sharing between store & reservation
    store1->addVehicle(std::shared_ptr<Vehicle>(std::move(ev)));
    store1->addVehicle(std::shared_ptr<Vehicle>(std::move(lv)));
    store2->addVehicle(std::shared_ptr<Vehicle>(std::move(sv)));

    // Register users
    auto user1 = std::make_shared<User>(123, "ABC", "abc@gmail.com");
    auto user2 = std::make_shared<User>(345, "BCD", "bcd@yahoo.com");
    rentalSystem.registerUser(user1);
    rentalSystem.registerUser(user2);

    // Create a reservation (dates)
    auto start = parseDate("2025-05-01");
    auto end = parseDate("2025-05-15");
    auto reservation = rentalSystem.createReservation(user1->getId(), "EC001", store1->getId(), store1->getId(), start, end);
    if (!reservation) {
        std::cout << "Failed to create reservation\n";
        return 1;
    }

    std::cout << "Created reservation #" << reservation->getId() << ", total $" << reservation->getTotalAmount() << "\n";

    // Process payment
    CreditCardPayment cc("4111111111111111", "John Doe", "12/27");
    bool paid = rentalSystem.processPayment(reservation->getId(), cc);
    if (paid) {
        std::cout << "Payment successful. Starting rental...\n";
        rentalSystem.startRental(reservation->getId());
        // ... simulate period ...
        rentalSystem.completeRental(reservation->getId());
        std::cout << "Rental completed.\n";
    }
    else {
        std::cout << "Payment failed.\n";
    }

    return 0;
}
