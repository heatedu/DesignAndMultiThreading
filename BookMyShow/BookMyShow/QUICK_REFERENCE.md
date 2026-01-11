# BookMyShow - Quick Reference

## 📐 Current Design (3 Layers)

```
┌──────────────┐
│   MANAGERS   │  ← Business Logic (4 classes)
└──────┬───────┘
       │
┌──────▼───────┐
│  STRATEGIES  │  ← Algorithms (Strategy Pattern)
└──────┬───────┘
       │
┌──────▼───────┐
│   MODELS     │  ← Data Structures
└──────────────┘
```

### **Managers:**
1. `TheatreManager` - Theatre, Screen, Seat operations
2. `ShowManager` - Movie & Show operations  
3. `BookingManager` - Booking & availability
4. `PaymentManager` - Payment processing

### **Strategies:**
1. `ISeatLockStrategy` → `InMemoryLockStrategy` (locking)
2. `PaymentStrategy` → `DebitCardStrategy`, `UpiStrategy`

### **Models:**
Theatre → Screen → Seat
Movie → Show
User → Booking → SeatLock

---

## 🔒 Seat Locking (2-Level Protection)

### **Level 1: Strategy Pattern**

```cpp
// InMemoryLockStrategy.h (Line 11-12)
map<int, map<int, SeatLock>> locks;  // showId → (seatId → lock)
mutex locksMutex;  // 🔒 Thread safety
```

**Data Structure:**
```
locks[1] (Show #1)
    ├─ [5] → SeatLock(seat5, user="John", time=12:00, timeout=600s)
    ├─ [6] → SeatLock(seat6, user="John", time=12:00, timeout=600s)
    └─ [7] → SeatLock(seat7, user="John", time=12:00, timeout=600s)
```

### **Level 2: Multithreading Protection**

```cpp
// InMemoryLockStrategy.cpp (Line 9)
void lockSeats(...) {
    lock_guard<mutex> guard(locksMutex);  // 🔒 LOCK
    
    // Check if already locked
    for (seat in seats) {
        if (lock exists && !isExpired()) {
            throw "Already locked!";  // ❌ Conflict
        }
    }
    
    // Lock all seats atomically
    for (seat in seats) {
        locks[showId][seatId] = SeatLock(...);
    }
}  // 🔓 UNLOCK (automatic)
```

```cpp
// BookingManager.cpp (Line 10)
string createBooking(...) {
    lock_guard<mutex> lock(bookingMutex);  // 🔒 LOCK
    
    if (isAnySeatAlreadyBooked(...)) throw ...;
    lockStrategy->lockSeats(...);  // Calls above method
    
    // Create booking
    Booking* b = new Booking(...);
    return bookingId;
}  // 🔓 UNLOCK
```

---

## 🎬 Complete Flow

```
USER
  │
  ├─► bookingManager->createBooking(user, show, {5,6,7})
  │
  ▼
BOOKINGMANAGER
  │ 🔒 lock_guard(bookingMutex)
  ├─► Check if permanently booked? ✅ No
  ├─► lockStrategy->lockSeats(show, {5,6,7}, user)
  │     │
  │     ▼
  │   INMEMORYLOCKSTRATEGY
  │     │ 🔒 lock_guard(locksMutex)
  │     ├─► Check if locked? ✅ No
  │     ├─► locks[1][5] = SeatLock(...)
  │     ├─► locks[1][6] = SeatLock(...)
  │     ├─► locks[1][7] = SeatLock(...)
  │     │ 🔓 unlock
  │     │
  │     └─► return
  │
  ├─► Create Booking(id="1", status=CREATED)
  │ 🔓 unlock
  │
  └─► return bookingId="1"

STATUS: Seats 5,6,7 LOCKED for 10 minutes
```

---

## 🔥 Concurrent Booking

```
Thread A: Book [5,6,7]     Thread B: Book [7,8,9]
────────────────────────   ────────────────────────
Lock bookingMutex 🔒        (waiting...)
  Check booked ✅
  Lock locksMutex 🔒          (waiting...)
    Check seat 7 FREE ✅
    Lock seat 7 for A
  Unlock locksMutex 🔓
  Create booking
Unlock bookingMutex 🔓     Lock bookingMutex 🔒
SUCCESS ✅                   Check booked ✅
                            Lock locksMutex 🔒
                              Check seat 7 LOCKED ❌
                              throw exception
                            Unlock locksMutex 🔓
                          Unlock bookingMutex 🔓
                          FAILED ❌
```

---

## ⏰ Timeout Mechanism

```cpp
// SeatLock.cpp
bool isLockExpired() {
    auto expiry = lockTime + chrono::seconds(600);  // 10 min
    return chrono::now() >= expiry;
}
```

**Timeline:**
```
12:00:00  User locks seat 5 (timeout=600s)
12:05:00  isExpired()? → false (still valid)
12:10:00  isExpired()? → true  (EXPIRED!)
12:10:01  Seat 5 available again
```

**Validation Before Payment:**
```cpp
// BookingManager.cpp (Line 47-49)
for (seat in booking->seats) {
    if (!lockStrategy->validateLock(show, seat, user)) {
        throw "Lock expired!";  // User took too long!
    }
}
```

---

## 📊 Key Files

| File | Purpose | Lines |
|------|---------|-------|
| `InMemoryLockStrategy.h` | Lock interface | 28 |
| `InMemoryLockStrategy.cpp` | Lock implementation | 89 |
| `BookingManager.h` | Booking interface | 37 |
| `BookingManager.cpp` | Booking logic | 111 |
| `SeatLock.h/cpp` | Lock data + expiry | 60 |

---

## 🎯 Pattern Highlights

### **Strategy Pattern:**
```cpp
// Easy to swap!
ISeatLockStrategy* strategy = new InMemoryLockStrategy(600);
// Future: RedisLockStrategy, DatabaseLockStrategy
```

### **Multithreading:**
```cpp
mutex bookingMutex;  // Manager level
mutex locksMutex;    // Strategy level
atomic<int> counter; // ID generation
```

### **RAII (Resource Acquisition Is Initialization):**
```cpp
lock_guard<mutex> lock(mutex);  // Auto-unlock on scope exit
```

---

## ✅ Safety Guarantees

✅ **No Race Conditions** - Mutex protection
✅ **No Double Booking** - Atomic lock acquisition
✅ **Timeout Handling** - Auto-release after 10 min
✅ **User Validation** - Only owner can confirm
✅ **Lock Validation** - Check before payment

---

## 🚀 Demo

Run `main.cpp` (lines 185-217) to see concurrent booking in action!

**Output:**
```
=== Simulating concurrent booking attempts ===

User1 booking (seats 5,6,7) succeeded ✅
User2 booking (seats 7,8,9) failed: Seat 7 is already locked ❌

Final available seats: [4, 8, 9, 10, ...]
```

**Proof: Thread-safe locking works!** 🎉

