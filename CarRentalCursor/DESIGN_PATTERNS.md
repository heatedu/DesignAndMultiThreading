# Design Patterns Implementation - Car Rental System

## Overview
This Car Rental System demonstrates **5 core design patterns** as required for SDE2 interview:

1. ✅ **Factory Pattern** - Vehicle Creation
2. ✅ **Singleton Pattern** - Rental System
3. ✅ **Strategy Pattern** - Payment Processing
4. ✅ **State Pattern** - Reservation Status Management
5. ✅ **Template Method** - Vehicle Pricing (Bonus)

---

## 1. Factory Pattern for Vehicle Creation

### Purpose
Encapsulates vehicle creation logic and allows easy addition of new vehicle types.

### Implementation

**File: `VehicleFactory.h`**

```cpp
class VehicleFactory {
public:
    static std::shared_ptr<Vehicle> createVehicle(
        VehicleType type,
        const std::string& regNum,
        const std::string& model,
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
};
```

### Usage
```cpp
// Instead of:
auto car = std::make_shared<EconomyVehicle>("MH01", "Swift", 50);

// Use Factory:
auto car = VehicleFactory::createVehicle(VehicleType::ECONOMY, "MH01", "Swift", 50);
```

### Benefits
✅ **Encapsulation** - Creation logic in one place  
✅ **Extensibility** - Add new vehicle types easily  
✅ **Type Safety** - Ensures valid vehicle creation  
✅ **Consistency** - Uniform creation interface  

### Demo Output
```
3. FACTORY PATTERN - Creating vehicles...
   ✓ Created Economy: Swift
   ✓ Created Luxury: Mercedes S-Class
   ✓ Created SUV: Fortuner
```

---

## 2. Singleton Pattern for RentalSystem

### Purpose
Ensures a single point of control for the entire rental operation. Maintains consistency across all stores and operations.

### Implementation

**File: `CarRentalSystem.h`**

```cpp
class CarRentalSystem {
private:
    static CarRentalSystem* instance;
    static std::mutex instanceMutex;
    
    // Private constructor
    CarRentalSystem() : reservationCounter(0), paymentCounter(0) {}

public:
    // Thread-safe singleton instance
    static CarRentalSystem* getInstance() {
        if (!instance) {
            std::lock_guard<std::mutex> lock(instanceMutex);
            if (!instance) {
                instance = new CarRentalSystem();
            }
        }
        return instance;
    }

    // Prevent copying
    CarRentalSystem(const CarRentalSystem&) = delete;
    CarRentalSystem& operator=(const CarRentalSystem&) = delete;
};
```

### Usage
```cpp
auto system1 = CarRentalSystem::getInstance();
auto system2 = CarRentalSystem::getInstance();
// system1 and system2 point to the same instance
```

### Benefits
✅ **Single Instance** - Only one system object exists  
✅ **Global Access** - Accessible from anywhere  
✅ **Thread-Safe** - Double-checked locking with mutex  
✅ **Resource Control** - Centralized management  

### Demo Output
```
1. SINGLETON PATTERN - Creating system instance...
   System instance 1: 000001E6CE46F710
   System instance 2: 000001E6CE46F710
   ✓ Both instances are same (Singleton verified)
```

---

## 3. Strategy Pattern for Payment Processing

### Purpose
Enables different payment strategies based on payment type (Credit Card, Debit Card, Cash, UPI, Net Banking). Can switch between strategies dynamically.

### Implementation

**File: `PaymentStrategy.h`**

**Abstract Strategy:**
```cpp
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual bool processPayment(double amt, const std::string& details) = 0;
    virtual std::string getPaymentType() const = 0;
    virtual bool validatePayment() const = 0;
};
```

**Concrete Strategies:**

```cpp
// Strategy 1: Credit Card
class CreditCardPayment : public PaymentStrategy {
private:
    std::string cardNumber, cvv, expiryDate, cardHolderName;
public:
    bool processPayment(double amt, const std::string& details) override {
        // Credit card specific processing
        std::cout << "Processing Credit Card payment...\n";
        return true;
    }
    std::string getPaymentType() const override { return "Credit Card"; }
};

// Strategy 2: UPI
class UPIPayment : public PaymentStrategy {
private:
    std::string upiId, appName;
public:
    bool processPayment(double amt, const std::string& details) override {
        // UPI specific processing
        std::cout << "Processing UPI payment via " << appName << "...\n";
        return true;
    }
    std::string getPaymentType() const override { return "UPI"; }
};

// Strategy 3: Cash
class CashPayment : public PaymentStrategy {
    // Cash specific implementation
};

// Strategy 4: Debit Card
class DebitCardPayment : public PaymentStrategy {
    // Debit card specific implementation
};

// Strategy 5: Net Banking
class NetBankingPayment : public PaymentStrategy {
    // Net banking specific implementation
};
```

**Payment Context:**
```cpp
class Payment {
private:
    std::unique_ptr<PaymentStrategy> strategy;
public:
    bool process(double amount) {
        return strategy->processPayment(amount, reservationId);
    }
    
    // Dynamic strategy change
    void setStrategy(std::unique_ptr<PaymentStrategy> newStrategy) {
        strategy = std::move(newStrategy);
    }
};
```

### Usage
```cpp
// Create reservation with Credit Card
auto creditCardStrategy = std::make_unique<CreditCardPayment>("1234...", "123", "12/25", "John");
auto reservation = system->createReservation(userId, vehicleId, start, end, 
                                             std::move(creditCardStrategy));

// Create reservation with UPI
auto upiStrategy = std::make_unique<UPIPayment>("user@upi", "PhonePe");
auto reservation = system->createReservation(userId, vehicleId, start, end,
                                             std::move(upiStrategy));

// Create reservation with Cash
auto cashStrategy = std::make_unique<CashPayment>("Counter-Mumbai");
auto reservation = system->createReservation(userId, vehicleId, start, end,
                                             std::move(cashStrategy));
```

### Benefits
✅ **Flexibility** - Easy to add new payment methods  
✅ **Runtime Selection** - Choose payment method at runtime  
✅ **Open/Closed** - Add new strategies without modifying existing code  
✅ **Separation of Concerns** - Each payment type isolated  

### Demo Output
```
6. STRATEGY PATTERN - Creating reservations with different payments...
   
   Reservation 1: Credit Card Payment
   Payment Method: Credit Card
   Processing Credit Card payment of Rs.135.00...
   Card: **** **** **** 3456
   Transaction ID: CC-RES1-1689
   
   Reservation 2: UPI Payment
   Payment Method: UPI
   Processing UPI payment of Rs.150.00...
   UPI ID: priya@upi
   App: PhonePe
   Transaction ID: UPI-RES2-8918
   
   Reservation 3: Cash Payment
   Payment Method: Cash
   Processing Cash payment of Rs.910.00...
   Collected by: Counter-Mumbai
   Receipt Number: RCP-11068
```

---

## 4. State Pattern with Enumerations for Reservation Status

### Purpose
Manages the lifecycle of reservations. Handles transitions between different states (PENDING, CONFIRMED, IN_PROGRESS, COMPLETED, CANCELLED, NO_SHOW).

### Implementation

**File: `VehicleTypes.h`**

```cpp
enum class ReservationStatus {
    PENDING,        // Initial state after creation
    CONFIRMED,      // After payment confirmation
    IN_PROGRESS,    // Vehicle picked up
    COMPLETED,      // Vehicle returned
    CANCELLED,      // Cancelled by user or system
    NO_SHOW         // User didn't show up
};
```

**File: `Reservation.h`**

```cpp
class Reservation {
private:
    ReservationStatus status;

public:
    // State transitions with validation
    bool confirm() {
        if (status == ReservationStatus::PENDING) {
            status = ReservationStatus::CONFIRMED;
            return true;
        }
        return false;
    }

    bool startRental() {
        if (status == ReservationStatus::CONFIRMED) {
            status = ReservationStatus::IN_PROGRESS;
            pickupTime = time(nullptr);
            return true;
        }
        return false;
    }

    bool complete() {
        if (status == ReservationStatus::IN_PROGRESS) {
            status = ReservationStatus::COMPLETED;
            returnTime = time(nullptr);
            return true;
        }
        return false;
    }

    bool cancel() {
        if (status == ReservationStatus::PENDING || 
            status == ReservationStatus::CONFIRMED) {
            status = ReservationStatus::CANCELLED;
            return true;
        }
        return false;
    }

    bool markNoShow() {
        if (status == ReservationStatus::CONFIRMED) {
            status = ReservationStatus::NO_SHOW;
            return true;
        }
        return false;
    }
};
```

### State Transition Diagram

```
    PENDING
       |
       | (payment successful)
       ↓
   CONFIRMED ──────→ CANCELLED (cancel())
       |             
       | (pickup)    NO_SHOW (didn't show up)
       ↓
  IN_PROGRESS
       |
       | (return)
       ↓
   COMPLETED
```

### Usage
```cpp
// Create reservation (starts in PENDING)
auto reservation = system->createReservation(...);

// Payment confirms it
reservation->processPayment();  // PENDING → CONFIRMED

// Start rental
system->startRental(reservationId);  // CONFIRMED → IN_PROGRESS

// Complete rental
system->completeRental(reservationId);  // IN_PROGRESS → COMPLETED

// Or cancel
reservation->cancel();  // CONFIRMED → CANCELLED
```

### Benefits
✅ **Clear States** - Explicit reservation lifecycle  
✅ **Valid Transitions** - Only allowed state changes  
✅ **Type Safety** - Enums prevent invalid states  
✅ **Maintainability** - Easy to track and debug  

### Demo Output
```
8. STATE MANAGEMENT - Reservation lifecycle with enums...
   Reservation: RES1
   Current State: Confirmed
   
   → Starting rental (pickup)...
   ✓ State changed to: In Progress
   
   → Completing rental (return)...
   ✓ State changed to: Completed
```

---

## 5. Bonus: Template Method Pattern for Vehicle Pricing

### Purpose
Defines the skeleton of the pricing algorithm in the base class, with subclasses providing specific implementations.

### Implementation

**File: `Vehicle.h`**

```cpp
// Abstract base class
class Vehicle {
protected:
    double dailyRate;
public:
    // Template method - pure virtual
    virtual double calculatePrice(int days) const = 0;
};

// Concrete implementations
class EconomyVehicle : public Vehicle {
public:
    double calculatePrice(int days) const override {
        return dailyRate * days;  // Base rate
    }
};

class LuxuryVehicle : public Vehicle {
public:
    double calculatePrice(int days) const override {
        return dailyRate * days * 1.5;  // 50% premium
    }
};

class SUVVehicle : public Vehicle {
public:
    double calculatePrice(int days) const override {
        return dailyRate * days * 1.3;  // 30% premium
    }
};
```

### Benefits
✅ **Polymorphism** - Runtime behavior selection  
✅ **Code Reuse** - Common structure in base class  
✅ **Extensibility** - Easy to add new vehicle types  
✅ **Maintainability** - Pricing logic encapsulated  

---

## Additional State Management with Enums

### Vehicle Status Enum

**File: `VehicleTypes.h`**

```cpp
enum class VehicleStatus {
    AVAILABLE,
    RESERVED,
    RENTED,
    MAINTENANCE,
    OUT_OF_SERVICE
};
```

### Vehicle Type Enum

```cpp
enum class VehicleType {
    ECONOMY,
    LUXURY,
    SUV,
    SEDAN,
    HATCHBACK
};
```

### Payment Status Enum

```cpp
enum class PaymentStatus {
    PENDING,
    PROCESSING,
    COMPLETED,
    FAILED,
    REFUNDED
};
```

---

## Pattern Integration Flow

Here's how all patterns work together:

```
1. User requests vehicle
        ↓
2. SINGLETON System instance handles request
        ↓
3. FACTORY creates vehicle if needed
        ↓
4. System creates reservation (STATE: PENDING)
        ↓
5. STRATEGY selects payment method
        ↓
6. Payment processed → STATE: CONFIRMED
        ↓
7. Pickup → STATE: IN_PROGRESS
        ↓
8. Return → STATE: COMPLETED
```

---

## Interview Talking Points

### When asked about Factory Pattern:
**"I used Factory Pattern for vehicle creation to encapsulate object creation logic. This makes it easy to add new vehicle types without modifying existing code. For example, if we want to add a SPORTS_CAR type, we just add a case in the factory."**

### When asked about Singleton Pattern:
**"CarRentalSystem uses Singleton because we need exactly one instance managing all stores and reservations globally. I implemented thread-safe double-checked locking to handle concurrent access."**

### When asked about Strategy Pattern:
**"For payments, I implemented Strategy Pattern with 5 different concrete strategies: CreditCard, DebitCard, UPI, Cash, and NetBanking. Each has its own validation and processing logic. Users can select any payment method at runtime, and we can add new payment methods without modifying existing reservation code."**

### When asked about State Pattern:
**"Reservations follow a strict lifecycle using enums: PENDING → CONFIRMED → IN_PROGRESS → COMPLETED. State transitions are validated - you can't complete a pending reservation or cancel one that's in progress. This prevents invalid state changes."**

---

## Files Modified

| File | Pattern(s) | Description |
|------|-----------|-------------|
| `VehicleFactory.h` | Factory | Vehicle creation logic |
| `CarRentalSystem.h` | Singleton | Thread-safe singleton implementation |
| `PaymentStrategy.h` | Strategy | 5 payment strategies |
| `Reservation.h` | State | Lifecycle management with enums |
| `VehicleTypes.h` | State | All enumerations |
| `Vehicle.h` | Template Method | Polymorphic pricing |
| `Store.h` | - | Uses Factory for vehicle creation |
| `main.cpp` | - | Demonstrates all patterns |

---

## Summary

✅ **Factory Pattern** - Encapsulates vehicle creation, easy to extend  
✅ **Singleton Pattern** - Single system instance, thread-safe  
✅ **Strategy Pattern** - 5 payment strategies, runtime selection  
✅ **State Pattern** - Reservation lifecycle with 6 states  
✅ **Template Method** - Polymorphic pricing in vehicle hierarchy  

All patterns are production-ready and demonstrate SOLID principles!

