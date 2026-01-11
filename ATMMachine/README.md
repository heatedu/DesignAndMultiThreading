# ATM Machine System - Low Level Design

## Overview

Complete working implementation of ATM Machine using State Pattern and Factory Pattern in C++.

## Design Patterns Used

### 1. State Pattern

- **Purpose**: Manages ATM state transitions
- **States**:
  - `IdleState`: Waiting for card
  - `HasCardState`: Card inserted, waiting for PIN
  - `SelectOperationState`: Authenticated, choose operation
  - `CashWithdrawalState`: Processing withdrawal
  - `CheckBalanceState`: Displaying balance

### 2. Factory Pattern (Implicit)

- State creation is encapsulated in state transitions
- Each state creates the next appropriate state

## Architecture

```
ATMMachine (Context)
    ├── ATMState (Abstract)
    │   ├── IdleState
    │   ├── HasCardState
    │   ├── SelectOperationState
    │   ├── CashWithdrawalState
    │   └── CheckBalanceState
    ├── CashDispenser
    ├── TransactionLog
    └── Card → Account
```

## Key Components

1. **ATMMachine**: Main context class managing states
2. **ATMState**: Abstract base for all states
3. **Card**: User card with PIN validation
4. **Account**: Bank account with balance
5. **CashDispenser**: Cash inventory management
6. **TransactionLog**: Audit trail

## Features

✓ PIN-based authentication (3 attempts max)
✓ Cash withdrawal with denomination management
✓ Balance inquiry
✓ Transaction logging
✓ Maintenance mode
✓ State-based operation validation
✓ Insufficient funds handling
✓ ATM cash inventory tracking

## Compilation & Running

### Using Visual Studio:

1. Open `ATMMachine.sln` in Visual Studio
2. Press F5 or Ctrl+F5 to build and run
3. Output will be in `bin\Debug\` or `bin\Release\`

### Using Makefile (Windows):

```bash
make
atm.exe
```

### Manual compilation:

```bash
g++ -std=c++14 -o atm main.cpp Account.cpp Card.cpp CashDispenser.cpp TransactionLog.cpp ATMMachine.cpp IdleState.cpp HasCardState.cpp SelectOperationState.cpp CashWithdrawalState.cpp CheckBalanceState.cpp
```

## Test Flow

1. Insert Card (option 1)
2. Enter PIN (option 2) - Try: 1234 or 5678
3. Select Operation (option 3) - Choose 1 (Withdraw) or 2 (Balance)
4. If withdrawal: Enter amount (option 4)
5. Eject Card (option 6)

## Discussion Points for Interview (1 hour)

### 1. State Pattern (15 min)

- Why State over simple if-else?
- State transition diagram
- Adding new states (deposit, transfer)

### 2. Design Principles (15 min)

- Single Responsibility
- Open/Closed Principle
- How to extend functionality

### 3. Edge Cases (15 min)

- Wrong PIN multiple times
- Insufficient funds
- ATM out of cash
- Maintenance mode

### 4. Improvements (15 min)

- Threading/concurrency
- Network failures
- Card retention
- Session timeout
- Database integration
- Strategy pattern for withdrawal algorithms
