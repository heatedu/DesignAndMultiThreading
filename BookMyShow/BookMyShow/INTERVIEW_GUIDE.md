# BookMyShow Interview Discussion Guide

## Quick Overview (5 minutes)
**Problem**: Design a movie ticket booking system like BookMyShow that handles concurrent bookings, prevents double-booking, and manages seat availability in real-time.

**Key Requirements**:
- Multiple theatres, screens, and shows
- Seat categorization (PLATINUM, GOLD, SILVER)
- Concurrent booking prevention
- Session timeout for locked seats
- Payment processing with multiple strategies

---

## System Architecture (10 minutes)

### Layered Architecture
```
Controllers → Services → Providers/Models
     ↓           ↓            ↓
  API Layer  Business    Data Access
             Logic
```

### Core Components
1. **Models** (Domain Objects)
   - Theatre, Screen, Seat, Show, Movie
   - User, Booking, SeatLock

2. **Providers** (Strategy Pattern)
   - ISeatLockProvider (interface)
   - SeatLockProvider (in-memory implementation)

3. **Services** (Business Logic)
   - MovieService, TheatreService, ShowService
   - BookingService, PaymentService
   - SeatAvailabilityService

4. **Controllers** (API Layer)
   - MovieController, TheatreController, ShowController
   - BookingController, PaymentController

---

## Design Patterns Used (10 minutes)

### 1. Repository Pattern
**Where**: All Service classes
**Why**: Centralized data management, easy to swap implementations
```cpp
class MovieService {
    map<int, Movie> movies;  // In-memory repository
public:
    Movie createMovie(...);
    Movie getMovie(int id);
};
```

### 2. Strategy Pattern
**Where**: Payment processing
**Why**: Support multiple payment methods without modifying core logic
```cpp
interface PaymentStrategy {
    bool processPayment();
};

class DebitCardStrategy : PaymentStrategy { ... }
class UpiStrategy : PaymentStrategy { ... }
```

### 3. Dependency Injection
**Where**: All controllers and services
**Why**: Loose coupling, testability, flexibility
```cpp
BookingService(shared_ptr<ISeatLockProvider> provider);
```

### 4. Factory Method Pattern
**Where**: Service layer entity creation
**Why**: Centralized creation logic, auto-incrementing IDs
```cpp
shared_ptr<Movie> MovieService::createMovie(string name, int duration);
```

---

## Concurrency Handling (15 minutes)

### Problem
Two users simultaneously trying to book the same seat:
```
User1: SELECT seat 5 (available) → Book seat 5
User2: SELECT seat 5 (available) → Book seat 5 ❌ Double booking!
```

### Solution: Seat Locking

#### 1. Lock Acquisition
```cpp
void SeatLockProvider::lockSeats(Show show, vector<Seat> seats, User user) {
    lock_guard<mutex> guard(locksMutex);  // Thread-safe
    
    // Check if any seat already locked
    for (seat in seats) {
        if (seatLocks.contains(seat) && !isExpired(seat)) {
            throw "Seat already locked";
        }
    }
    
    // Lock all seats atomically
    for (seat in seats) {
        seatLocks[seat] = SeatLock(seat, show, timeout, now, user);
    }
}
```

#### 2. Lock Validation
```cpp
bool validateLock(Show show, Seat seat, User user) {
    return lock.exists() 
        && !lock.isExpired() 
        && lock.lockedBy == user;
}
```

#### 3. Lock Expiration
```cpp
bool SeatLock::isLockExpired() {
    auto expirationTime = lockTime + chrono::seconds(timeout);
    return chrono::system_clock::now() >= expirationTime;
}
```

### Concurrency Mechanisms Used
- **Mutex**: Protects critical sections (locksMutex)
- **Atomic Counters**: Thread-safe ID generation
- **Lock Guards**: RAII-based mutex management
- **Time-based Expiration**: Auto-release abandoned locks

---

## Critical Flows (10 minutes)

### Booking Flow
```
1. User selects seats
2. BookingController::createBooking()
3. BookingService checks if seats available
4. SeatLockProvider locks seats (10 min timeout)
5. Booking created with CREATED status
6. User pays via PaymentController
7. PaymentService validates locks
8. Booking status → CONFIRMED
9. Seats permanently marked as booked
```

### Concurrent Booking Scenario
```
Thread 1 (User A)              Thread 2 (User B)
─────────────────              ─────────────────
Lock mutex
Check seat 7 available
Lock seat 7 for User A
Unlock mutex
                              Lock mutex
                              Check seat 7 (locked by A)
                              ❌ Throw "Seat locked"
                              Unlock mutex
Payment succeeds
Confirm booking
```

---

## Scalability Discussion (10 minutes)

### Current Limitations (In-Memory)
1. ❌ Single server only
2. ❌ Lost on restart
3. ❌ No horizontal scaling
4. ❌ No persistence

### Production Solutions

#### 1. Distributed Locking (Redis)
```cpp
class RedisLockProvider : ISeatLockProvider {
    void lockSeats(...) {
        redis.SET("lock:show:{showId}:seat:{seatId}", userId, 
                  "EX", 600, "NX");  // 10 min expiry, only if not exists
    }
};
```

**Pros**: Distributed, persistent, atomic operations
**Cons**: Network latency, Redis dependency

#### 2. Database with Optimistic Locking
```sql
UPDATE bookings 
SET user_id = ?, status = 'LOCKED', version = version + 1
WHERE show_id = ? AND seat_id = ? AND status = 'AVAILABLE' AND version = ?;
-- If rows_affected == 0, seat taken
```

**Pros**: ACID guarantees, no external dependencies
**Cons**: Higher DB load, potential deadlocks

#### 3. Event Sourcing
Store all booking events:
```
Event 1: SeatLockRequested(user, show, seat, timestamp)
Event 2: SeatLockAcquired(user, show, seat, lockId)
Event 3: PaymentCompleted(lockId)
Event 4: BookingConfirmed(lockId)
```

**Pros**: Full audit trail, replay capability, temporal queries
**Cons**: Complex implementation, eventual consistency

---

## Trade-offs & Design Decisions (5 minutes)

### 1. Lock Timeout Duration
**Current**: 600 seconds (10 minutes)
- ✅ Enough time for payment
- ❌ Holds seats unnecessarily if user abandons
- **Alternative**: 2-3 minutes with warnings

### 2. Lock Granularity
**Current**: Per-seat locking
- ✅ Fine-grained control
- ✅ Maximizes availability
- ❌ More lock operations
- **Alternative**: Row-level locking (faster, less available)

### 3. In-Memory vs Persistent
**Current**: In-memory maps
- ✅ Fast O(1) lookups
- ✅ Simple implementation
- ❌ No persistence
- **Production**: Redis + DB combination

### 4. Atomic Multi-Seat Booking
**Current**: All-or-nothing lock acquisition
- ✅ Prevents partial bookings
- ❌ Higher lock contention
- **Alternative**: Individual seat locks (risks fragmentation)

---

## Extension Ideas (5 minutes)

### Easy Extensions
1. **Seat Pricing**: Add price field, pricing strategy
2. **Show Timing**: Prevent overlapping shows on same screen
3. **User History**: Track user bookings
4. **Cancellation**: Add refund logic, release seats

### Medium Extensions
1. **Seat Selection Preferences**: Window, aisle, center
2. **Discounts**: Coupon codes, promotions
3. **Waitlist**: Notify when seats available
4. **Multiple Theatres**: City-wise theatre management

### Complex Extensions
1. **Dynamic Pricing**: Surge pricing based on demand
2. **Recommendation Engine**: Suggest shows based on history
3. **Seat Grouping**: Auto-group seats for families
4. **Real-time Notifications**: WebSocket updates
5. **Analytics Dashboard**: Booking patterns, revenue

---

## Key Interview Talking Points

### Strengths of Design
✅ Clean separation of concerns (Controllers/Services/Models)
✅ Thread-safe with mutexes and atomic operations
✅ Extensible via Strategy pattern
✅ SOLID principles followed
✅ Testable with dependency injection

### Areas to Improve for Production
🔧 Add database persistence layer
🔧 Implement distributed locking (Redis/Zookeeper)
🔧 Add comprehensive logging
🔧 Implement retry logic for transient failures
🔧 Add API rate limiting
🔧 Implement circuit breakers for external services
🔧 Add monitoring and alerting

### Complexity Analysis
- **Time**: O(1) for most operations (hash map lookups)
- **Space**: O(T × S × SH) where T=theatres, S=screens, SH=shows
- **Concurrency**: Mutex contention is O(number of concurrent bookings)

---

## Sample Interview Questions & Answers

**Q: How do you prevent two users from booking the same seat?**
A: Using a two-phase locking mechanism:
1. Acquire exclusive lock on seats (mutex-protected)
2. Validate lock before confirming booking
3. Time-based expiration for abandoned bookings

**Q: What happens if the server crashes during booking?**
A: Current in-memory design loses data. Production would use:
- Write-ahead logging (WAL)
- Database transactions
- Message queue for async processing
- Idempotency keys to prevent duplicate bookings

**Q: How to scale to multiple data centers?**
A: 
- Use distributed cache (Redis Cluster)
- Partition by theatre_id (geographic sharding)
- Eventual consistency with conflict resolution
- Strong consistency only for booking confirmation

**Q: How to handle high load during popular movie launches?**
A:
- Queue-based booking system (virtual waiting room)
- Rate limiting per user
- Horizontal scaling of API servers
- Read replicas for seat availability queries
- CDN for static content

---

## Time Estimates for Interview

- **5 min**: Problem understanding & clarifications
- **10 min**: High-level architecture diagram
- **15 min**: Core classes (Movie, Theatre, Booking)
- **15 min**: Concurrency handling & locking
- **10 min**: Payment processing & state management
- **5 min**: API/Controller layer discussion
- **5 min**: Questions & extensions

**Total: ~60 minutes** ✅

---

## Build & Run Commands

```bash
# Build
cd BookMyShow
mkdir build && cd build
cmake ..
cmake --build . --config Release

# Run
./bin/BookMyShow.exe

# Clean
rm -rf build
```

Good luck with your interview! 🚀

