# BookMyShow - Movie Ticket Booking System (C++)

A low-level design implementation of a movie ticket booking system similar to BookMyShow, featuring concurrent booking handling, seat locking mechanisms, and payment processing.

## 🎯 Features

- **Multiple Screens & Shows**: Support for multiple screens showing different movies
- **Seat Management**: Three categories - SILVER, GOLD, PLATINUM
- **Concurrent Booking**: Thread-safe seat locking to prevent double bookings
- **Session Timeout**: Automatic release of locked seats if payment not completed
- **Payment Integration**: Strategy pattern for multiple payment methods (Debit Card, UPI)
- **Real-time Seat Availability**: Dynamic seat availability checking

## 🏗️ Architecture & Design Patterns

### Design Patterns Used:
1. **Repository Pattern**: Services act as repositories managing collections of entities
2. **Strategy Pattern**: Payment processing with interchangeable strategies
3. **Dependency Injection**: Constructor-based injection throughout
4. **Factory Pattern**: Entity creation through service methods
5. **MVC Pattern**: Controller → Service separation

### Project Structure
```
BookMyShow/
├── include/
│   ├── enums/          # BookingStatus, SeatCategory
│   ├── models/         # Core domain models
│   ├── providers/      # Seat locking providers
│   ├── services/       # Business logic layer
│   ├── controllers/    # Request handling layer
│   └── payment/        # Payment strategies
├── src/                # Implementation files
├── main.cpp            # Demo application
├── CMakeLists.txt      # Build configuration
└── README.md
```

## 🚀 Building & Running

### Prerequisites
- C++17 compatible compiler (MSVC, GCC, Clang)
- CMake 3.10 or higher
- Visual Studio (optional, for Windows)

### Build Instructions

#### Using CMake (Command Line)
```bash
cd BookMyShow
mkdir build
cd build
cmake ..
cmake --build .
```

#### Using Visual Studio
1. Open Visual Studio
2. File → Open → CMake → Select `CMakeLists.txt`
3. Build → Build All
4. Run the executable

#### Quick Build Script (Windows PowerShell)
```powershell
cd BookMyShow
if (Test-Path build) { Remove-Item build -Recurse -Force }
mkdir build
cd build
cmake ..
cmake --build . --config Release
.\Release\BookMyShow.exe
```

### Running the Application
```bash
# From build directory
./bin/BookMyShow        # Linux/Mac
.\bin\BookMyShow.exe    # Windows
```

## 💡 Core Components

### Models
- **Movie**: Represents a movie with duration
- **Theatre**: Contains multiple screens
- **Screen**: Has seats and shows movies
- **Seat**: Individual seat with category
- **Show**: Movie screening at specific time
- **User**: System user
- **Booking**: Represents a seat reservation
- **SeatLock**: Temporary seat lock during booking

### Services
- **MovieService**: Movie CRUD operations
- **TheatreService**: Theatre, screen, and seat management
- **ShowService**: Show scheduling and retrieval
- **BookingService**: Booking creation and confirmation
- **SeatAvailabilityService**: Real-time seat availability
- **PaymentService**: Payment processing with retry logic

### Controllers
- **MovieController**: Movie creation endpoints
- **TheatreController**: Theatre/screen/seat setup
- **ShowController**: Show management and seat queries
- **BookingController**: Booking operations
- **PaymentController**: Payment processing

## 🔒 Concurrency Handling

The system handles concurrent bookings through:
- **Mutex-protected seat locks**: Thread-safe locking mechanism
- **Atomic counters**: For unique ID generation
- **Lock expiration**: Time-based lock release
- **Lock validation**: Ensures user owns the lock before confirmation

## 📊 Sample Output

```
=== BookMyShow System - Movie Ticket Booking ===

Creating a new theatre...
Theatre created with ID: 1

Creating a new screen...
Screen created with ID: 1

Creating seats...
Created seat at row 1 with ID: 1 and category: PLATINUM
...

Available seats: [1, 2, 3, 4, 5, ...]
Total available seats: 50

Booking created with ID: 1
Payment processed successfully!
Booking status: CONFIRMED

=== Simulating concurrent booking attempts ===
User1 booking (seats 5,6,7) succeeded with Booking ID: 2
User2 booking (seats 7,8,9) failed: Seat 7 is already locked.
```

## 🎤 Interview Discussion Points

### Key Topics (for 1-hour SDE2 discussion):

1. **Concurrency Management**
   - How seat locking prevents race conditions
   - Lock timeout mechanism
   - Thread safety with mutexes

2. **Scalability Concerns**
   - Distributed locking (Redis, Zookeeper)
   - Database transactions
   - Horizontal scaling strategies

3. **Design Patterns**
   - Why Strategy pattern for payments?
   - Repository pattern benefits
   - Dependency injection advantages

4. **Trade-offs**
   - In-memory vs persistent locks
   - Lock granularity (seat vs show)
   - Timeout duration selection

5. **Extensions**
   - Multiple theatres support
   - Pricing strategies
   - Discount coupons
   - Seat selection preferences

## 🔄 Possible Enhancements

1. **Persistent Storage**: Add database layer (PostgreSQL/MongoDB)
2. **Distributed Locks**: Implement Redis-based locking
3. **Event Sourcing**: Track all booking state changes
4. **Notification System**: Email/SMS alerts
5. **API Layer**: REST/GraphQL endpoints
6. **Caching**: Redis caching for frequently accessed data
7. **Analytics**: Booking metrics and reports
8. **Admin Panel**: Theatre/show management UI

## 📝 Notes for Interviews

- **Time Complexity**: Most operations O(1) with hash maps
- **Space Complexity**: O(n) where n = total seats across all shows
- **Lock Timeout**: Configurable (default 600s = 10 minutes)
- **Thread Safety**: Critical sections protected with mutexes
- **Extensibility**: Easy to add new payment methods via Strategy pattern

## 📚 References

- Original design inspiration: [Code with Aryan](https://codewitharyan.com/tech-blogs/design-movie-ticket-booking-system)
- Design Patterns: Gang of Four
- Concurrency: C++ Threading Library

## 📧 Contact

For questions or discussion about this implementation, feel free to reach out!

---

**Built with C++17 | CMake | Modern Design Patterns**

