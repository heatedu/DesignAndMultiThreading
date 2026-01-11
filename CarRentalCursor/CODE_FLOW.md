# Car Rental System - Complete Code Flow

## 🚀 Program Execution Flow

### Overview
This document explains the **complete execution flow** of the Car Rental System, showing how all 5 design patterns work together.

---

## 📋 Table of Contents
1. [System Initialization](#1-system-initialization)
2. [Store Setup](#2-store-setup)
3. [Vehicle Creation (Factory Pattern)](#3-vehicle-creation-factory-pattern)
4. [User Registration](#4-user-registration)
5. [Vehicle Search](#5-vehicle-search)
6. [Reservation Creation with Payment (Strategy Pattern)](#6-reservation-creation-with-payment)
7. [State Transitions (State Pattern)](#7-state-transitions)
8. [Conflict Prevention](#8-conflict-prevention)
9. [Cancellation Flow](#9-cancellation-flow)

---

## 1. System Initialization

### 1.1 Entry Point
**File**: `main.cpp` - Line 1

```cpp
int main() {
    // Initialize random seed for transaction IDs
    srand(static_cast<unsigned int>(time(nullptr)));
```

**What happens**: Program starts, random seed initialized for generating transaction IDs.

---

### 1.2 Singleton Instance Creation
**File**: `main.cpp` - Line 40

```cpp
auto system = CarRentalSystem::getInstance();
auto system2 = CarRentalSystem::getInstance();
```

**Code Flow**:

```
main() 
  │
  └─► CarRentalSystem::getInstance()
       │
       ├─► Check: instance == nullptr?
       │    │
       │    ├─► YES: Lock mutex
       │    │    │
       │    │    ├─► Double-check: instance == nullptr?
       │    │    │    │
       │    │    │    └─► Create new CarRentalSystem()
       │    │    │         │
       │    │    │         └─► Initialize:
       │    │    │              - reservationCounter = 0
       │    │    │              - paymentCounter = 0
       │    │    │              - empty maps for stores/users/reservations
       │    │    │
       │    │    └─► Unlock mutex
       │    │
       │    └─► NO: Return existing instance
       │
       └─► Return instance pointer
```

**Result**: Both `system` and `system2` point to the **same instance** (Singleton verified).

**Output**:
```
System instance 1: 000001E6CE46F710
System instance 2: 000001E6CE46F710
✓ Both instances are same (Singleton verified)
```

---

## 2. Store Setup

**File**: `main.cpp` - Lines 48-52

```cpp
auto mumbaiStore = make_shared<Store>("S1", "Mumbai", "Andheri East");
auto delhiStore = make_shared<Store>("S2", "Delhi", "Connaught Place");
system->addStore(mumbaiStore);
system->addStore(delhiStore);
```

**Code Flow**:

```
main()
  │
  ├─► Store("S1", "Mumbai", "Andheri East")
  │    │
  │    └─► Initialize:
  │         - storeId = "S1"
  │         - location = "Mumbai"
  │         - address = "Andheri East"
  │         - inventory = empty map
  │
  ├─► CarRentalSystem::addStore(mumbaiStore)
  │    │
  │    └─► stores["Mumbai"] = mumbaiStore
  │
  └─► (Same for Delhi store)
```

**Result**: 2 stores added to system, accessible by location name.

---

## 3. Vehicle Creation (Factory Pattern)

### 3.1 Using Factory Directly
**File**: `main.cpp` - Lines 64-68

```cpp
auto swift = VehicleFactory::createVehicle(VehicleType::ECONOMY, 
                                           "MH01AB1234", "Swift", 50);
```

**Code Flow**:

```
main()
  │
  └─► VehicleFactory::createVehicle(ECONOMY, "MH01AB1234", "Swift", 50)
       │
       │ (File: VehicleFactory.h)
       │
       ├─► switch(VehicleType::ECONOMY)
       │    │
       │    └─► case ECONOMY:
       │         │
       │         └─► return make_shared<EconomyVehicle>("MH01AB1234", "Swift", 50)
       │              │
       │              │ (File: Vehicle.h)
       │              │
       │              ├─► EconomyVehicle constructor
       │              │    │
       │              │    └─► Call base Vehicle constructor
       │              │         │
       │              │         └─► Initialize:
       │              │              - registrationNumber = "MH01AB1234"
       │              │              - model = "Swift"
       │              │              - type = ECONOMY
       │              │              - dailyRate = 50
       │              │              - status = AVAILABLE
       │              │
       │              └─► Return shared_ptr<EconomyVehicle>
       │
       └─► Return as shared_ptr<Vehicle>
```

**Polymorphism**: Returned as `Vehicle*` but actually points to `EconomyVehicle`.

### 3.2 Add Vehicle to Store
**File**: `main.cpp` - Line 76

```cpp
mumbaiStore->addVehicle(swift);
```

**Code Flow**:

```
Store::addVehicle(swift)
  │
  └─► inventory[swift->getRegNumber()] = swift
       │
       └─► inventory["MH01AB1234"] = shared_ptr to swift
```

**Result**: Vehicle stored in inventory map, accessible by registration number.

---

## 4. User Registration

**File**: `main.cpp` - Lines 92-97

```cpp
auto user1 = make_shared<User>("U001", "Rahul Sharma", "DL123456");
system->addUser(user1);
```

**Code Flow**:

```
main()
  │
  ├─► User("U001", "Rahul Sharma", "DL123456")
  │    │
  │    └─► Initialize:
  │         - userId = "U001"
  │         - name = "Rahul Sharma"
  │         - licenseNumber = "DL123456"
  │
  └─► CarRentalSystem::addUser(user1)
       │
       └─► users["U001"] = user1
```

**Result**: User stored in system, accessible by userId.

---

## 5. Vehicle Search

**File**: `main.cpp` - Line 107

```cpp
auto economyCars = system->searchVehicles("Mumbai", VehicleType::ECONOMY, 
                                          start, end);
```

**Code Flow**:

```
CarRentalSystem::searchVehicles("Mumbai", ECONOMY, start, end)
  │
  │ (File: CarRentalSystem.h - Line 80)
  │
  ├─► Get store: stores.find("Mumbai")
  │    │
  │    └─► storeIt = iterator to mumbaiStore
  │
  ├─► Get vehicles by type:
  │    │
  │    └─► Store::getAvailableVehicles(ECONOMY)
  │         │
  │         │ (File: Store.h - Line 42)
  │         │
  │         └─► For each vehicle in inventory:
  │              │
  │              ├─► Check: vehicle->getStatus() == AVAILABLE?
  │              ├─► Check: vehicle->getType() == ECONOMY?
  │              │
  │              └─► If both true: add to result vector
  │
  ├─► Filter by date conflicts:
  │    │
  │    └─► For each available vehicle:
  │         │
  │         └─► hasConflict(vehicleReg, start, end)?
  │              │
  │              │ (Check all existing reservations)
  │              │
  │              ├─► NO CONFLICT: add to results
  │              └─► HAS CONFLICT: skip
  │
  └─► Return vector of available vehicles
```

**Result**: List of economy vehicles available in Mumbai for the date range.

---

## 6. Reservation Creation with Payment

### 6.1 Create Payment Strategy
**File**: `main.cpp` - Lines 125-127

```cpp
auto creditCardStrategy = make_unique<CreditCardPayment>(
    "1234567890123456", "123", "12/25", "Rahul Sharma");
```

**Code Flow**:

```
CreditCardPayment constructor
  │
  │ (File: PaymentStrategy.h - Line 45)
  │
  └─► Initialize:
       - cardNumber = "1234567890123456"
       - cvv = "123"
       - expiryDate = "12/25"
       - cardHolderName = "Rahul Sharma"
       - status = PENDING (from base class)
       - amount = 0.0 (from base class)
```

### 6.2 Create Reservation
**File**: `main.cpp` - Lines 129-134

```cpp
auto reservation1 = system->createReservation(
    user1->getUserId(),
    selectedCar->getRegNumber(),
    start, end,
    std::move(creditCardStrategy)
);
```

**Code Flow**:

```
CarRentalSystem::createReservation(userId, vehicleReg, start, end, strategy)
  │
  │ (File: CarRentalSystem.h - Line 114)
  │
  ├─► Step 1: Check conflicts
  │    │
  │    └─► hasConflict(vehicleReg, start, end)
  │         │
  │         └─► For each existing reservation:
  │              │
  │              └─► Check time overlap
  │                   │
  │                   ├─► NO OVERLAP: continue
  │                   └─► OVERLAP: return nullptr (ABORT)
  │
  ├─► Step 2: Find vehicle
  │    │
  │    └─► findVehicle(vehicleReg)
  │         │
  │         └─► Search all stores for vehicle
  │              │
  │              └─► Return shared_ptr<Vehicle>
  │
  ├─► Step 3: Calculate price
  │    │
  │    ├─► Calculate days: (end - start) / 86400
  │    │    │
  │    │    └─► days = 3
  │    │
  │    └─► vehicle->calculatePrice(3)
  │         │
  │         │ (POLYMORPHISM - calls EconomyVehicle::calculatePrice)
  │         │
  │         │ (File: Vehicle.h - Line 36)
  │         │
  │         └─► return dailyRate * days
  │              │
  │              └─► return 50 * 3 = 150
  │
  ├─► Step 4: Generate reservation ID
  │    │
  │    └─► resId = "RES" + to_string(++reservationCounter)
  │         │
  │         └─► resId = "RES1"
  │
  ├─► Step 5: Create Reservation object
  │    │
  │    └─► Reservation("RES1", userId, vehicleReg, start, end)
  │         │
  │         │ (File: Reservation.h - Line 22)
  │         │
  │         └─► Initialize:
  │              - reservationId = "RES1"
  │              - userId = "U001"
  │              - vehicleRegNumber = "MH01AB1234"
  │              - startTime = start
  │              - endTime = end
  │              - status = PENDING (initial state)
  │              - totalCost = 0
  │              - createdAt = current time
  │
  ├─► Step 6: Set total cost
  │    │
  │    └─► reservation->setTotalCost(150.0)
  │
  ├─► Step 7: Create Payment with Strategy
  │    │
  │    ├─► paymentId = "PAY" + to_string(++paymentCounter)
  │    │    │
  │    │    └─► paymentId = "PAY1"
  │    │
  │    └─► Payment("PAY1", "RES1", move(creditCardStrategy))
  │         │
  │         │ (File: PaymentStrategy.h - Line 283)
  │         │
  │         └─► Initialize:
  │              - paymentId = "PAY1"
  │              - reservationId = "RES1"
  │              - strategy = unique_ptr to CreditCardPayment
  │
  ├─► Step 8: Attach payment to reservation
  │    │
  │    └─► reservation->setPayment(move(payment))
  │
  ├─► Step 9: Store reservation
  │    │
  │    └─► reservations["RES1"] = reservation
  │
  ├─► Step 10: Update vehicle status
  │    │
  │    └─► vehicle->setStatus(RESERVED)
  │
  └─► Return reservation
```

**Result**: Reservation created in PENDING state with payment attached.

### 6.3 Process Payment
**File**: `main.cpp` - Line 146

```cpp
system->processReservationPayment(reservation1->getReservationId())
```

**Code Flow**:

```
CarRentalSystem::processReservationPayment("RES1")
  │
  │ (File: CarRentalSystem.h - Line 151)
  │
  ├─► Get reservation: getReservation("RES1")
  │    │
  │    └─► reservations.find("RES1")
  │         │
  │         └─► Return reservation pointer
  │
  └─► Call: reservation->processPayment()
       │
       │ (File: Reservation.h - Line 73)
       │
       ├─► Get payment amount: totalCost = 150.0
       │
       └─► payment->process(150.0)
            │
            │ (File: PaymentStrategy.h - Line 286)
            │
            ├─► Get payment type:
            │    │
            │    └─► strategy->getPaymentType() = "Credit Card"
            │
            └─► Process payment:
                 │
                 │ (STRATEGY PATTERN - calls concrete strategy)
                 │
                 └─► strategy->processPayment(150.0, "RES1")
                      │
                      │ (File: PaymentStrategy.h - Line 51)
                      │
                      ├─► Validate payment:
                      │    │
                      │    └─► validatePayment()
                      │         │
                      │         └─► Check card details
                      │              │
                      │              └─► return true
                      │
                      ├─► Set status: PROCESSING
                      │
                      ├─► Print: "Processing Credit Card payment..."
                      │
                      ├─► Generate transaction ID:
                      │    │
                      │    └─► transactionId = "CC-RES1-" + rand()
                      │         │
                      │         └─► "CC-RES1-1689"
                      │
                      ├─► Set status: COMPLETED
                      │
                      ├─► Print: "Transaction ID: CC-RES1-1689"
                      │
                      ├─► Return true to payment->process()
                      │
                      └─► Reservation::processPayment() calls confirm()
                           │
                           │ (File: Reservation.h - Line 38)
                           │
                           ├─► Check: status == PENDING?
                           │    │
                           │    └─► YES: Continue
                           │
                           ├─► Set status: CONFIRMED
                           │
                           └─► Return true
```

**Output**:
```
Payment Method: Credit Card
Processing Credit Card payment of Rs.150.00...
Card: **** **** **** 3456
Transaction ID: CC-RES1-1689
✓ Payment successful!
Status updated: Confirmed
```

**Result**: 
- Payment processed using **Strategy Pattern** (Credit Card)
- Reservation state changed: **PENDING → CONFIRMED** (State Pattern)

---

## 7. State Transitions

### 7.1 Start Rental (Pickup)
**File**: `main.cpp` - Line 295

```cpp
system->startRental("RES1")
```

**Code Flow**:

```
CarRentalSystem::startRental("RES1")
  │
  │ (File: CarRentalSystem.h - Line 157)
  │
  ├─► Get reservation: getReservation("RES1")
  │
  └─► reservation->startRental()
       │
       │ (File: Reservation.h - Line 45)
       │
       ├─► Check: status == CONFIRMED?
       │    │
       │    └─► YES: Continue
       │
       ├─► Set status: IN_PROGRESS
       │
       ├─► Set pickupTime: current time
       │
       ├─► Return true to CarRentalSystem
       │
       └─► Update vehicle status:
            │
            └─► vehicle->setStatus(RENTED)
```

**State Change**: CONFIRMED → IN_PROGRESS

### 7.2 Complete Rental (Return)
**File**: `main.cpp` - Line 300

```cpp
system->completeRental("RES1")
```

**Code Flow**:

```
CarRentalSystem::completeRental("RES1")
  │
  │ (File: CarRentalSystem.h - Line 171)
  │
  ├─► Get reservation: getReservation("RES1")
  │
  └─► reservation->complete()
       │
       │ (File: Reservation.h - Line 52)
       │
       ├─► Check: status == IN_PROGRESS?
       │    │
       │    └─► YES: Continue
       │
       ├─► Set status: COMPLETED
       │
       ├─► Set returnTime: current time
       │
       ├─► Return true to CarRentalSystem
       │
       └─► Update vehicle status:
            │
            └─► vehicle->setStatus(AVAILABLE)
```

**State Change**: IN_PROGRESS → COMPLETED

**Complete State Flow**:
```
PENDING → CONFIRMED → IN_PROGRESS → COMPLETED
   ↓
CANCELLED
```

---

## 8. Conflict Prevention

**File**: `main.cpp` - Lines 260-267

```cpp
auto conflictRes = system->createReservation(
    user3->getUserId(),
    economyCars[0]->getRegNumber(),  // Same vehicle!
    start + 86400,   // 1 day later (overlaps)
    end + 86400
);
```

**Code Flow**:

```
CarRentalSystem::createReservation(...)
  │
  ├─► hasConflict(vehicleReg, newStart, newEnd)
  │    │
  │    │ (File: CarRentalSystem.h - Line 203)
  │    │
  │    └─► For each reservation in reservations:
  │         │
  │         ├─► Check: reservation->vehicleRegNumber == vehicleReg?
  │         │    │
  │         │    └─► YES: Check time conflict
  │         │         │
  │         │         └─► reservation->hasConflict(newStart, newEnd)
  │         │              │
  │         │              │ (File: Reservation.h - Line 28)
  │         │              │
  │         │              ├─► Check: status == CANCELLED?
  │         │              │    └─► NO: Continue
  │         │              │
  │         │              └─► Check overlap:
  │         │                   │
  │         │                   └─► !(newEnd <= start OR newStart >= end)
  │         │                        │
  │         │                        │ Existing: start=Day 2, end=Day 5
  │         │                        │ New: newStart=Day 3, newEnd=Day 6
  │         │                        │
  │         │                        │ newEnd <= start? NO (Day 6 > Day 2)
  │         │                        │ newStart >= end? NO (Day 3 < Day 5)
  │         │                        │
  │         │                        └─► OVERLAP DETECTED!
  │         │                             │
  │         │                             └─► return true
  │         │
  │         └─► HAS CONFLICT: return true
  │              │
  │              └─► Back to createReservation()
  │                   │
  │                   └─► return nullptr (ABORT)
  │
  └─► Reservation creation ABORTED
```

**Output**:
```
✓ SUCCESS: Booking rejected due to conflict (as expected)
```

**Result**: Conflict detected, reservation **not created**.

---

## 9. Cancellation Flow

**File**: `main.cpp` - Line 316

```cpp
system->cancelReservation("RES2")
```

**Code Flow**:

```
CarRentalSystem::cancelReservation("RES2")
  │
  │ (File: CarRentalSystem.h - Line 185)
  │
  ├─► Get reservation: reservations.find("RES2")
  │
  └─► reservation->cancel()
       │
       │ (File: Reservation.h - Line 59)
       │
       ├─► Check: status == PENDING or CONFIRMED?
       │    │
       │    └─► YES (status = CONFIRMED): Continue
       │
       ├─► Set status: CANCELLED
       │
       ├─► Return true to CarRentalSystem
       │
       └─► Update vehicle status:
            │
            └─► vehicle->setStatus(AVAILABLE)
                 │
                 └─► Vehicle available for booking again!
```

**State Change**: CONFIRMED → CANCELLED

**Result**: Vehicle status changed to AVAILABLE, can be booked again.

---

## 🔄 Complete Flow Summary

### Typical User Journey

```
1. System Initialization
   └─► Singleton instance created

2. Setup Phase
   ├─► Stores created
   └─► Vehicles created using Factory Pattern

3. User Registration
   └─► User added to system

4. Search Phase
   ├─► User searches vehicles
   ├─► System filters by type, location, dates
   └─► Conflict check performed

5. Booking Phase
   ├─► User selects payment method (Strategy Pattern)
   ├─► Reservation created (State: PENDING)
   ├─► Payment processed
   └─► State changes to CONFIRMED

6. Rental Phase
   ├─► User picks up vehicle
   ├─► State: CONFIRMED → IN_PROGRESS
   ├─► Vehicle status: RESERVED → RENTED
   └─► Pickup time recorded

7. Return Phase
   ├─► User returns vehicle
   ├─► State: IN_PROGRESS → COMPLETED
   ├─► Vehicle status: RENTED → AVAILABLE
   └─► Return time recorded

Alternative: Cancellation
   ├─► User cancels reservation
   ├─► State: CONFIRMED → CANCELLED
   └─► Vehicle status: RESERVED → AVAILABLE
```

---

## 🎯 Design Pattern Flow Integration

```
┌─────────────────────────────────────────────┐
│  1. SINGLETON - Get system instance         │
└─────────────────┬───────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────┐
│  2. FACTORY - Create vehicles               │
└─────────────────┬───────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────┐
│  3. ENUMS - Type-safe states throughout     │
└─────────────────┬───────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────┐
│  4. STRATEGY - Select payment method        │
└─────────────────┬───────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────┐
│  5. STATE - Manage reservation lifecycle    │
│     PENDING → CONFIRMED → IN_PROGRESS       │
│     → COMPLETED                             │
└─────────────────────────────────────────────┘
```

All patterns working together seamlessly!

---

## 📊 Method Call Sequence

For a complete reservation:

```
1. main()
2. └─► CarRentalSystem::getInstance()
3.     └─► (Singleton creation if needed)
4. └─► VehicleFactory::createVehicle()
5.     └─► EconomyVehicle::EconomyVehicle()
6.         └─► Vehicle::Vehicle()
7. └─► CarRentalSystem::searchVehicles()
8.     └─► Store::getAvailableVehicles()
9.     └─► hasConflict() (for each vehicle)
10. └─► CarRentalSystem::createReservation()
11.     └─► Reservation::Reservation()
12.     └─► Payment::Payment()
13.         └─► (Strategy attached)
14. └─► CarRentalSystem::processReservationPayment()
15.     └─► Reservation::processPayment()
16.         └─► Payment::process()
17.             └─► CreditCardPayment::processPayment()
18.                 └─► (Transaction processing)
19.             └─► Reservation::confirm()
20.                 └─► (State: PENDING → CONFIRMED)
21. └─► CarRentalSystem::startRental()
22.     └─► Reservation::startRental()
23.         └─► (State: CONFIRMED → IN_PROGRESS)
24. └─► CarRentalSystem::completeRental()
25.     └─► Reservation::complete()
26.         └─► (State: IN_PROGRESS → COMPLETED)
```

**Total**: 26 method calls for a complete rental lifecycle!

---

This is the **complete code flow** showing how all design patterns work together in practice! 🚀

