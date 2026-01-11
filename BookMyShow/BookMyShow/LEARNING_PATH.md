# BookMyShow - Learning Path (Step-by-Step)

## 🎯 Learning Strategy

**Bottom-Up Approach:** Start with simple data structures, then move to strategies, and finally business logic.

```
Level 1: Enums & Models (Data) → Simple
Level 2: Strategies (Patterns) → Medium  
Level 3: Managers (Logic) → Complex
Level 4: Application (Demo) → Integration
```

---

## 📚 **LEVEL 1: Understand Data Structures (Models & Enums)**

Start here to understand **WHAT** the system stores.

### **Step 1: Enums (5 minutes)**

#### `include/enums/SeatCategory.h`
```cpp
enum class SeatCategory {
    SILVER,    // ₹200
    GOLD,      // ₹300
    PLATINUM   // ₹500
};
```
**What to learn:** Different seat types

#### `include/enums/BookingStatus.h`
```cpp
enum class BookingStatus {
    CREATED,    // Locked, awaiting payment
    CONFIRMED,  // Paid and confirmed
    EXPIRED     // Timeout or cancelled
};
```
**What to learn:** Booking lifecycle states

---

### **Step 2: Basic Models (15 minutes)**

#### 📄 `include/models/User.h` + `src/models/User.cpp`
```cpp
class User {
    string name;
    string emailAddress;
};
```
**What to learn:**
- Simple data holder
- `operator==` for comparison (line 17 in .cpp)

---

#### 📄 `include/models/Movie.h` + `src/models/Movie.cpp`
```cpp
class Movie {
    int movieId;
    string movieName;
    int movieDurationInMinutes;
};
```
**What to learn:**
- Basic getters (lines 9-17 in .cpp)
- No complex logic, just data

---

#### 📄 `include/models/Seat.h` + `src/models/Seat.cpp`
```cpp
class Seat {
    int seatId;
    int row;
    SeatCategory category;
};
```
**What to learn:**
- Uses `SeatCategory` enum
- `operator==` compares by `seatId` (line 19 in .cpp)

---

### **Step 3: Hierarchical Models (15 minutes)**

#### 📄 `include/models/Theatre.h` + `src/models/Theatre.cpp`
```cpp
class Theatre {
    int id;
    string name;
    vector<shared_ptr<Screen>> screens;  // Has many screens
    
    void addScreen(Screen);
};
```
**What to learn:**
- Container pattern (has many screens)
- Uses `shared_ptr` for ownership

---

#### 📄 `include/models/Screen.h` + `src/models/Screen.cpp`
```cpp
class Screen {
    int id;
    string name;
    shared_ptr<Theatre> theatre;       // Belongs to theatre
    vector<shared_ptr<Seat>> seats;    // Has many seats
    
    void addSeat(Seat);
};
```
**What to learn:**
- Bidirectional relationship (knows its theatre)
- Container for seats

**Mental Model:**
```
Theatre → Screen → Seat
(1)       (many)   (many)
```

---

#### 📄 `include/models/Show.h` + `src/models/Show.cpp`
```cpp
class Show {
    int id;
    shared_ptr<Movie> movie;
    shared_ptr<Screen> screen;
    chrono::time_point startTime;
    int durationInMinutes;
};
```
**What to learn:**
- Connects Movie + Screen + Time
- `operator==` for comparison (line 29 in .cpp)

**Mental Model:**
```
Show = Movie + Screen + Time
```

---

### **Step 4: Complex Models (20 minutes)**

#### 📄 `include/models/Booking.h` + `src/models/Booking.cpp`
```cpp
class Booking {
    string id;
    shared_ptr<Show> show;
    vector<shared_ptr<Seat>> seatsBooked;
    User user;
    BookingStatus status;  // CREATED/CONFIRMED/EXPIRED
    
    void confirmBooking();  // CREATED → CONFIRMED
    bool isConfirmed();
};
```
**What to learn:**
- State machine (line 12-20 in .cpp)
- Validation in `confirmBooking()` (line 16 in .cpp)
- Uses `BookingStatus` enum

**Focus on:**
```cpp
// Line 16 in Booking.cpp
void confirmBooking() {
    if (status != CREATED) {
        throw "Cannot confirm!";  // Only CREATED can become CONFIRMED
    }
    status = CONFIRMED;
}
```

---

#### 📄 `include/models/SeatLock.h` + `src/models/SeatLock.cpp` ⭐ **IMPORTANT**
```cpp
class SeatLock {
    shared_ptr<Seat> seat;
    shared_ptr<Show> show;
    User lockedBy;
    chrono::time_point lockTime;
    int timeoutInSeconds;  // 600 = 10 min
    
    bool isLockExpired();  // ⭐ KEY METHOD
};
```
**What to learn:**
- Time-based expiration (lines 9-12 in .cpp)
- Current time vs lock time comparison

**Focus on:**
```cpp
// Line 9-12 in SeatLock.cpp
bool isLockExpired() {
    auto expiry = lockTime + chrono::seconds(timeout);
    auto now = chrono::system_clock::now();
    return now >= expiry;  // true if expired
}
```

---

## 🎨 **LEVEL 2: Understand Design Patterns (Strategies)**

Now learn **HOW** algorithms are swappable.

### **Step 5: Strategy Pattern - Locking (30 minutes)** ⭐ **CRITICAL**

#### 📄 `include/strategies/ISeatLockStrategy.h`
```cpp
class ISeatLockStrategy {
    virtual void lockSeats(...) = 0;
    virtual void unlockSeats(...) = 0;
    virtual bool validateLock(...) = 0;
    virtual vector<Seat> getLockedSeats(...) = 0;
};
```
**What to learn:**
- Interface (pure virtual functions)
- Strategy pattern contract
- Any class implementing this can be used

---

#### 📄 `include/strategies/InMemoryLockStrategy.h` + `src/strategies/InMemoryLockStrategy.cpp` ⭐⭐⭐ **MOST IMPORTANT**
```cpp
class InMemoryLockStrategy : public ISeatLockStrategy {
    map<int, map<int, SeatLock>> locks;  // showId → (seatId → lock)
    mutex locksMutex;  // 🔒 Thread safety
    int lockTimeout;
};
```

**What to learn:**
- Data structure for storing locks (line 11 in .h)
- Mutex for thread safety (line 12 in .h)

**Focus on these methods in .cpp:**

1. **`lockSeats()` (lines 6-33)** ⭐⭐⭐
```cpp
void lockSeats(...) {
    lock_guard<mutex> guard(locksMutex);  // 🔒 Line 9
    
    // Line 18-25: Check if already locked
    for (seat in seats) {
        if (lock exists && !isExpired()) {
            throw "Already locked!";
        }
    }
    
    // Line 27-32: Lock all seats
    auto now = chrono::now();
    for (seat in seats) {
        locks[showId][seatId] = SeatLock(seat, show, timeout, now, user);
    }
}
```

2. **`validateLock()` (lines 52-65)** ⭐⭐
```cpp
bool validateLock(...) {
    lock_guard<mutex> guard(locksMutex);  // 🔒
    
    // Line 61-63: Check lock validity
    return lock exists 
        && !lock.isExpired() 
        && lock.lockedBy == user;
}
```

3. **`getLockedSeats()` (lines 67-82)** ⭐
```cpp
vector<Seat> getLockedSeats(Show show) {
    lock_guard<mutex> guard(locksMutex);  // 🔒
    
    // Return only non-expired locks
    for (lock in locks[showId]) {
        if (!lock.isExpired()) {
            result.push_back(lock.getSeat());
        }
    }
}
```

**Key Concepts:**
- `lock_guard<mutex>` - Automatic lock/unlock (RAII)
- Nested map structure
- Expiration checking

---

### **Step 6: Strategy Pattern - Payment (10 minutes)**

#### 📄 `include/strategies/PaymentStrategy.h`
```cpp
class PaymentStrategy {
    virtual bool processPayment() = 0;
};
```
**What to learn:** Simple strategy interface

#### 📄 `include/strategies/DebitCardStrategy.h` + `.cpp`
```cpp
bool processPayment() {
    return true;  // Simulate success
}
```

#### 📄 `include/strategies/UpiStrategy.h` + `.cpp`
```cpp
bool processPayment() {
    return false;  // Simulate failure
}
```
**What to learn:** Easy to add new payment methods

---

## 🏢 **LEVEL 3: Understand Business Logic (Managers)**

Now learn **WHAT** the system does.

### **Step 7: Simple Managers (20 minutes)**

#### 📄 `include/managers/TheatreManager.h` + `src/managers/TheatreManager.cpp`
```cpp
class TheatreManager {
    map<int, shared_ptr<Theatre>> theatres;
    map<int, shared_ptr<Screen>> screens;
    map<int, shared_ptr<Seat>> seats;
    
    atomic<int> theatreCounter;  // Thread-safe ID
    mutex theatreMutex;
    
    int createTheatre(string name);
    int createScreen(string name, int theatreId);
    int createSeat(int row, SeatCategory cat, int screenId);
};
```

**What to learn:**
- Storage maps (lines 16-18 in .h)
- Atomic counters for IDs (lines 20-22 in .h)
- Mutex for thread safety (line 24 in .h)

**Focus on:**
```cpp
// Lines 9-14 in TheatreManager.cpp
int createTheatre(string name) {
    lock_guard<mutex> lock(theatreMutex);  // 🔒
    
    int id = ++theatreCounter;  // Atomic increment
    Theatre* t = new Theatre(id, name);
    theatres[id] = t;
    return id;
}
```

---

#### 📄 `include/managers/ShowManager.h` + `src/managers/ShowManager.cpp`
```cpp
class ShowManager {
    map<int, shared_ptr<Movie>> movies;
    map<int, shared_ptr<Show>> shows;
    
    atomic<int> movieCounter;
    atomic<int> showCounter;
    mutex showMutex;
    
    int createMovie(...);
    int createShow(...);
};
```
**What to learn:** Similar pattern to TheatreManager

---

### **Step 8: Complex Manager - Booking (40 minutes)** ⭐⭐⭐ **MOST IMPORTANT**

#### 📄 `include/managers/BookingManager.h` + `src/managers/BookingManager.cpp`

**Start with the header to see the interface (BookingManager.h):**
```cpp
class BookingManager {
    map<string, shared_ptr<Booking>> bookings;
    shared_ptr<ISeatLockStrategy> lockStrategy;  // 🔑 Strategy!
    atomic<int> bookingCounter;
    mutex bookingMutex;
    
    string createBooking(...);      // ⭐⭐⭐
    void confirmBooking(...);       // ⭐⭐
    vector<Seat> getAvailableSeats(...);  // ⭐
};
```

**Now read the implementation (BookingManager.cpp):**

1. **`createBooking()` (lines 8-26)** ⭐⭐⭐ **START HERE**
```cpp
string createBooking(User user, Show show, vector<Seat> seats) {
    lock_guard<mutex> lock(bookingMutex);  // 🔒 Line 10
    
    // Line 13-15: Check permanently booked
    if (isAnySeatAlreadyBooked(show, seats)) {
        throw "Already booked!";
    }
    
    // Line 18: Lock seats (delegates to strategy)
    lockStrategy->lockSeats(show, seats, user);
    
    // Line 21-23: Create booking
    string id = to_string(++bookingCounter);
    Booking* b = new Booking(id, show, user, seats);
    bookings[id] = b;
    
    return id;
}
```

2. **`confirmBooking()` (lines 36-54)** ⭐⭐
```cpp
void confirmBooking(string bookingId, User user) {
    lock_guard<mutex> lock(bookingMutex);  // 🔒 Line 37
    
    // Line 42-44: Validate user
    if (booking->user != user) {
        throw "Wrong user!";
    }
    
    // Line 47-51: Validate locks (IMPORTANT!)
    for (seat in booking->seats) {
        if (!lockStrategy->validateLock(show, seat, user)) {
            throw "Lock expired!";  // User took too long!
        }
    }
    
    // Line 53: Confirm
    booking->confirmBooking();  // CREATED → CONFIRMED
}
```

3. **`getAvailableSeats()` (lines 56-78)** ⭐
```cpp
vector<Seat> getAvailableSeats(Show show) {
    vector<Seat> allSeats = show->screen->getSeats();
    vector<Seat> booked = getBookedSeats(show);
    vector<Seat> locked = lockStrategy->getLockedSeats(show);
    
    // Remove booked and locked from all
    return allSeats - booked - locked;
}
```

**Key Concepts:**
- Delegates locking to strategy (line 18)
- Two-level checking (booked + locked)
- Lock validation before confirmation

---

### **Step 9: Payment Manager (10 minutes)**

#### 📄 `include/managers/PaymentManager.h` + `src/managers/PaymentManager.cpp`
```cpp
class PaymentManager {
    shared_ptr<PaymentStrategy> paymentStrategy;
    shared_ptr<BookingManager> bookingManager;
    
    void processPayment(string bookingId, User user);
};
```

**Focus on:**
```cpp
// Lines 9-15 in PaymentManager.cpp
void processPayment(string bookingId, User user) {
    lock_guard<mutex> lock(paymentMutex);  // 🔒
    
    if (paymentStrategy->processPayment()) {
        bookingManager->confirmBooking(bookingId, user);  // ✅
    } else {
        recordPaymentFailure(bookingId, user);  // ❌
    }
}
```

---

## 🚀 **LEVEL 4: See It All Together (Application)**

### **Step 10: Main Application (30 minutes)** ⭐ **FINAL INTEGRATION**

#### 📄 `main.cpp`

**Read in this order:**

1. **Setup (lines 59-77)** - How managers are created
```cpp
auto theatreManager = make_shared<TheatreManager>();
auto showManager = make_shared<ShowManager>();

auto lockStrategy = make_shared<InMemoryLockStrategy>(600);
auto bookingManager = make_shared<BookingManager>(lockStrategy);

auto paymentStrategy = make_shared<DebitCardStrategy>();
auto paymentManager = make_shared<PaymentManager>(paymentStrategy, bookingManager);
```

2. **Theatre Setup (lines 79-121)** - Creating infrastructure
```cpp
int theatreId = theatreManager->createTheatre("PVR");
int screenId = theatreManager->createScreen("Screen 1", theatreId);
int seatId = theatreManager->createSeat(1, SeatCategory::GOLD, screenId);
```

3. **Booking Flow (lines 146-177)** - Sequential booking
```cpp
User john("John", "john@email.com");
vector<Seat> seats = {seat1, seat2, seat3};

// Book
string bookingId = bookingManager->createBooking(john, show, seats);

// Pay
paymentManager->processPayment(bookingId, john);

// Verify
Booking* b = bookingManager->getBooking(bookingId);
cout << b->isConfirmed();  // true
```

4. **Concurrent Booking (lines 185-217)** ⭐⭐⭐ **MOST IMPORTANT**
```cpp
thread t1([&]() {
    bookingManager->createBooking(userA, show, {5,6,7});
});

thread t2([&]() {
    bookingManager->createBooking(userB, show, {7,8,9});  // Conflict!
});

t1.join();
t2.join();

// Output: One succeeds, one fails (seat 7 conflict)
```

---

## 📝 **COMPLETE READING SEQUENCE**

Copy this checklist:

```
LEVEL 1: MODELS (60 min)
□ include/enums/SeatCategory.h
□ include/enums/BookingStatus.h
□ include/models/User.h + .cpp
□ include/models/Movie.h + .cpp
□ include/models/Seat.h + .cpp
□ include/models/Theatre.h + .cpp
□ include/models/Screen.h + .cpp
□ include/models/Show.h + .cpp
□ include/models/Booking.h + .cpp ⭐
□ include/models/SeatLock.h + .cpp ⭐⭐

LEVEL 2: STRATEGIES (40 min)
□ include/strategies/ISeatLockStrategy.h
□ include/strategies/InMemoryLockStrategy.h + .cpp ⭐⭐⭐
□ include/strategies/PaymentStrategy.h
□ include/strategies/DebitCardStrategy.h + .cpp
□ include/strategies/UpiStrategy.h + .cpp

LEVEL 3: MANAGERS (70 min)
□ include/managers/TheatreManager.h + .cpp
□ include/managers/ShowManager.h + .cpp
□ include/managers/BookingManager.h + .cpp ⭐⭐⭐
□ include/managers/PaymentManager.h + .cpp

LEVEL 4: APPLICATION (30 min)
□ main.cpp ⭐⭐⭐

TOTAL: ~3 hours
```

---

## 🎯 **Key Files to Master**

If you have limited time, focus on these **5 critical files**:

1. ⭐⭐⭐ `src/strategies/InMemoryLockStrategy.cpp` (Locking logic)
2. ⭐⭐⭐ `src/managers/BookingManager.cpp` (Booking logic)
3. ⭐⭐ `src/models/SeatLock.cpp` (Timeout mechanism)
4. ⭐⭐ `src/models/Booking.cpp` (State machine)
5. ⭐⭐⭐ `main.cpp` (lines 185-217: Concurrent booking)

---

## 💡 **Study Tips**

### **For Each File:**
1. ✅ Read the `.h` file first (interface)
2. ✅ Then read the `.cpp` file (implementation)
3. ✅ Focus on lines mentioned in this guide
4. ✅ Trace one example through the code

### **Use a Debugger:**
1. Set breakpoint at `main.cpp` line 162
2. Step through `createBooking()`
3. Watch it call `lockSeats()`
4. See the mutex lock/unlock

### **Draw Diagrams:**
```
User → BookingManager → LockStrategy → locks[show][seat]
       (mutex 🔒)        (mutex 🔒)
```

---

**Good luck! Start with Level 1 and work your way up!** 🚀

