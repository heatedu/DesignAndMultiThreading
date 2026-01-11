# Car Rental System - Interview Guide

## Quick Reference for 1-Hour Interview

### Timeline Breakdown

**Phase 1: Requirements (5 min)**
- Clarify: Multiple locations? Vehicle types? Reservation conflicts?
- Confirm: Payment, cancellation, search requirements
- State assumptions clearly

**Phase 2: Core Classes (30 min)**
Write in this order:
1. `VehicleTypes.h` - Enums (2 min)
2. `Vehicle.h` - Hierarchy (8 min)
3. `Reservation.h` - With conflict logic (7 min)
4. `Store.h` - Inventory management (5 min)
5. `CarRentalSystem.h` - Main controller (8 min)

**Phase 3: Demo & Testing (10 min)**
- Write quick main() demonstrating key flows
- Show conflict prevention working

**Phase 4: Discussion (15 min)**
- Extensions, scalability, edge cases

---

## Key Talking Points During Coding

### While Writing `Vehicle.h`
**"I'm using polymorphism for pricing because different vehicle types have different pricing strategies. Economy is base rate, Luxury has 50% premium, SUV has 30% premium."**

**"This follows Open/Closed principle - we can add new vehicle types without modifying existing code."**

### While Writing `Reservation.h`
**"The critical logic here is `hasConflict()`. It checks if two time ranges overlap using the condition: NOT (end1 <= start2 OR start1 >= end2)."**

**"I'm also handling cancelled reservations - they shouldn't block future bookings."**

### While Writing `CarRentalSystem.h`
**"Using Singleton because we need a single point of coordination for all stores, users, and reservations."**

**"In `createReservation()`, I check conflicts BEFORE creating the reservation. This is critical for preventing double-booking."**

**"The search function filters by location first, then checks availability against existing reservations."**

---

## Common Follow-Up Questions & Answers

### Q1: "How would you handle concurrent bookings?"

```cpp
class CarRentalSystem {
private:
    std::mutex reservationMutex;
    
public:
    std::shared_ptr<Reservation> createReservation(...) {
        std::lock_guard<std::mutex> lock(reservationMutex);
        // Critical section: conflict check + reservation creation
        // This ensures atomicity
    }
};
```

**Key Point**: "The conflict check and reservation creation must be atomic. We need a mutex to prevent race conditions where two requests check simultaneously and both think the vehicle is available."

---

### Q2: "How would you add different pricing strategies?"

```cpp
class PricingStrategy {
public:
    virtual double calculate(const Vehicle* v, int days, time_t start) = 0;
};

class WeekendPricing : public PricingStrategy {
    double calculate(const Vehicle* v, int days, time_t start) override {
        // Check if weekend, add 20% surge
        return v->getBasePrice() * days * 1.2;
    }
};

class Vehicle {
    std::shared_ptr<PricingStrategy> pricingStrategy;
public:
    void setPricingStrategy(std::shared_ptr<PricingStrategy> s) {
        pricingStrategy = s;
    }
    double calculatePrice(int days, time_t start) {
        return pricingStrategy->calculate(this, days, start);
    }
};
```

**Key Point**: "Strategy pattern allows runtime selection of pricing algorithms. Could switch based on season, demand, customer loyalty tier."

---

### Q3: "Database schema?"

```sql
-- Stores table
CREATE TABLE stores (
    store_id VARCHAR(50) PRIMARY KEY,
    location VARCHAR(100),
    address VARCHAR(255)
);

-- Vehicles table
CREATE TABLE vehicles (
    registration_number VARCHAR(50) PRIMARY KEY,
    model VARCHAR(100),
    vehicle_type ENUM('ECONOMY', 'LUXURY', 'SUV'),
    daily_rate DECIMAL(10,2),
    status ENUM('AVAILABLE', 'RESERVED', 'RENTED'),
    store_id VARCHAR(50),
    FOREIGN KEY (store_id) REFERENCES stores(store_id),
    INDEX idx_store_type_status (store_id, vehicle_type, status)
);

-- Users table
CREATE TABLE users (
    user_id VARCHAR(50) PRIMARY KEY,
    name VARCHAR(100),
    license_number VARCHAR(50) UNIQUE
);

-- Reservations table
CREATE TABLE reservations (
    reservation_id VARCHAR(50) PRIMARY KEY,
    user_id VARCHAR(50),
    vehicle_reg VARCHAR(50),
    start_time TIMESTAMP,
    end_time TIMESTAMP,
    status ENUM('ACTIVE', 'COMPLETED', 'CANCELLED'),
    total_cost DECIMAL(10,2),
    FOREIGN KEY (user_id) REFERENCES users(user_id),
    FOREIGN KEY (vehicle_reg) REFERENCES vehicles(registration_number),
    INDEX idx_vehicle_time (vehicle_reg, start_time, end_time)
);
```

**Key Point**: "The composite index on (vehicle_reg, start_time, end_time) is crucial for fast conflict detection queries."

---

### Q4: "How to optimize conflict detection for millions of reservations?"

**Current**: O(R) where R = all reservations

**Optimized Approach 1 - Index by Vehicle**:
```cpp
std::unordered_map<std::string, std::vector<std::shared_ptr<Reservation>>> 
    reservationsByVehicle;
```
Now: O(k) where k = reservations per vehicle (typically << R)

**Optimized Approach 2 - Interval Tree**:
```cpp
// Use interval tree per vehicle
// Insertion: O(log k)
// Conflict check: O(log k + m) where m = overlapping intervals
```

**Database Query**:
```sql
SELECT * FROM reservations 
WHERE vehicle_reg = ? 
  AND status != 'CANCELLED'
  AND NOT (end_time <= ? OR start_time >= ?)
LIMIT 1;
```

**Key Point**: "For production, I'd partition reservations by vehicle_id. Database would use B-tree index on (vehicle_reg, start_time) for O(log k) lookups."

---

### Q5: "How to handle late returns and damage fees?"

```cpp
class Reservation {
private:
    time_t actualEndTime;
    double lateFee;
    double damageFee;
    std::string damageNotes;

public:
    void completeWithPenalties(time_t actualReturn, double damage) {
        actualEndTime = actualReturn;
        
        // Calculate late fee
        if (actualReturn > endTime) {
            int lateHours = (actualReturn - endTime) / 3600;
            lateFee = (dailyRate / 24) * lateHours * 1.5; // 50% penalty
        }
        
        damageFee = damage;
        status = ReservationStatus::COMPLETED;
    }
    
    double getFinalAmount() {
        return totalCost + lateFee + damageFee;
    }
};
```

---

### Q6: "How to implement payment processing?"

```cpp
class PaymentProcessor {
public:
    virtual bool processPayment(double amount, const std::string& details) = 0;
};

class CreditCardProcessor : public PaymentProcessor {
    // Integration with Stripe, PayPal, etc.
};

class Payment {
    enum class PaymentStatus { PENDING, COMPLETED, FAILED, REFUNDED };
    PaymentStatus status;
    std::unique_ptr<PaymentProcessor> processor;
    
public:
    bool process() {
        status = PaymentStatus::PROCESSING;
        bool success = processor->processPayment(amount, reservationId);
        status = success ? PaymentStatus::COMPLETED : PaymentStatus::FAILED;
        return success;
    }
};

// In Reservation
class Reservation {
    std::unique_ptr<Payment> payment;
    
    bool confirmReservation() {
        if (!payment || !payment->process()) {
            cancel();
            return false;
        }
        status = ReservationStatus::CONFIRMED;
        return true;
    }
};
```

---

### Q7: "Scalability for millions of users and requests?"

**Horizontal Scaling**:
- Partition by location (Mumbai service, Delhi service)
- Load balancer distributes by geography

**Caching Layer**:
```
Redis cache for:
- Available vehicles per location (TTL: 5 min)
- User session data
- Recent search results
```

**Database Optimization**:
- Read replicas for search queries
- Master for writes (reservations)
- Sharding by location_id

**Async Processing**:
```cpp
// Use message queue for non-critical operations
- Email notifications
- SMS confirmations
- Analytics logging
- Invoice generation
```

**Rate Limiting**:
```cpp
// Prevent abuse
- Max 10 searches per minute per user
- Max 5 reservation attempts per hour
```

---

### Q8: "How to add vehicle search filters?"

```cpp
class VehicleFilter {
public:
    virtual bool apply(const Vehicle* v) const = 0;
};

class PriceRangeFilter : public VehicleFilter {
    double minPrice, maxPrice;
public:
    bool apply(const Vehicle* v) const override {
        return v->getDailyRate() >= minPrice && 
               v->getDailyRate() <= maxPrice;
    }
};

class SearchEngine {
public:
    static vector<Vehicle*> search(
        vector<Vehicle*> vehicles,
        vector<unique_ptr<VehicleFilter>>& filters) {
        
        vector<Vehicle*> results;
        for (auto* v : vehicles) {
            bool passes = true;
            for (auto& filter : filters) {
                if (!filter->apply(v)) {
                    passes = false;
                    break;
                }
            }
            if (passes) results.push_back(v);
        }
        return results;
    }
};

// Usage
vector<unique_ptr<VehicleFilter>> filters;
filters.push_back(make_unique<PriceRangeFilter>(50, 200));
filters.push_back(make_unique<TypeFilter>(VehicleType::SUV));
auto results = SearchEngine::search(allVehicles, filters);
```

---

## Edge Cases to Mention

1. **Timezone handling**: Store all times in UTC, convert for display
2. **Reservation modifications**: Allow only if new dates don't conflict
3. **Vehicle maintenance**: Set status to MAINTENANCE, blocks new bookings
4. **No-shows**: Timer to auto-cancel if not picked up within grace period
5. **Partial cancellations**: Different refund policies based on time before pickup
6. **Multi-day pricing**: Discounts for weekly/monthly rentals
7. **Peak season surge**: Dynamic pricing during holidays
8. **Insurance options**: Additional services in reservation

---

## Anti-Patterns to Avoid

❌ **Don't**: Put business logic in main()
✅ **Do**: Keep main() as demo/driver only

❌ **Don't**: Use raw pointers
✅ **Do**: Use smart pointers (shared_ptr, unique_ptr)

❌ **Don't**: Hardcode magic numbers
✅ **Do**: Use constants or config

❌ **Don't**: Ignore const correctness
✅ **Do**: Mark getters as const

❌ **Don't**: Return null for collections
✅ **Do**: Return empty vector

---

## Strong Closing Statements

**"This design demonstrates:**
- **SOLID principles** - each class has single responsibility
- **Design patterns** - Singleton, Strategy, Template Method
- **Scalability** - can extend for payments, notifications, pricing
- **Conflict prevention** - core business requirement handled correctly
- **Production-ready thinking** - considered concurrency, database, caching

**What I'd add given more time:**
- Notification service (Observer pattern)
- Advanced search with Elasticsearch
- Audit logging
- Admin dashboard
- Mobile app integration
- Analytics and reporting"

---

## Time-Saving Tips

1. **Start with enums** - quickest to write, shows you're thinking about types
2. **Abstract class first** - shows polymorphic thinking
3. **Focus on one complex method** - like `hasConflict()` - shows algorithmic skill
4. **Comment critical sections** - "// Prevent double-booking" 
5. **Name things clearly** - `createReservation` not just `create`

---

## Red Flags Interviewers Look For

🚩 No consideration for concurrency
🚩 No conflict detection logic
🚩 Poor separation of concerns
🚩 God class (one class doing everything)
🚩 No extensibility (hardcoded types)
🚩 Memory leaks (raw pointers without delete)
🚩 No error handling

---

## What Impresses Interviewers

✨ Clarifying requirements upfront
✨ Drawing a quick class diagram
✨ Mentioning design patterns by name
✨ Thinking about edge cases
✨ Discussing tradeoffs
✨ Scalability considerations
✨ Clean, readable code
✨ Const correctness
✨ Smart pointers

---

**Remember**: This is not just a coding test. It's a conversation about software design. Think aloud, discuss tradeoffs, and show you understand real-world constraints!

