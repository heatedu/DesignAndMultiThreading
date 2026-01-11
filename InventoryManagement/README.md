# Inventory Management System - Low Level Design (C++)

Complete implementation of an Inventory Management System using 5 Design Patterns, suitable for SDE-2 level interviews.

## Design Patterns Implemented

### 1. **Singleton Pattern** - InventoryManager
- **Purpose**: Single point of control for inventory operations
- **Implementation**: Thread-safe lazy initialization with mutex
- **Location**: `InventoryManager.h`
```cpp
static InventoryManager* getInstance() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (instance == nullptr) {
        instance = new InventoryManager();
    }
    return instance;
}
```

### 2. **Factory Pattern** - ProductFactory
- **Purpose**: Encapsulates product creation logic
- **Implementation**: Creates different product types based on category
- **Location**: `ProductFactory.h`
```cpp
Product* createProduct(ProductCategory category, 
                       const std::string& sku, ...);
```

### 3. **Builder Pattern** - Product Construction
- **Purpose**: Constructs complex product objects with many optional parameters
- **Implementation**: Fluent interface with method chaining
- **Location**: `Product.h`
```cpp
ElectronicsProduct* laptop = 
    ElectronicsProduct::ElectronicsBuilder("SKU", "Laptop", 999.99, "Dell")
        .warrantyPeriodValue(24)
        .modelNumberValue("XPS-15")
        .wirelessConnectivityValue(true)
        .quantityValue(50)
        .build();
```

### 4. **Observer Pattern** - Inventory Alerts
- **Purpose**: Notifies stakeholders when stock changes
- **Implementation**: Subject-Observer relationship
- **Location**: `Observer.h`, `InventoryManager.h`
- **Observers**:
  - `SupplierNotifier` - Notifies suppliers for low stock
  - `DashboardAlertSystem` - Shows CRITICAL/WARNING alerts based on stock %

```cpp
void notifyObservers(Product* product) {
    for (auto* observer : observers) {
        observer->update(product);
    }
}
```

### 5. **Strategy Pattern** - Replenishment Strategies
- **Purpose**: Enables different replenishment algorithms
- **Implementation**: Interchangeable strategy objects
- **Location**: `ReplenishmentStrategy.h`
- **Strategies**:
  - `JustInTimeStrategy` - Minimal orders
  - `BulkOrderStrategy` - Large quantity orders

```cpp
manager->setReplenishmentStrategy(&jitStrategy);
manager->checkAndReplenish("SKU123");
```

---

## Project Structure

```
InventoryManagement/
├── InventoryManagement.sln          ← Visual Studio Solution
├── InventoryManagement/
│   ├── Enums.h                      ← ProductCategory enum
│   ├── Product.h                    ← Product + Builder Pattern
│   ├── ProductFactory.h             ← Factory Pattern
│   ├── Observer.h                   ← Observer Pattern (SupplierNotifier, DashboardAlertSystem)
│   ├── ReplenishmentStrategy.h      ← Strategy Pattern (JIT, Bulk)
│   ├── Warehouse.h                  ← Warehouse entity
│   ├── InventoryManager.h           ← Singleton Pattern
│   ├── main.cpp                     ← Demo application
│   └── InventoryManagement.vcxproj
└── README.md                        ← This file
```

---

## Class Diagram

```
┌─────────────────────────────────────────────────────────┐
│              InventoryManager (Singleton)                │
│  ┌────────────┐  ┌──────────┐  ┌──────────────────┐   │
│  │ Warehouses │  │ Factory  │  │ Strategy         │   │
│  │ List       │  │          │  │ Replenishment    │   │
│  └────────────┘  └──────────┘  └──────────────────┘   │
│  ┌────────────────────────────────────────────────┐    │
│  │           Observers List                       │    │
│  └────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────┘
                    │
         ┌──────────┴──────────┐
         ▼                     ▼
    ┌─────────┐          ┌──────────┐
    │Warehouse│          │ Observer │
    └─────────┘          └──────────┘
         │                     │
         ▼                     ├─→ SupplierNotifier
  ┌──────────────┐            └─→ DashboardAlertSystem
  │   Product    │
  │  (Builder)   │
  └──────────────┘
         │
    ┌────┴────┬──────────┬─────────┐
    ▼         ▼          ▼         ▼
Electronics Clothing  Grocery  Furniture
```

---

## Key Features

| Feature | Implementation |
|---------|---------------|
| **Multiple Warehouses** | `InventoryManager::addWarehouse()` |
| **Complex Product Creation** | Builder Pattern with fluent interface |
| **Stock Tracking** | `Product::quantity`, `Product::threshold` |
| **Low Stock Alerts** | Observer Pattern with multiple notification types |
| **Flexible Replenishment** | Strategy Pattern for different algorithms |
| **Thread-Safe Singleton** | Mutex-protected getInstance() |
| **Type-Safe Enums** | `enum class ProductCategory` |

---

## Usage Examples

### 1. Create Products using Builder Pattern

```cpp
// Electronics with all optional parameters
ElectronicsProduct* laptop = 
    ElectronicsProduct::ElectronicsBuilder("ELEC-001", "Gaming Laptop", 1299.99, "Dell")
        .warrantyPeriodValue(24)
        .modelNumberValue("XPS-15-2024")
        .wirelessConnectivityValue(true)
        .powerConsumptionValue(150)
        .quantityValue(50)
        .thresholdValue(10)
        .build();

// Clothing with some optional parameters
ClothingProduct* jacket = 
    ClothingProduct::ClothingBuilder("CLTH-001", "Winter Jacket", 129.99, "L", "Black")
        .materialValue("Gore-Tex")
        .waterResistantValue(true)
        .quantityValue(30)
        .build();

// Grocery with defaults
GroceryProduct* milk = 
    GroceryProduct::GroceryBuilder("GROC-001", "Milk", 5.99, expiryDate)
        .refrigeratedValue(true)
        .organicValue(true)
        .build();
```

### 2. Setup Observers

```cpp
SupplierNotifier supplier("ABC Suppliers", "orders@abc.com");
DashboardAlertSystem dashboard("HIGH", {"admin@company.com"});

manager->addObserver(&supplier);
manager->addObserver(&dashboard);
```

### 3. Switch Replenishment Strategies

```cpp
JustInTimeStrategy jitStrategy;
manager->setReplenishmentStrategy(&jitStrategy);
manager->checkAndReplenish("SKU123");

// Switch to bulk ordering
BulkOrderStrategy bulkStrategy;
manager->setReplenishmentStrategy(&bulkStrategy);
```

---

## Expected Output

```
========================================
   BUILDER PATTERN DEMONSTRATION
========================================

--- Building ElectronicsProduct (Laptop) ---
Created: Gaming Laptop
  SKU: ELEC-001
  Brand: Dell
  Model: XPS-15-2024
  Warranty: 24 months
  Wireless: Yes
  Power: 150W
  Quantity: 50

--- Building ClothingProduct (Jacket) ---
Created: Winter Jacket
  SKU: CLTH-001
  Size: L
  Color: Black
  Material: Gore-Tex
  Brand: North Face
  Water Resistant: Yes

========================================
   COMPLETE SYSTEM DEMONSTRATION
========================================

--- Performing Inventory Check ---
Notification sent to TechSupply Co. for low stock of Premium Laptop
CRITICAL ALERT: Premium Laptop stock critically low at 8 units (53.3% of threshold)
Dashboard notification sent to admin: admin@company.com - CRITICAL level alert for Premium Laptop
Applying Just-In-Time replenishment for Premium Laptop
```

---

## Building & Running

### Visual Studio 2022
1. Open `InventoryManagement.sln`
2. Set configuration to **Debug | x64**
3. Build: `Ctrl+Shift+B`
4. Run: `F5` or `Ctrl+F5`

### Command Line (with MSVC)
```bash
cl /EHsc /std:c++17 main.cpp /Fe:inventory.exe
./inventory.exe
```

---

## Interview Discussion Points (1 Hour)

| Time | Topic |
|------|-------|
| 0-10 min | Problem statement, requirements clarification |
| 10-25 min | **Singleton** + **Factory** patterns |
| 25-40 min | **Builder** pattern (complex object construction) |
| 40-50 min | **Observer** + **Strategy** patterns |
| 50-60 min | Edge cases, scalability, trade-offs |

### Key Discussion Topics:
- **Singleton**: Thread-safety, lazy vs eager initialization, testability
- **Factory**: When to use vs Builder, extensibility
- **Builder**: Handling 10+ optional parameters, immutability
- **Observer**: Push vs Pull, memory leaks with raw pointers, weak_ptr
- **Strategy**: Runtime vs compile-time polymorphism, performance

---

## Design Decisions

| Decision | Rationale |
|----------|-----------|
| **Raw pointers** | Interview-friendly, simpler than smart pointers |
| **Template Builder** | Type-safe fluent interface with method chaining |
| **const members** | Immutability for core product attributes |
| **Map<string, Product*>** | Fast SKU-based lookups |
| **Thread-safe Singleton** | Production-ready implementation |

---

## Extending the System

### Add New Product Type
1. Inherit from `Product`
2. Create nested `Builder` class
3. Update `ProductFactory` if needed

### Add New Observer
1. Inherit from `InventoryObserver`
2. Implement `update(Product*)`
3. Register with `InventoryManager`

### Add New Replenishment Strategy
1. Inherit from `ReplenishmentStrategy`
2. Implement `replenish(Product*)`
3. Inject via `setReplenishmentStrategy()`

---

## References
- [Design Patterns - Gang of Four](https://en.wikipedia.org/wiki/Design_Patterns)
- [CodeWithAryan - Inventory Management System](https://codewitharyan.com/tech-blogs/design-inventory-management-system)

---

## Author
Implementation for SDE-2 Level Interview Preparation

**Design Patterns**: Singleton, Factory, Builder, Observer, Strategy  
**Language**: C++17  
**Platform**: Visual Studio 2022

