# Car Rental System - Complete Class Diagram with Design Patterns

## UML Class Diagram (Text Format) - Updated with All Patterns

```
┌──────────────────────────────────────────────────────────────────────┐
│                    <<singleton>> (DESIGN PATTERN 2)                  │
│                       CarRentalSystem                                │
├──────────────────────────────────────────────────────────────────────┤
│ - instance: static CarRentalSystem*                                  │
│ - instanceMutex: static mutex                                        │
│ - stores: unordered_map<string, shared_ptr<Store>>                   │
│ - users: unordered_map<string, shared_ptr<User>>                     │
│ - reservations: unordered_map<string, shared_ptr<Reservation>>       │
│ - reservationCounter: int                                            │
│ - paymentCounter: int                                                │
├──────────────────────────────────────────────────────────────────────┤
│ + getInstance(): static CarRentalSystem*  [thread-safe]              │
│ + addStore(store): void                                              │
│ + addUser(user): void                                                │
│ + searchVehicles(location, type, start, end): vector<Vehicle*>       │
│ + createReservation(userId, vehicleReg, start, end,                  │
│                     paymentStrategy): Reservation*                   │
│ + processReservationPayment(resId): bool                             │
│ + startRental(resId): bool                                           │
│ + completeRental(resId): bool                                        │
│ + cancelReservation(resId): bool                                     │
│ + addVehicleToStore(location, type, regNum, model, rate): bool       │
│ - hasConflict(vehicleReg, start, end): bool                          │
│ - findVehicle(regNumber): Vehicle*                                   │
└──────────────────────────────────────────────────────────────────────┘
              │                    │                  │
              │ manages            │ manages          │ manages
              │ 1..*               │ 1..*             │ 1..*
              ▼                    ▼                  ▼
    ┌─────────────┐    ┌─────────────┐    ┌──────────────────────┐
    │   Store     │    │    User     │    │   Reservation        │
    │             │    │             │    │   (STATE PATTERN 4)  │
    ├─────────────┤    ├─────────────┤    ├──────────────────────┤
    │- storeId    │    │- userId     │    │- reservationId       │
    │- location   │    │- name       │    │- userId              │
    │- address    │    │- license    │    │- vehicleRegNumber    │
    │- inventory  │    ├─────────────┤    │- startTime           │
    ├─────────────┤    │+getUserId() │    │- endTime             │
    │+addVehicle()│    │+getName()   │    │- status (6 states)   │
    │+createAnd   │    └─────────────┘    │- totalCost           │
    │ AddVehicle()│                       │- payment             │
    │  (uses      │                       ├──────────────────────┤
    │  Factory)   │                       │+hasConflict()        │
    │+getVehicle()│                       │+confirm()            │
    └──────┬──────┘                       │+startRental()        │
           │                              │+complete()           │
           │ contains                     │+cancel()             │
           │ 1..*                         │+markNoShow()         │
           ▼                              │+processPayment()     │
    ┌────────────────────────────┐       └──────────┬───────────┘
    │  <<factory>> (PATTERN 1)   │                  │ has
    │   VehicleFactory           │                  │ 1
    ├────────────────────────────┤                  ▼
    │+ createVehicle(type,       │       ┌──────────────────┐
    │    regNum, model, rate):   │       │    Payment       │
    │    Vehicle* [static]       │       ├──────────────────┤
    │+ validateParams():bool     │       │- paymentId       │
    └────────────┬───────────────┘       │- reservationId   │
                 │ creates               │- strategy        │
                 ▼                       ├──────────────────┤
    ┌──────────────────────────┐        │+ process(amt)    │
    │    <<abstract>>          │        │+ setStrategy()   │
    │       Vehicle            │◄───────┤+ getStatus()     │
    ├──────────────────────────┤    refers to            │
    │# registrationNumber      │        └────────┬─────────┘
    │# model                   │                 │ uses
    │# type                    │                 │ 1
    │# dailyRate               │                 ▼
    │# status                  │      ┌────────────────────────────┐
    ├──────────────────────────┤      │ <<strategy>> (PATTERN 3)   │
    │+ calculatePrice(days)    │      │    PaymentStrategy         │
    │  [abstract]              │      ├────────────────────────────┤
    │+ getRegNumber()          │      │# status                    │
    │+ getType()               │      │# amount                    │
    │+ getStatus()             │      │# transactionId             │
    │+ setStatus()             │      ├────────────────────────────┤
    └────────────┬─────────────┘      │+ processPayment() [abstract│
                 │                    │+ getPaymentType() [abstract│
                 │ inherits           │+ validatePayment() [abstract
        ┌────────┼────────┐           └──────────┬─────────────────┘
        │        │        │                      │
        ▼        ▼        ▼                      │ implemented by
┌──────────┐ ┌────────┐ ┌────────┐              │
│ Economy  │ │ Luxury │ │  SUV   │    ┌─────────┼─────────┬─────────┬─────────┐
│ Vehicle  │ │ Vehicle│ │ Vehicle│    │         │         │         │         │
├──────────┤ ├────────┤ ├────────┤    ▼         ▼         ▼         ▼         ▼
│+calc     │ │+calc   │ │+calc   │ ┌────────┐┌───────┐┌─────┐┌──────┐┌───────┐
│Price()   │ │Price() │ │Price() │ │Credit  ││Debit  ││ UPI ││ Cash ││Net    │
│rate*days │ │rate*   │ │rate*   │ │Card    ││Card   ││     ││      ││Banking│
│*1.0      │ │days*1.5│ │days*1.3│ └────────┘└───────┘└─────┘└──────┘└───────┘
└──────────┘ └────────┘ └────────┘
```

## Enumerations (DESIGN PATTERN 5 - State Management)

```
┌──────────────────┐  ┌────────────────────┐  ┌─────────────────────┐
│  VehicleType     │  │  VehicleStatus     │  │ ReservationStatus   │
├──────────────────┤  ├────────────────────┤  ├─────────────────────┤
│ • ECONOMY        │  │ • AVAILABLE        │  │ • PENDING           │
│ • LUXURY         │  │ • RESERVED         │  │ • CONFIRMED         │
│ • SUV            │  │ • RENTED           │  │ • IN_PROGRESS       │
│ • SEDAN          │  │ • MAINTENANCE      │  │ • COMPLETED         │
│ • HATCHBACK      │  │ • OUT_OF_SERVICE   │  │ • CANCELLED         │
└──────────────────┘  └────────────────────┘  │ • NO_SHOW           │
                                              └─────────────────────┘

┌──────────────────┐  ┌────────────────────┐
│  PaymentType     │  │  PaymentStatus     │
├──────────────────┤  ├────────────────────┤
│ • CREDIT_CARD    │  │ • PENDING          │
│ • DEBIT_CARD     │  │ • PROCESSING       │
│ • CASH           │  │ • COMPLETED        │
│ • UPI            │  │ • FAILED           │
│ • NET_BANKING    │  │ • REFUNDED         │
└──────────────────┘  └────────────────────┘
```

## Design Patterns Visualization

### 1. Factory Pattern (VehicleFactory)

```
                  ┌─────────────────────┐
                  │  VehicleFactory     │
                  │    <<factory>>      │
                  └──────────┬──────────┘
                             │ createVehicle(type)
                             │
            ┌────────────────┼────────────────┐
            │                │                │
            ▼                ▼                ▼
      ┌──────────┐    ┌──────────┐    ┌──────────┐
      │ Economy  │    │ Luxury   │    │   SUV    │
      │ Vehicle  │    │ Vehicle  │    │ Vehicle  │
      └──────────┘    └──────────┘    └──────────┘
```

### 2. Singleton Pattern (CarRentalSystem)

```
    ┌─────────────────────────────────┐
    │     CarRentalSystem             │
    │      <<singleton>>              │
    ├─────────────────────────────────┤
    │ - instance: static CRS*         │
    │ - instanceMutex: static mutex   │
    ├─────────────────────────────────┤
    │ + getInstance(): static CRS*    │
    │   [thread-safe double-check]    │
    │ - CarRentalSystem() [private]   │
    │ - copy ctor deleted             │
    │ - operator= deleted             │
    └─────────────────────────────────┘
```

### 3. Strategy Pattern (Payment)

```
                ┌───────────────────────┐
                │  PaymentStrategy      │
                │    <<interface>>      │
                ├───────────────────────┤
                │+ processPayment()     │
                │+ validatePayment()    │
                │+ getPaymentType()     │
                └──────────┬────────────┘
                           │
         ┌─────────────────┼─────────────────┬──────────┐
         │                 │                 │          │
         ▼                 ▼                 ▼          ▼
┌────────────────┐ ┌─────────────┐ ┌──────────┐ ┌──────────┐
│ CreditCard     │ │ DebitCard   │ │   UPI    │ │   Cash   │
│ Payment        │ │ Payment     │ │ Payment  │ │ Payment  │
└────────────────┘ └─────────────┘ └──────────┘ └──────────┘
         │
         │
         ▼
┌────────────────┐
│  NetBanking    │
│  Payment       │
└────────────────┘

               ┌──────────────────┐
               │     Payment      │
               │    <<context>>   │
               ├──────────────────┤
               │- strategy        │
               ├──────────────────┤
               │+ process()       │
               │+ setStrategy()   │
               └──────────────────┘
```

### 4. State Pattern (Reservation Lifecycle)

```
State Transition Diagram:

      PENDING
         │
         │ processPayment()
         ▼
     CONFIRMED ──────────────→ CANCELLED
         │                         ↑
         │ startRental()           │
         ▼                         │ cancel()
    IN_PROGRESS                    │
         │                         │
         │ complete()              │
         ▼                         │
     COMPLETED                NO_SHOW
                            (timeout)

Each state has specific allowed transitions
```

## Complete Relationships

| From | To | Type | Multiplicity | Pattern |
|------|-----|------|--------------|---------|
| CarRentalSystem | Store | Composition | 1 → many | Singleton |
| CarRentalSystem | User | Composition | 1 → many | Singleton |
| CarRentalSystem | Reservation | Composition | 1 → many | Singleton |
| CarRentalSystem | VehicleFactory | Usage | uses | Factory |
| Store | Vehicle | Composition | 1 → many | - |
| Store | VehicleFactory | Usage | uses | Factory |
| VehicleFactory | Vehicle | Creates | creates | Factory |
| Reservation | Vehicle | Association | many → 1 | - |
| Reservation | Payment | Composition | 1 → 1 | Strategy |
| Payment | PaymentStrategy | Composition | 1 → 1 | Strategy |
| PaymentStrategy | CreditCardPayment | Inheritance | 1 → 1 | Strategy |
| PaymentStrategy | DebitCardPayment | Inheritance | 1 → 1 | Strategy |
| PaymentStrategy | UPIPayment | Inheritance | 1 → 1 | Strategy |
| PaymentStrategy | CashPayment | Inheritance | 1 → 1 | Strategy |
| PaymentStrategy | NetBankingPayment | Inheritance | 1 → 1 | Strategy |
| Vehicle | EconomyVehicle | Inheritance | 1 → 1 | Template |
| Vehicle | LuxuryVehicle | Inheritance | 1 → 1 | Template |
| Vehicle | SUVVehicle | Inheritance | 1 → 1 | Template |

## Key Algorithms

### 1. Singleton getInstance() - Thread-Safe

```cpp
static CarRentalSystem* getInstance() {
    if (!instance) {                          // First check (no lock)
        std::lock_guard<std::mutex> lock(instanceMutex);
        if (!instance) {                      // Double check (with lock)
            instance = new CarRentalSystem();
        }
    }
    return instance;
}
```

### 2. Factory createVehicle()

```cpp
static Vehicle* createVehicle(VehicleType type, ...) {
    switch (type) {
        case VehicleType::ECONOMY:
            return new EconomyVehicle(...);
        case VehicleType::LUXURY:
            return new LuxuryVehicle(...);
        case VehicleType::SUV:
            return new SUVVehicle(...);
        default:
            throw invalid_argument("Unknown type");
    }
}
```

### 3. Strategy processPayment()

```cpp
class Payment {
    unique_ptr<PaymentStrategy> strategy;
public:
    bool process(double amount) {
        return strategy->processPayment(amount, reservationId);
    }
    void setStrategy(unique_ptr<PaymentStrategy> newStrategy) {
        strategy = move(newStrategy);  // Dynamic switching
    }
};
```

### 4. State Transition with Validation

```cpp
bool Reservation::startRental() {
    if (status == ReservationStatus::CONFIRMED) {
        status = ReservationStatus::IN_PROGRESS;
        pickupTime = time(nullptr);
        return true;
    }
    return false;  // Invalid transition
}
```

## Pattern Integration Flow

```
USER REQUEST
     │
     ▼
┌────────────────────┐
│ SINGLETON SYSTEM   │  getInstance()
└──────┬─────────────┘
       │
       ▼
┌────────────────────┐
│ FACTORY PATTERN    │  createVehicle(type)
└──────┬─────────────┘
       │
       ▼
┌────────────────────┐
│ Create Reservation │  State: PENDING
└──────┬─────────────┘
       │
       ▼
┌────────────────────┐
│ STRATEGY PATTERN   │  Select payment method
│ (Credit/UPI/Cash)  │
└──────┬─────────────┘
       │
       ▼
┌────────────────────┐
│ Process Payment    │  State: PENDING → CONFIRMED
└──────┬─────────────┘
       │
       ▼
┌────────────────────┐
│ Start Rental       │  State: CONFIRMED → IN_PROGRESS
└──────┬─────────────┘
       │
       ▼
┌────────────────────┐
│ Complete Rental    │  State: IN_PROGRESS → COMPLETED
└────────────────────┘
```

## Interview Explanation Flow

### Start with High-Level
"The system uses **5 design patterns**:
1. **Singleton** for centralized control
2. **Factory** for vehicle creation
3. **Strategy** for payment processing
4. **State** for reservation lifecycle
5. **Enums** for type-safe state management"

### Explain Each Pattern
1. **Singleton**: "Only one CarRentalSystem instance, thread-safe"
2. **Factory**: "VehicleFactory encapsulates creation, easy to add types"
3. **Strategy**: "5 payment methods, runtime selection, isolated logic"
4. **State**: "6 reservation states, validated transitions"
5. **Enums**: "Type-safe states, compile-time checking"

### Show Integration
"All patterns work together: Singleton coordinates, Factory creates vehicles, Strategy handles payments, State manages lifecycle, and Enums ensure type safety throughout."

## Files Mapping

| Class/Pattern | File | Lines |
|--------------|------|-------|
| VehicleFactory | VehicleFactory.h | 60 |
| PaymentStrategy hierarchy | PaymentStrategy.h | 300+ |
| Singleton CarRentalSystem | CarRentalSystem.h | 258 |
| State Reservation | Reservation.h | 120 |
| All Enums | VehicleTypes.h | 52 |
| Vehicle hierarchy | Vehicle.h | 70 |
| Store | Store.h | 80 |
| User | User.h | 25 |
| Demo | main.cpp | 350 |

**Total**: ~1,315 lines of well-structured code

---

This diagram now includes **all 5 design patterns** with their relationships and interactions clearly shown!

