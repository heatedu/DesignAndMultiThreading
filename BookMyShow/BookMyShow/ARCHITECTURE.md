# BookMyShow System Architecture

## System Component Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                         CLIENT LAYER                             │
│                   (Demo in main.cpp)                             │
└───────────────────────────┬─────────────────────────────────────┘
                            │
┌───────────────────────────▼─────────────────────────────────────┐
│                     CONTROLLER LAYER                             │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐          │
│  │   Movie      │  │   Theatre    │  │    Show      │          │
│  │  Controller  │  │  Controller  │  │  Controller  │          │
│  └──────────────┘  └──────────────┘  └──────────────┘          │
│  ┌──────────────┐  ┌──────────────┐                            │
│  │   Booking    │  │   Payment    │                            │
│  │  Controller  │  │  Controller  │                            │
│  └──────────────┘  └──────────────┘                            │
└───────────────────────────┬─────────────────────────────────────┘
                            │
┌───────────────────────────▼─────────────────────────────────────┐
│                      SERVICE LAYER                               │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐          │
│  │   Movie      │  │   Theatre    │  │    Show      │          │
│  │   Service    │  │   Service    │  │   Service    │          │
│  └──────────────┘  └──────────────┘  └──────────────┘          │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐          │
│  │   Booking    │  │   Payment    │  │SeatAvailabil-│          │
│  │   Service    │  │   Service    │  │ityService    │          │
│  └──────────────┘  └──────────────┘  └──────────────┘          │
└─────────┬──────────────────┬──────────────────┬─────────────────┘
          │                  │                  │
┌─────────▼──────────┐ ┌─────▼──────────┐ ┌────▼────────────────┐
│   PROVIDER LAYER   │ │  PAYMENT LAYER │ │    MODEL LAYER      │
│  ┌──────────────┐  │ │ ┌────────────┐ │ │  ┌──────────────┐  │
│  │ISeatLock     │  │ │ │ Payment    │ │ │  │   Theatre    │  │
│  │Provider      │  │ │ │ Strategy   │ │ │  │   Screen     │  │
│  │(Interface)   │  │ │ │(Interface) │ │ │  │   Seat       │  │
│  └──────┬───────┘  │ │ └─────┬──────┘ │ │  │   Movie      │  │
│         │          │ │       │        │ │  │   Show       │  │
│  ┌──────▼───────┐  │ │ ┌─────▼──────┐ │ │  │   User       │  │
│  │SeatLock      │  │ │ │ DebitCard  │ │ │  │   Booking    │  │
│  │Provider      │  │ │ │ Strategy   │ │ │  │   SeatLock   │  │
│  │(Impl)        │  │ │ │            │ │ │  └──────────────┘  │
│  └──────────────┘  │ │ └────────────┘ │ │                    │
│                    │ │ ┌────────────┐ │ │  ┌──────────────┐  │
│                    │ │ │    UPI     │ │ │  │ BookingStatus│  │
│                    │ │ │  Strategy  │ │ │  │ SeatCategory │  │
│                    │ │ └────────────┘ │ │  │   (Enums)    │  │
│                    │ │                │ │  └──────────────┘  │
└────────────────────┘ └────────────────┘ └─────────────────────┘
```

## Data Flow: Booking a Seat

```
┌──────┐
│ User │ Wants to book seats [1, 2, 3] for Show #1
└───┬──┘
    │
    ▼
┌────────────────────┐
│BookingController   │ createBooking(user, showId=1, seats=[1,2,3])
└────────┬───────────┘
         │
         ▼
┌────────────────────┐
│BookingService      │ 1. Check if seats already booked ❌
└────────┬───────────┘ 2. Lock seats via SeatLockProvider
         │
         ├──────────────────┐
         │                  │
         ▼                  ▼
┌────────────────────┐  ┌──────────────────┐
│SeatLockProvider    │  │TheatreService    │
│                    │  │                  │
│ 🔒 Lock seats      │  │ Get Seat objects │
│ mutex.lock()       │  │ by IDs [1,2,3]   │
│ Check availability │  └──────────────────┘
│ Create SeatLocks   │
│ Store in map       │
│ mutex.unlock()     │
└────────┬───────────┘
         │
         ▼
┌────────────────────┐
│Create Booking      │
│Status: CREATED     │ Booking ID: "1"
│Return to user      │
└────────┬───────────┘
         │
    [User has 10 minutes to pay]
         │
         ▼
┌────────────────────┐
│PaymentController   │ processPayment(bookingId="1", user)
└────────┬───────────┘
         │
         ▼
┌────────────────────┐
│PaymentService      │ 1. Execute payment strategy
└────────┬───────────┘ 2. If success → confirmBooking()
         │
         ├──────────────────┐
         │                  │
         ▼                  ▼
┌────────────────────┐  ┌──────────────────┐
│PaymentStrategy     │  │BookingService    │
│ (DebitCard/UPI)    │  │                  │
│                    │  │ Validate locks   │
│ processPayment()   │  │ (still owned by  │
│ return true/false  │  │  user & not      │
└────────────────────┘  │  expired)        │
                        │                  │
                        │ ✅ Set status to │
                        │    CONFIRMED     │
                        └──────────────────┘
```

## Concurrency: Two Users, Same Seat

```
Time  User A (Thread 1)              User B (Thread 2)
────  ─────────────────              ─────────────────
t0    Request seats [5, 6, 7]       Request seats [7, 8, 9]
      │                              │
t1    │                              │
      ├─► Lock mutex                 │ (waiting for mutex)
t2    │   Check seat 7: Available    │
      │   Check seat 6: Available    │
      │   Check seat 5: Available    │
t3    │   Lock seat 5 → User A       │
      │   Lock seat 6 → User A       │
      │   Lock seat 7 → User A       │
t4    │   Unlock mutex ──────────────┼─► Lock mutex acquired
      │                              │   Check seat 7: LOCKED by A ❌
t5    │                              │   Throw Exception
      │                              │   Unlock mutex
      │   ✅ Success                  │   ❌ Failed
t6    │   Return Booking ID          │   Return Error
```

## Class Relationship Diagram

```
┌──────────┐
│ Theatre  │ 1
└────┬─────┘
     │ has
     │ *
┌────▼─────┐
│  Screen  │ 1
└────┬─────┘
     │ has
     │ *
┌────▼─────┐
│   Seat   │
└──────────┘

┌──────────┐     ┌──────────┐
│  Movie   │     │  Screen  │
└────┬─────┘     └────┬─────┘
     │                │
     │ shown in       │ hosts
     │                │
     └────────┬───────┘
              │
         ┌────▼─────┐
         │   Show   │
         └────┬─────┘
              │
              │ has
              │
         ┌────▼─────┐
         │ Booking  │ * booked by User
         └────┬─────┘   locked by SeatLock
              │
              │ contains
              │ *
         ┌────▼─────┐
         │   Seat   │
         └──────────┘
```

## State Machine: Booking Lifecycle

```
                    createBooking()
┌──────────┐      ┌─────────────────┐
│   NULL   │─────►│    CREATED      │
│          │      │ (Seats Locked)  │
└──────────┘      └────────┬─────┬──┘
                           │     │
                           │     │ timeout > 600s
                           │     │ OR user cancels
        processPayment()   │     │
        & success          │     ▼
                           │  ┌──────────┐
                           │  │ EXPIRED  │
                           │  └──────────┘
                           │
                           ▼
                    ┌─────────────┐
                    │ CONFIRMED   │
                    │(Permanently │
                    │  Booked)    │
                    └─────────────┘
```

## File Structure

```
BookMyShow/
│
├── include/
│   ├── enums/
│   │   ├── BookingStatus.h         (CREATED, CONFIRMED, EXPIRED)
│   │   └── SeatCategory.h          (SILVER, GOLD, PLATINUM)
│   │
│   ├── models/
│   │   ├── Movie.h                 (id, name, duration)
│   │   ├── Seat.h                  (id, row, category)
│   │   ├── Screen.h                (id, name, theatre, seats[])
│   │   ├── Theatre.h               (id, name, screens[])
│   │   ├── Show.h                  (id, movie, screen, time)
│   │   ├── User.h                  (name, email)
│   │   ├── Booking.h               (id, show, user, seats[], status)
│   │   └── SeatLock.h              (seat, show, user, timeout, lockTime)
│   │
│   ├── providers/
│   │   ├── ISeatLockProvider.h     (interface)
│   │   └── SeatLockProvider.h      (mutex-based implementation)
│   │
│   ├── payment/
│   │   ├── PaymentStrategy.h       (interface)
│   │   ├── DebitCardStrategy.h     (returns true)
│   │   └── UpiStrategy.h           (returns false - demo)
│   │
│   ├── services/
│   │   ├── MovieService.h          (create, get movies)
│   │   ├── TheatreService.h        (manage theatres/screens/seats)
│   │   ├── ShowService.h           (create, get shows)
│   │   ├── BookingService.h        (create, confirm bookings)
│   │   ├── PaymentService.h        (process payments)
│   │   └── SeatAvailabilityService.h (check available seats)
│   │
│   └── controllers/
│       ├── MovieController.h       (API for movies)
│       ├── TheatreController.h     (API for theatres)
│       ├── ShowController.h        (API for shows)
│       ├── BookingController.h     (API for bookings)
│       └── PaymentController.h     (API for payments)
│
├── src/                            (All .cpp implementations)
│   ├── models/
│   ├── providers/
│   ├── payment/
│   ├── services/
│   └── controllers/
│
├── main.cpp                        (Demo application)
├── CMakeLists.txt                  (Build configuration)
├── build.ps1                       (Build script)
├── README.md                       (Project documentation)
├── INTERVIEW_GUIDE.md              (Interview prep)
└── ARCHITECTURE.md                 (This file)
```

## Key Design Principles

### 1. **Single Responsibility Principle (SRP)**
- Each class has one reason to change
- `MovieService` only handles movies
- `BookingService` only handles bookings

### 2. **Open/Closed Principle (OCP)**
- Open for extension, closed for modification
- New payment methods via `PaymentStrategy` interface
- New lock providers via `ISeatLockProvider` interface

### 3. **Liskov Substitution Principle (LSP)**
- `DebitCardStrategy` and `UpiStrategy` can replace `PaymentStrategy`
- `SeatLockProvider` can be replaced with `RedisLockProvider`

### 4. **Interface Segregation Principle (ISP)**
- Small, focused interfaces
- `ISeatLockProvider` only has locking methods
- `PaymentStrategy` only has payment methods

### 5. **Dependency Inversion Principle (DIP)**
- Depend on abstractions, not concretions
- `BookingService` depends on `ISeatLockProvider` (interface)
- `PaymentService` depends on `PaymentStrategy` (interface)

## Performance Characteristics

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Create Movie | O(1) | O(1) |
| Create Theatre | O(1) | O(1) |
| Create Screen | O(1) | O(1) |
| Create Seat | O(1) | O(1) |
| Create Show | O(1) | O(1) |
| Get Available Seats | O(n) | O(n) |
| Lock Seats | O(m) | O(m) |
| Create Booking | O(m) | O(1) |
| Confirm Booking | O(m) | O(1) |

Where:
- n = number of seats in screen
- m = number of seats being booked

## Thread Safety Analysis

| Component | Thread-Safe? | Mechanism |
|-----------|-------------|-----------|
| SeatLockProvider | ✅ Yes | `std::mutex` |
| BookingService | ✅ Yes | `std::mutex` + atomic counter |
| PaymentService | ✅ Yes | `std::mutex` |
| MovieService | ⚠️ Partial | Atomic counter, no mutex for map |
| TheatreService | ⚠️ Partial | Atomic counter, no mutex for map |
| ShowService | ⚠️ Partial | Atomic counter, no mutex for map |

**Note**: Services without mutexes are safe for read operations but would need protection for concurrent writes in production.

---

## Memory Layout

```
Typical Memory Usage (50 seats, 1 show, 3 bookings):

┌─────────────────────┐
│ MovieService        │ ~1 KB  (1 movie)
├─────────────────────┤
│ TheatreService      │ ~5 KB  (1 theatre, 1 screen, 50 seats)
├─────────────────────┤
│ ShowService         │ ~1 KB  (1 show)
├─────────────────────┤
│ BookingService      │ ~2 KB  (3 bookings)
├─────────────────────┤
│ SeatLockProvider    │ ~5 KB  (up to 50 locks)
└─────────────────────┘
Total: ~14 KB (very lightweight!)

For 1000 shows × 50 seats = 50,000 seats:
Estimated: ~700 KB - 1 MB (still very manageable)
```

---

This architecture demonstrates:
- ✅ Clean separation of concerns
- ✅ SOLID principles
- ✅ Design patterns in action
- ✅ Thread-safe concurrent operations
- ✅ Scalable and maintainable structure

