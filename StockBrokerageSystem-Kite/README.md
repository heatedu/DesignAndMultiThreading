# Online Stock Brokerage System (LLD)

A Low-Level Design implementation of an Online Stock Brokerage System in C++17, built for SDE-2 level interviews.

---

## Table of Contents

- [Requirements](#requirements)
- [Core Entities](#core-entities)
- [Design Patterns Used](#design-patterns-used)
- [Class Diagram](#class-diagram)
- [File Structure](#file-structure)
- [API Reference](#api-reference)
- [Flow of Execution](#flow-of-execution)
- [How to Build & Run](#how-to-build--run)
- [Sample Output](#sample-output)
- [Key Design Decisions](#key-design-decisions)

---

## Requirements

| # | Requirement | How It's Handled |
|---|-------------|-----------------|
| R1 | Users create & manage trading accounts | `User` + `Account` classes, `StockBroker::createAccount()` |
| R2 | Buy/Sell stocks, view portfolio & history | `BuyOrder`, `SellOrder`, `Portfolio`, `Account::getTransactionHistory()` |
| R3 | Real-time stock quotes & market data | **Observer pattern** — `Stock` notifies registered `Account`s on price change |
| R4 | Order placement → execution → settlement | `StockBroker::placeOrder()` → `OrderFactory::createOrder()` → `Order::execute()` |
| R5 | Business rules (balance check, stock availability) | `InsufficientFundsException`, `InsufficientStockException` thrown during execution |
| R6 | Concurrent requests, data consistency | `std::mutex` + `std::lock_guard` on all shared state; `std::atomic` for ID generation |
| R7 | Scalable, single entry point | **Singleton pattern** — one `StockBroker` instance |
| R8 | Secure, encapsulated data | Private members, controlled access via public methods |

---

## Core Entities

### User
- Represents a person using the brokerage.
- Fields: `userId`, `name`, `email`
- Pure data holder, no business logic.

### Account
- A user's trading account.
- Fields: `accountId`, `user` (User ref), `balance`, `portfolio`, `transactionHistory`
- **Implements `IStockObserver`** — gets notified when a stock's price changes.
- Methods: `deposit()`, `withdraw()` (throws `InsufficientFundsException`)

### Stock
- Represents a tradable stock on the market.
- Fields: `symbol`, `name`, `price`, `observers` (list of `IStockObserver*`)
- **Acts as Subject in Observer pattern.**
- `updatePrice(newPrice)` → triggers `notifyObservers()` → calls `onPriceUpdate()` on every registered observer.

### Portfolio
- Holds a map of `stockSymbol → quantity` for an account.
- `addStock(symbol, qty)` — increases holdings.
- `removeStock(symbol, qty)` — decreases holdings; throws `InsufficientStockException` if not enough.

### Order (Abstract Base Class)
- Fields: `orderId` (auto-generated), `account`, `stock`, `quantity`, `price`, `status`
- Pure virtual: `execute()`, `getTypeName()`
- Auto-generates unique IDs using `std::atomic<int>`.

### BuyOrder (extends Order)
- `execute()` flow:
  1. Calculate `totalCost = qty * price`
  2. `account.withdraw(totalCost)` — debit funds
  3. `account.portfolio.addStock(symbol, qty)` — add shares
  4. Set status to `EXECUTED`
  5. If `InsufficientFundsException` → status = `REJECTED`

### SellOrder (extends Order)
- `execute()` flow:
  1. `account.portfolio.removeStock(symbol, qty)` — remove shares
  2. Calculate `totalCredit = qty * price`
  3. `account.deposit(totalCredit)` — credit funds
  4. Set status to `EXECUTED`
  5. If `InsufficientStockException` → status = `REJECTED`

### OrderFactory
- **Factory pattern** — static method `createOrder(OrderType, ...)` returns `unique_ptr<Order>`.
- Switch on `OrderType::BUY` or `OrderType::SELL` to create the correct concrete class.

### StockBroker (Singleton)
- **Central orchestrator** of the entire system.
- Manages: `accounts` map, `stocks` map, `orders` list.
- Private constructor, deleted copy/assign.
- `getInstance()` uses Meyers' Singleton (thread-safe since C++11).
- `placeOrder()` uses `OrderFactory` to create and execute orders.

---

## Design Patterns Used

### 1. Singleton — `StockBroker`

```
┌─────────────────────────────────┐
│     StockBroker «Singleton»     │
├─────────────────────────────────┤
│ - StockBroker()        [private]│
│ - operator=(const&)    [deleted]│
│ - StockBroker(const&)  [deleted]│
├─────────────────────────────────┤
│ + getInstance(): StockBroker&   │ ← static, returns the ONE instance
│ + createAccount(...)            │
│ + addStock(...)                 │
│ + placeOrder(...)               │
└─────────────────────────────────┘
```

**Why?** The brokerage is a single centralized system. Only one instance should manage all accounts, stocks, and orders. Prevents duplicate state.

**How?** Meyers' Singleton — `static StockBroker instance;` inside `getInstance()`. Thread-safe guaranteed by C++11 standard (§6.7).

### 2. Factory — `OrderFactory`

```
         OrderFactory::createOrder(type, ...)
                    │
          ┌─────────┴─────────┐
          ▼                   ▼
     BuyOrder            SellOrder
```

**Why?** Decouples order creation from the broker. Adding new order types (e.g., `StopLossOrder`, `LimitOrder`) only requires adding a new class + one case in the factory — no changes to `StockBroker`.

**How?** Static method switches on `OrderType` enum, returns `std::unique_ptr<Order>` (polymorphic).

### 3. Observer — `Stock` (Subject) → `IStockObserver` → `Account` (Observer)

```
  Stock (Subject)                    IStockObserver (Interface)
  ┌───────────────┐                  ┌─────────────────────┐
  │ observers_[]  │──notifies───────►│ onPriceUpdate(      │
  │ updatePrice() │                  │   symbol, newPrice)  │
  │ registerObs() │                  └──────────┬──────────┘
  │ removeObs()   │                             │ implements
  └───────────────┘                  ┌──────────┴──────────┐
                                     │      Account        │
                                     │ onPriceUpdate() {   │
                                     │   // log holdings   │
                                     │   // value update   │
                                     │ }                   │
                                     └─────────────────────┘
```

**Why?** When a stock price changes, all accounts holding that stock should be notified in real-time. Loose coupling — `Stock` doesn't know or care what `Account` does with the notification.

**How?** `Stock` maintains a `vector<IStockObserver*>`. On `updatePrice()`, it iterates and calls `onPriceUpdate()` on each observer.

---

## Class Diagram

```
┌──────────────┐      ┌────────────────────────────────────┐
│    User      │1───1 │           Account                  │
│ userId       │      │ accountId, balance                  │
│ name         │      │ portfolio: Portfolio                │
│ email        │      │ transactionHistory: vector<Order*>  │
└──────────────┘      │ deposit(), withdraw()               │
                      │ onPriceUpdate() ◄── IStockObserver  │
                      └──────────┬─────────────────────────┘
                                 │ has-a
                      ┌──────────┴──────────┐
                      │     Portfolio        │
                      │ holdings: map<str,int>│
                      │ addStock()           │
                      │ removeStock()        │
                      └─────────────────────┘

┌─────────────────────────────────────────────────┐
│          StockBroker «Singleton»                │
│ accounts: map<string, Account>                   │
│ stocks: map<string, Stock>                       │
│ orders: vector<Order>                            │
│ getInstance(), createAccount(), addStock()       │
│ placeOrder() ──uses──► OrderFactory              │
└──────────────────────────────────────────────────┘

┌───────────────┐  registerObserver()   ┌──────────────────┐
│    Stock      │───────────────────────│  IStockObserver   │
│ symbol, name  │  notifyObservers()    │  onPriceUpdate()  │
│ price         │──────────────────────►│                   │
│ observers[]   │                       └──────────────────┘
└───────────────┘                              ▲ implements
                                               │
                                          Account

                ┌───────────────┐
                │ OrderFactory  │
                │ createOrder() │
                └───────┬───────┘
                        │ creates
                ┌───────┴───────┐
                │  Order (ABC)  │
                │ orderId       │
                │ account, stock│
                │ qty, price    │
                │ status        │
                │ execute() = 0 │
                └───────┬───────┘
             ┌──────────┼──────────┐
        ┌────┴────┐          ┌─────┴─────┐
        │BuyOrder │          │ SellOrder  │
        │execute()│          │ execute()  │
        └─────────┘          └───────────┘

Enums:                    Exceptions:
┌─────────────┐           ┌──────────────────────────┐
│ OrderStatus │           │ InsufficientFundsException│
│ PENDING     │           │ InsufficientStockException│
│ EXECUTED    │           └──────────────────────────┘
│ REJECTED    │
├─────────────┤
│ OrderType   │
│ BUY         │
│ SELL        │
└─────────────┘
```

---

## File Structure

```
StockBrokerageSystem-Kite/
│
├── StockBrokerageSystem.sln        ← Visual Studio solution file
├── StockBrokerageSystem.vcxproj    ← Visual Studio project file
├── README.md                       ← This file
│
├── main.cpp                        ← Entry point / demo runner
│
├── OrderStatus.h                   ← Enums: OrderStatus, OrderType
├── Exceptions.h                    ← Custom exceptions
├── IStockObserver.h                ← Observer interface
│
├── User.h / User.cpp              ← User entity
├── Portfolio.h / Portfolio.cpp     ← Holdings management
├── Account.h / Account.cpp        ← Trading account + observer impl
├── Stock.h / Stock.cpp            ← Tradable stock + subject (observer pattern)
│
├── Order.h / Order.cpp            ← Abstract base order
├── BuyOrder.h / BuyOrder.cpp      ← Concrete buy order
├── SellOrder.h / SellOrder.cpp    ← Concrete sell order
├── OrderFactory.h                 ← Factory for creating orders
│
└── StockBroker.h / StockBroker.cpp ← Singleton broker (central orchestrator)
```

---

## API Reference

### StockBroker (Singleton)

| Method | Description |
|--------|-------------|
| `static StockBroker& getInstance()` | Returns the single broker instance |
| `Account* createAccount(accountId, user, balance)` | Creates a new trading account |
| `Account* getAccount(accountId)` | Looks up account by ID |
| `void addStock(symbol, name, price)` | Adds a stock to the market |
| `Stock* getStock(symbol)` | Looks up stock by symbol |
| `void placeOrder(OrderType, account, stock, qty, price)` | Creates & executes an order via factory |
| `void printPortfolio(account)` | Prints account balance & holdings |
| `void printTransactionHistory(account)` | Prints all orders for account |

### Account

| Method | Description |
|--------|-------------|
| `void deposit(amount)` | Adds funds to balance |
| `void withdraw(amount)` | Removes funds; throws `InsufficientFundsException` |
| `Portfolio& getPortfolio()` | Returns reference to the account's portfolio |
| `void onPriceUpdate(symbol, price)` | Observer callback — logs holding value |

### Stock

| Method | Description |
|--------|-------------|
| `void updatePrice(newPrice)` | Updates price and notifies all observers |
| `void registerObserver(IStockObserver*)` | Subscribes an observer |
| `void removeObserver(IStockObserver*)` | Unsubscribes an observer |

### OrderFactory

| Method | Description |
|--------|-------------|
| `static unique_ptr<Order> createOrder(type, account, stock, qty, price)` | Returns `BuyOrder` or `SellOrder` |

---

## Flow of Execution

```
main()
  │
  ├─► StockBroker::getInstance()          // Get singleton
  │
  ├─► broker.addStock("AAPL", ...)        // Populate market
  ├─► broker.addStock("GOOGL", ...)
  │
  ├─► broker.createAccount("ACC-1", ...)  // Create accounts
  │
  ├─► aapl->registerObserver(acc1)        // Subscribe to price updates
  │
  ├─► broker.placeOrder(BUY, acc1, aapl, 10, 150.0)
  │     │
  │     ├─► OrderFactory::createOrder(BUY, ...) → BuyOrder
  │     │
  │     └─► buyOrder->execute()
  │           ├─► account.withdraw(1500)       ← debit funds
  │           ├─► portfolio.addStock("AAPL",10) ← add shares
  │           ├─► status = EXECUTED
  │           └─► account.addToHistory(this)
  │
  ├─► aapl->updatePrice(160.0)            // Price change
  │     └─► notifyObservers()
  │           └─► acc1->onPriceUpdate("AAPL", 160.0)
  │                 └─► "Account ACC-1: AAPL now $160, holding 10, value $1600"
  │
  ├─► broker.placeOrder(SELL, acc1, aapl, 5, 160.0)
  │     └─► sellOrder->execute()
  │           ├─► portfolio.removeStock("AAPL", 5)
  │           ├─► account.deposit(800)
  │           └─► status = EXECUTED
  │
  ├─► broker.placeOrder(SELL, acc2, aapl, 100, ...)  ← REJECTED (no holdings)
  │     └─► InsufficientStockException caught → status = REJECTED
  │
  ├─► broker.placeOrder(BUY, acc2, googl, 1000, ...) ← REJECTED (no funds)
  │     └─► InsufficientFundsException caught → status = REJECTED
  │
  ├─► broker.printPortfolio(acc1)
  └─► broker.printTransactionHistory(acc1)
```

---

## How to Build & Run

### Visual Studio (Recommended)

1. Double-click `StockBrokerageSystem.sln`
2. Select **Debug | x64** (or any config)
3. **Build** → Build Solution (`Ctrl+Shift+B`)
4. **Run** → Start Without Debugging (`Ctrl+F5`)

### Command Line (g++ / MinGW)

```bash
g++ -std=c++17 -o StockBrokerage.exe User.cpp Stock.cpp Portfolio.cpp Account.cpp Order.cpp BuyOrder.cpp SellOrder.cpp StockBroker.cpp main.cpp
./StockBrokerage.exe
```

### Command Line (MSVC Developer Prompt)

```bash
cl /std:c++17 /EHsc /Fe:StockBrokerage.exe User.cpp Stock.cpp Portfolio.cpp Account.cpp Order.cpp BuyOrder.cpp SellOrder.cpp StockBroker.cpp main.cpp
StockBrokerage.exe
```

---

## Sample Output

```
[StockBroker] Stock added: AAPL (Apple Inc.) @ $150
[StockBroker] Stock added: GOOGL (Alphabet Inc.) @ $2800
[StockBroker] Stock added: AMZN (Amazon.com Inc.) @ $3400
[StockBroker] Account created: ACC-1 for user Alice
[StockBroker] Account created: ACC-2 for user Bob

--- Placing BUY orders ---
[StockBroker] Processing order ORD-1 (BUY)
[Account ACC-1] Withdrew 1500, new balance: 98500
[BuyOrder] ORD-1 EXECUTED: Bought 10 shares of AAPL @ $150
[StockBroker] Processing order ORD-2 (BUY)
[Account ACC-1] Withdrew 14000, new balance: 84500
[BuyOrder] ORD-2 EXECUTED: Bought 5 shares of GOOGL @ $2800
[StockBroker] Processing order ORD-3 (BUY)
[Account ACC-2] Withdrew 10200, new balance: 39800
[BuyOrder] ORD-3 EXECUTED: Bought 3 shares of AMZN @ $3400

=== Portfolio for Account ACC-1 ===
Balance: $84500
  GOOGL: 5 shares
  AAPL: 10 shares
=======================================

=== Portfolio for Account ACC-2 ===
Balance: $39800
  AMZN: 3 shares
=======================================

--- Stock price update ---
[Stock] AAPL price updated to 160
[Observer] Account ACC-1 notified: AAPL now at $160 | holding 10 shares | value $1600
[Stock] GOOGL price updated to 2900
[Observer] Account ACC-1 notified: GOOGL now at $2900 | holding 5 shares | value $14500

--- Placing SELL order ---
[StockBroker] Processing order ORD-4 (SELL)
[Account ACC-1] Deposited 800, new balance: 85300
[SellOrder] ORD-4 EXECUTED: Sold 5 shares of AAPL @ $160

--- Attempting invalid SELL ---
[StockBroker] Processing order ORD-5 (SELL)
[SellOrder] ORD-5 REJECTED: Insufficient stock: AAPL (have 0, need 100)

--- Attempting buy with insufficient funds ---
[StockBroker] Processing order ORD-6 (BUY)
[BuyOrder] ORD-6 REJECTED: Insufficient funds in account ACC-2 (have 39800.00, need 2900000.00)

=== Portfolio for Account ACC-1 ===
Balance: $85300
  GOOGL: 5 shares
  AAPL: 5 shares
=======================================

=== Portfolio for Account ACC-2 ===
Balance: $39800
  AMZN: 3 shares
=======================================

=== Transaction History for Account ACC-1 ===
  ORD-1 | BUY  | AAPL  | qty=10 | price=$150  | EXECUTED
  ORD-2 | BUY  | GOOGL | qty=5  | price=$2800 | EXECUTED
  ORD-4 | SELL | AAPL  | qty=5  | price=$160  | EXECUTED
=================================================

=== Transaction History for Account ACC-2 ===
  ORD-3 | BUY  | AMZN  | qty=3  | price=$3400 | EXECUTED
  ORD-5 | SELL | AAPL  | qty=100| price=$160  | REJECTED
  ORD-6 | BUY  | GOOGL | qty=1000| price=$2900| REJECTED
=================================================
```

---

## Key Design Decisions

| Decision | Rationale |
|----------|-----------|
| **Meyers' Singleton** over eager init | Thread-safe by C++11 spec, lazy init, no need for `call_once` or double-checked locking |
| **`unique_ptr<Order>`** ownership | `StockBroker` owns all orders. Raw ptrs in `transactionHistory` are non-owning views — no double-free |
| **Observer uses raw ptrs** | Observers (Accounts) always outlive the Stock in our design. In production, you'd use `weak_ptr` or deregister in destructor |
| **Exceptions for business rules** | `InsufficientFundsException` / `InsufficientStockException` — caught at order level, order marked `REJECTED`, system continues |
| **Mutex per object** | Fine-grained locking on `Account`, `Portfolio`, `Stock`, `StockBroker` — avoids global lock bottleneck |
| **Factory is header-only** | Simple enough — just a static switch. No `.cpp` needed |
| **No STL allocator / pool** | Kept simple for interview scope. Mention it as a scalability improvement if asked |

---

## Extensibility Points (Talk About in Interview)

- **New order types** (LimitOrder, StopLossOrder) → add class + one case in `OrderFactory`
- **Order matching engine** → replace direct execution with a matching queue
- **Strategy pattern** for pricing / commission → plug different fee structures
- **State pattern** for order lifecycle (Pending → PartialFill → Filled → Settled)
- **Chain of Responsibility** for validation (balance check → stock availability → risk check → compliance)
- **Database persistence** → swap in-memory maps for a repository layer
- **Message queue** for async order processing at scale
