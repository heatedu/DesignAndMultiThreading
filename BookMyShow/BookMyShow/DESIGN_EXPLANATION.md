# BookMyShow - Current Design & Locking Mechanism

## 🏗️ Current Architecture

### **3-Layer Design (Simplified)**

```
┌─────────────────────────────────────────┐
│           APPLICATION LAYER              │
│              (main.cpp)                  │
└───────────────┬─────────────────────────┘
                │
┌───────────────▼─────────────────────────┐
│         MANAGERS LAYER                   │
│      (Business Logic)                    │
│                                          │
│  ┌────────────────┐  ┌────────────────┐ │
│  │ TheatreManager │  │  ShowManager   │ │
│  │ - createTheatre│  │ - createMovie  │ │
│  │ - createScreen │  │ - createShow   │ │
│  │ - createSeat   │  │ - getShow      │ │
│  └────────────────┘  └────────────────┘ │
│                                          │
│  ┌────────────────┐  ┌────────────────┐ │
│  │ BookingManager │  │ PaymentManager │ │
│  │ - createBooking│  │ - processPayment│ │
│  │ - getAvailable │  │                │ │
│  │ - confirmBook  │  │                │ │
│  └────────────────┘  └────────────────┘ │
└───────────────┬─────────────────────────┘
                │
┌───────────────▼─────────────────────────┐
│        STRATEGIES LAYER                  │
│      (Algorithms - Strategy Pattern)     │
│                                          │
│  ┌────────────────────────────────────┐ │
│  │   ISeatLockStrategy (Interface)    │ │
│  │   ┌──────────────────────────────┐ │ │
│  │   │  InMemoryLockStrategy        │ │ │
│  │   │  - lockSeats()  🔒           │ │ │
│  │   │  - unlockSeats()             │ │ │
│  │   │  - validateLock()            │ │ │
│  │   │  - getLockedSeats()          │ │ │
│  │   └──────────────────────────────┘ │ │
│  └────────────────────────────────────┘ │
│                                          │
│  ┌────────────────────────────────────┐ │
│  │   PaymentStrategy (Interface)      │ │
│  │   ┌──────────────────────────────┐ │ │
│  │   │  DebitCardStrategy           │ │ │
│  │   │  UpiStrategy                 │ │ │
│  │   └──────────────────────────────┘ │ │
│  └────────────────────────────────────┘ │
└───────────────┬─────────────────────────┘
                │
┌───────────────▼─────────────────────────┐
│           MODELS LAYER                   │
│         (Data Structures)                │
│                                          │
│  Theatre → Screen → Seat                 │
│  Movie → Show                            │
│  User → Booking → SeatLock               │
└──────────────────────────────────────────┘
```

---

## 🔒 Seat Locking Mechanism (2-Level Protection)

### **Level 1: Strategy Pattern (ISeatLockStrategy)**

The locking logic is **abstracted** using Strategy pattern:

```cpp
// Interface
class ISeatLockStrategy {
public:
    virtual void lockSeats(Show, vector<Seat>, User) = 0;
    virtual void unlockSeats(Show, vector<Seat>, User) = 0;
    virtual bool validateLock(Show, Seat, User) = 0;
    virtual vector<Seat> getLockedSeats(Show) = 0;
};

// Concrete implementation
class InMemoryLockStrategy : public ISeatLockStrategy {
private:
    map<int, map<int, SeatLock>> locks;  // showId -> (seatId -> SeatLock)
    mutex locksMutex;  // 🔒 Thread safety!
    int lockTimeout;   // 600 seconds (10 minutes)
    
public:
    void lockSeats(...) override {
        lock_guard<mutex> guard(locksMutex);  // 🔒 Acquire mutex
        
        // Check if seats already locked
        for (seat in seats) {
            if (lock exists && !isExpired()) {
                throw "Seat already locked!";
            }
        }
        
        // Lock all seats atomically
        auto now = currentTime();
        for (seat in seats) {
            locks[showId][seatId] = SeatLock(seat, show, 600, now, user);
        }
    }
};
```

**Why Strategy Pattern?**
- ✅ Easy to swap implementations (InMemory → Redis → Database)
- ✅ Testable (can mock the strategy)
- ✅ Single Responsibility (locking logic isolated)

---

### **Level 2: Multithreading Protection (Mutex)**

#### **A. Manager-Level Mutex**

```cpp
class BookingManager {
private:
    mutex bookingMutex;  // 🔒 Protects booking operations
    shared_ptr<ISeatLockStrategy> lockStrategy;
    
public:
    string createBooking(User user, Show show, vector<Seat> seats) {
        lock_guard<mutex> lock(bookingMutex);  // 🔒 LOCK
        
        // Critical section - only ONE thread at a time
        if (isAnySeatAlreadyBooked(show, seats)) {
            throw "Already booked!";
        }
        
        lockStrategy->lockSeats(show, seats, user);  // Delegate to strategy
        
        string id = generateId();
        Booking* booking = new Booking(id, show, user, seats);
        bookings[id] = booking;
        
        return id;
    }  // 🔓 UNLOCK (automatic via RAII)
};
```

#### **B. Strategy-Level Mutex**

```cpp
class InMemoryLockStrategy {
private:
    mutex locksMutex;  // 🔒 Protects lock data structure
    
public:
    void lockSeats(...) {
        lock_guard<mutex> guard(locksMutex);  // 🔒 LOCK
        
        // Critical section - only ONE thread can modify locks
        // Check and acquire locks atomically
        
    }  // 🔓 UNLOCK
};
```

**Result: 2 levels of mutex protection!**

---

## 🎬 Complete Booking Flow with Locking

### **Scenario: John wants to book seats 5, 6, 7**

```cpp
// In main.cpp
User john("John", "john@email.com");
vector<shared_ptr<Seat>> seats = {seat5, seat6, seat7};
string bookingId = bookingManager->createBooking(john, show, seats);
```

### **Step-by-Step Execution:**

```
┌─────────────────────────────────────────────────────┐
│ 1. USER CALLS: bookingManager->createBooking()      │
└────────────────────┬────────────────────────────────┘
                     │
┌────────────────────▼────────────────────────────────┐
│ 2. BOOKINGMANAGER                                    │
│    lock_guard<mutex> lock(bookingMutex);  🔒 LOCK   │
│                                                      │
│    // Check if seats already PERMANENTLY booked     │
│    if (isAnySeatAlreadyBooked(show, {5,6,7})) {    │
│        throw "Already booked!";                      │
│    }                                                 │
│    // ✅ All seats available                         │
└────────────────────┬────────────────────────────────┘
                     │
┌────────────────────▼────────────────────────────────┐
│ 3. DELEGATE TO STRATEGY                              │
│    lockStrategy->lockSeats(show, {5,6,7}, john);    │
└────────────────────┬────────────────────────────────┘
                     │
┌────────────────────▼────────────────────────────────┐
│ 4. INMEMORYLOCKSTRATEGY                              │
│    lock_guard<mutex> guard(locksMutex);  🔒 LOCK    │
│                                                      │
│    // Check if seats TEMPORARILY locked             │
│    for (seat in {5,6,7}) {                          │
│        if (locks[showId][seatId] exists) {          │
│            if (!lock.isExpired()) {                 │
│                throw "Seat already locked!"; ❌      │
│            }                                         │
│        }                                            │
│    }                                                │
│    // ✅ All seats free                              │
│                                                      │
│    // Lock all seats ATOMICALLY                     │
│    auto now = chrono::now();                        │
│    locks[1][5] = SeatLock(seat5, show, 600, now, john); │
│    locks[1][6] = SeatLock(seat6, show, 600, now, john); │
│    locks[1][7] = SeatLock(seat7, show, 600, now, john); │
│                                                      │
│    🔓 UNLOCK (locksMutex released)                  │
└────────────────────┬────────────────────────────────┘
                     │
┌────────────────────▼────────────────────────────────┐
│ 5. BOOKINGMANAGER (continued)                        │
│    string id = "1";                                  │
│    Booking* booking = new Booking(id, show, john,   │
│                                    {5,6,7},          │
│                                    CREATED);         │
│    bookings["1"] = booking;                          │
│                                                      │
│    🔓 UNLOCK (bookingMutex released)                │
│    return "1";                                       │
└────────────────────┬────────────────────────────────┘
                     │
┌────────────────────▼────────────────────────────────┐
│ 6. RESULT                                            │
│    ✅ Booking created with ID: "1"                   │
│    ✅ Seats 5, 6, 7 LOCKED for John                  │
│    ⏰ Timeout: 10 minutes                            │
│    📌 Status: CREATED (not confirmed yet)            │
└──────────────────────────────────────────────────────┘
```

---

## 🔥 Concurrent Booking Protection

### **Scenario: Two users try to book seat 7 simultaneously**

```
Time  User A (Thread 1)              User B (Thread 2)
────  ─────────────────              ─────────────────
t0    Book seats [5, 6, 7]          Book seats [7, 8, 9]
      │                              │
t1    │                              │
      ├─► Lock bookingMutex 🔒       │ (waiting...)
t2    │   Check booked seats ✅      │
      │   Call lockStrategy          │
      │   ├─► Lock locksMutex 🔒     │ (waiting...)
t3    │   │   Check seat 7: FREE ✅  │
      │   │   Check seat 6: FREE ✅  │
      │   │   Check seat 5: FREE ✅  │
t4    │   │   Lock seat 5 → User A   │
      │   │   Lock seat 6 → User A   │
      │   │   Lock seat 7 → User A ✅ │
t5    │   │   Unlock locksMutex 🔓──┤
      │   Create booking             │
      │   Unlock bookingMutex 🔓─────┼─► Lock bookingMutex 🔒
t6    │   ✅ SUCCESS                  │   Check booked seats ✅
      │                              │   Call lockStrategy
      │                              │   ├─► Lock locksMutex 🔒
t7    │                              │   │   Check seat 7: LOCKED by A ❌
      │                              │   │   Throw exception!
      │                              │   │   Unlock locksMutex 🔓
      │                              │   Unlock bookingMutex 🔓
t8    │                              │   ❌ FAILED
```

**Key Points:**
- ⏰ Mutex ensures **sequential execution**
- 🔒 Only ONE thread can lock seats at a time
- ✅ Thread A succeeds first
- ❌ Thread B fails (seat 7 already locked)
- 🎯 **No race condition!**

---

## 🕐 Timeout Mechanism

### **SeatLock Model:**

```cpp
class SeatLock {
private:
    shared_ptr<Seat> seat;
    shared_ptr<Show> show;
    User lockedBy;
    chrono::time_point lockTime;
    int timeoutInSeconds;  // 600 (10 minutes)
    
public:
    bool isLockExpired() const {
        auto expirationTime = lockTime + chrono::seconds(timeoutInSeconds);
        auto currentTime = chrono::system_clock::now();
        return currentTime >= expirationTime;  // true if expired
    }
};
```

### **How Timeout Works:**

```
t=0     User locks seat 5
        lockTime = 12:00:00 PM
        timeout = 600 seconds (10 min)
        
t=300   User still has 5 minutes
        currentTime = 12:05:00 PM
        expirationTime = 12:10:00 PM
        isExpired? → false ✅ Lock valid
        
t=600   Timeout reached!
        currentTime = 12:10:00 PM
        expirationTime = 12:10:00 PM
        isExpired? → true ❌ Lock expired
        
t=601   Another user can now lock seat 5
        Old lock is ignored (expired)
```

### **Validation Before Payment:**

```cpp
void BookingManager::confirmBooking(string bookingId, User user) {
    lock_guard<mutex> lock(bookingMutex);
    
    auto booking = getBooking(bookingId);
    
    // Validate each seat lock
    for (seat in booking->seats) {
        if (!lockStrategy->validateLock(show, seat, user)) {
            throw "Lock expired or invalid!";  // ❌ Too late!
        }
    }
    
    booking->confirmBooking();  // ✅ Status: CONFIRMED
}
```

---

## 📊 Locking States

```
SEAT STATE MACHINE:

┌──────────┐
│ AVAILABLE│ ◄─────────────────────┐
└────┬─────┘                       │
     │ lockSeats()                 │
     │                             │ timeout (10 min)
     ▼                             │
┌──────────┐                       │
│  LOCKED  │───────────────────────┘
│ (temp)   │
└────┬─────┘
     │ confirmBooking()
     │
     ▼
┌──────────┐
│  BOOKED  │ (permanent)
│(confirmed)│
└──────────┘
```

---

## 🎯 Key Design Decisions

### **1. Why Strategy Pattern for Locking?**

```cpp
// Easy to swap implementations
auto lockStrategy = make_shared<InMemoryLockStrategy>(600);
// Future:
// auto lockStrategy = make_shared<RedisLockStrategy>("redis://...");
// auto lockStrategy = make_shared<DatabaseLockStrategy>(db);
```

**Benefits:**
- ✅ Can use Redis for distributed systems
- ✅ Can use Database for persistence
- ✅ Easy to test with mock strategies

---

### **2. Why Two-Level Mutex?**

```cpp
// Level 1: BookingManager mutex
mutex bookingMutex;  // Protects booking business logic

// Level 2: LockStrategy mutex
mutex locksMutex;    // Protects lock data structure
```

**Benefits:**
- ✅ **Separation of concerns**: Booking logic vs Lock logic
- ✅ **Fine-grained locking**: Only lock what's needed
- ✅ **Better performance**: Multiple bookings for different shows can proceed

---

### **3. Why Atomic ID Generation?**

```cpp
atomic<int> bookingCounter;

string createBooking(...) {
    string id = to_string(++bookingCounter);  // Thread-safe increment
}
```

**Benefits:**
- ✅ No mutex needed for ID generation
- ✅ Guaranteed unique IDs
- ✅ Better performance

---

## 📝 Summary

### **Current Design:**
```
Managers (Business Logic)
    ↓
Strategies (Locking + Payment)
    ↓
Models (Data)
```

### **Locking Mechanism:**

| Level | Component | Protection | Purpose |
|-------|-----------|-----------|---------|
| **1** | BookingManager | `bookingMutex` | Booking business logic |
| **2** | LockStrategy | `locksMutex` | Lock data structure |
| **3** | SeatLock | `timeout` | Time-based expiration |
| **4** | Validation | `validateLock()` | Check before payment |

### **Thread Safety:**
✅ Mutex at manager level
✅ Mutex at strategy level
✅ Atomic counters for IDs
✅ Time-based lock expiration
✅ Lock validation before confirmation

### **Patterns Used:**
✅ **Strategy Pattern** - Swappable lock/payment strategies
✅ **Multithreading** - Concurrent booking with mutexes
✅ **RAII** - Automatic mutex unlock with lock_guard

---

## 🚀 See It In Action

Run `main.cpp` and look at lines 185-217:
```cpp
// Concurrent booking simulation
thread t1([&]() { bookingManager->createBooking(user1, show, {5,6,7}); });
thread t2([&]() { bookingManager->createBooking(user2, show, {7,8,9}); });

t1.join();
t2.join();

// Result: One succeeds, one fails (seat 7 conflict)
```

**Output:**
```
User1 booking (seats 5,6,7) succeeded with Booking ID: 2
User2 booking (seats 7,8,9) failed: Seat 7 is already locked
```

✅ **Thread-safe! No race conditions!** 🎉

