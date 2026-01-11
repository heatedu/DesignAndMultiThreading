# Car Rental System - Low Level Design (C++)

## Overview
Complete low-level design of a Car Rental System suitable for a 1-hour SDE2 interview discussion.

## Features Implemented

### Core Functionality
- ✅ **Multi-Location Support**: Multiple rental stores across different cities
- ✅ **Vehicle Hierarchy**: Economy, Luxury, SUV with polymorphic pricing
- ✅ **Reservation Management**: Create, cancel, and track reservations
- ✅ **Conflict Prevention**: Automatic detection of overlapping bookings
- ✅ **Search & Filter**: Find vehicles by location, type, and date range
- ✅ **Dynamic Pricing**: Different pricing strategies for vehicle types

### Design Patterns Used
1. **Singleton**: `CarRentalSystem` - single instance managing entire system
2. **Template Method**: `Vehicle::calculatePrice()` - polymorphic pricing
3. **Strategy**: Ready to extend with pricing strategies
4. **Factory**: Can be added for vehicle creation

### SOLID Principles
- **S**ingle Responsibility: Each class has one clear purpose
- **O**pen/Closed: Extensible for new vehicle types without modifying existing code
- **L**iskov Substitution: All vehicle subtypes work interchangeably
- **I**nterface Segregation: Focused, minimal interfaces
- **D**ependency Inversion: System depends on abstractions

## Project Structure

```
CarRentalCursor/
├── VehicleTypes.h       - Enums for types and statuses
├── Vehicle.h            - Vehicle hierarchy (base + Economy/Luxury/SUV)
├── User.h               - User/Customer class
├── Reservation.h        - Reservation with conflict detection
├── Store.h              - Store/Location inventory management
├── CarRentalSystem.h    - Main system controller (Singleton)
└── main.cpp             - Comprehensive demo
```

## How to Build & Run

### Option 1: Visual Studio (Recommended)
1. Open `CarRentalCursor.sln` in Visual Studio
2. Build Solution (Ctrl+Shift+B)
3. Run (F5 or Ctrl+F5)

### Option 2: Command Line (Developer Command Prompt)
```bash
# Open "Developer Command Prompt for VS"
cd CarRentalCursor
cl /EHsc /std:c++17 main.cpp /Fe:CarRental.exe
CarRental.exe
```

### Option 3: MSBuild
```bash
msbuild CarRentalCursor.sln /p:Configuration=Release /p:Platform=x64
cd x64\Release
CarRentalCursor.exe
```

## Demo Output

The demo showcases:
1. Store setup (Mumbai, Delhi)
2. Adding vehicles (7 vehicles across different types)
3. User registration (2 users)
4. Vehicle search by location & type
5. Creating reservations with price calculation
6. Conflict prevention (overlapping booking rejection)
7. Cancellation and availability updates

Expected output shows:
- Available vehicles with pricing
- Successful reservation creation
- Conflict detection in action
- Dynamic availability updates

## Key Classes Explained

### `Vehicle` (Abstract Base)
- Polymorphic pricing via `calculatePrice(int days)`
- Three concrete types: `EconomyVehicle`, `LuxuryVehicle`, `SUVVehicle`
- Each has different pricing multipliers

### `Reservation`
- **Critical Logic**: `hasConflict(time_t start, time_t end)`
- Prevents double-booking
- Tracks status (ACTIVE, COMPLETED, CANCELLED)

### `CarRentalSystem` (Singleton)
- Manages all stores, users, reservations
- **Key Methods**:
  - `searchVehicles()` - Find available vehicles
  - `createReservation()` - Create booking with conflict check
  - `cancelReservation()` - Cancel and free up vehicle

### `Store`
- Manages inventory per location
- Filters by type and availability

## Interview Discussion Points

### Implemented
1. ✅ Vehicle polymorphism
2. ✅ Conflict detection algorithm
3. ✅ Multi-location support
4. ✅ Status management
5. ✅ Basic search/filter

### Extensions to Discuss
1. **Concurrency**: Add mutex for thread-safe reservations
2. **Pricing Strategies**: Strategy pattern for weekend/seasonal pricing
3. **Payment Integration**: Abstract payment processor
4. **Database Design**: Schema for persistence
5. **Scalability**: Caching, indexing, partitioning
6. **Additional Features**: 
   - Late return fees
   - Damage reporting
   - User ratings
   - Notifications
   - Loyalty programs

### Sample Follow-up Questions

**Q: How would you handle concurrent bookings?**
```cpp
std::mutex reservationMutex;
std::lock_guard<std::mutex> lock(reservationMutex);
// Critical section: conflict check + reservation creation
```

**Q: Database schema?**
```sql
vehicles (reg_number PK, type, rate, store_id, status)
stores (store_id PK, location)
reservations (res_id PK, vehicle_reg FK, start_time, end_time, status)
-- Index on (vehicle_reg, start_time, end_time) for fast conflict checks
```

**Q: How to optimize for 1000s of vehicles?**
- Indexing by (location, type, status)
- Interval tree for date conflict detection
- Redis caching for frequent searches
- Read replicas for search queries

## Code Quality Features

- ✅ Const correctness
- ✅ Smart pointers (`shared_ptr`) - no manual memory management
- ✅ Clear naming conventions
- ✅ Separation of concerns
- ✅ Header guards
- ✅ Modern C++17 features

## Time Complexity

- **Search vehicles**: O(V) where V = vehicles at location
- **Create reservation**: O(R) where R = existing reservations for conflict check
- **Cancel reservation**: O(1) lookup + O(S) for store search

**Optimization opportunity**: Index reservations by vehicle_id for O(k) conflict check where k = reservations per vehicle

## Requirements Coverage

| Requirement | Status | Implementation |
|-------------|--------|----------------|
| Multiple locations | ✅ | Store class per location |
| Vehicle types | ✅ | Economy/Luxury/SUV hierarchy |
| Availability tracking | ✅ | VehicleStatus enum |
| Search by criteria | ✅ | searchVehicles() with filters |
| Reservation management | ✅ | Create/cancel operations |
| Conflict prevention | ✅ | hasConflict() logic |
| Dynamic pricing | ✅ | Polymorphic calculatePrice() |
| User management | ✅ | User class with license |

## Author Notes

This design prioritizes:
1. **Interview suitability**: Can be coded in 30-40 mins
2. **Discussion depth**: Leaves room for extensions
3. **Clean code**: Demonstrates OOP mastery
4. **Practical design**: Real-world applicable patterns

Perfect for SDE2 interviews focusing on LLD skills.

