# Splitwise Expense Sharing System 💰

A complete C++ implementation of a Splitwise-like expense sharing system demonstrating **Factory** and **Observer** design patterns.

## 🎯 Design Patterns Implemented

### 1. Factory Pattern
- **SplitFactory**: Creates different types of splits (Equal, Percentage)
- Easily extensible to add new split types without modifying existing code
- Centralizes split type creation logic

### 2. Observer Pattern
- **ExpenseManager** (Subject): Manages expenses and notifies observers
- **BalanceSheet** (Observer): Automatically updates when expenses are added
- Loose coupling between components

## 📁 Project Structure

```
Splitwise/
├── include/              # Header files
│   ├── User.h
│   ├── UserPair.h
│   ├── Transaction.h
│   ├── Expense.h
│   ├── Split.h
│   ├── EqualSplit.h
│   ├── PercentageSplit.h
│   ├── SplitFactory.h
│   ├── ExpenseObserver.h
│   ├── ExpenseSubject.h
│   ├── ExpenseManager.h
│   └── BalanceSheet.h
├── src/                  # Implementation files
│   ├── main.cpp
│   ├── User.cpp
│   ├── UserPair.cpp
│   ├── Transaction.cpp
│   ├── Expense.cpp
│   ├── EqualSplit.cpp
│   ├── PercentageSplit.cpp
│   ├── SplitFactory.cpp
│   ├── ExpenseManager.cpp
│   └── BalanceSheet.cpp
├── .vscode/             # VS Code configuration
│   ├── tasks.json
│   └── launch.json
├── CMakeLists.txt       # CMake build configuration
├── compile.bat          # Direct compilation script
└── build.bat           # CMake build script
```

## 🚀 Building the Project

### Method 1: Direct Compilation (Recommended for Quick Start)

```bash
# Windows
.\compile.bat

# Or manually:
g++ -std=c++17 -I./include src/*.cpp -o Splitwise.exe
```

### Method 2: Using CMake

```bash
# Windows
.\build.bat

# Or manually:
mkdir build
cd build
cmake ..
cmake --build .
```

### Method 3: Using VS Code

1. Open the project in VS Code
2. Press `Ctrl+Shift+B` to build
3. Select "Build Splitwise (g++)"
4. Press `F5` to run with debugging

## 🏃 Running the Application

```bash
# After compilation
.\Splitwise.exe
```

## 📝 Key Features

### Core Classes
- **User**: Represents a user with ID, name, and email
- **Expense**: Tracks expense details, payer, participants, and shares
- **Transaction**: Represents a payment between two users
- **UserPair**: Manages balance tracking between user pairs

### Split Types (Factory Pattern)
- **EqualSplit**: Divides expense equally among all participants
- **PercentageSplit**: Divides based on specified percentages
- Extensible: Add new split types by implementing the `Split` interface

### Balance Management
- Tracks who owes whom and how much
- Calculates individual and pairwise balances
- Provides multiple settlement algorithms

### Settlement Algorithms

1. **Simplified Settlements**: O(n) greedy approach
   - Practical solution for debt settlement
   - Minimizes transactions in most cases

2. **Suboptimal Backtracking**: O((n-1)!) approach
   - Explores all possibilities
   - Guaranteed minimum transactions
   - Suitable for small groups

3. **Optimal Dynamic Programming**: O(n * 2^n) approach
   - Uses bitmask DP
   - Optimal solution for groups
   - More efficient than backtracking

## 🎓 Interview Discussion Points

### Design Decisions
1. **Why Factory Pattern?**
   - New split types can be added without modifying existing code
   - Follows Open/Closed Principle
   - Centralizes object creation logic

2. **Why Observer Pattern?**
   - BalanceSheet updates automatically when expenses change
   - Multiple observers can track expenses independently
   - Loose coupling between ExpenseManager and BalanceSheet

3. **Why these algorithms?**
   - Simple greedy: Fast and practical
   - Backtracking: Educational, shows the problem complexity
   - DP: Optimal solution with better time complexity

### Extensibility
- Add new split types: Implement `Split` interface
- Add new observers: Implement `ExpenseObserver` interface
- Add groups, categories, recurring expenses, etc.

### Time Complexity Analysis
- Adding expense: O(n) where n = number of participants
- Getting balance: O(1) to O(n) depending on balance type
- Simplified settlements: O(n log n)
- Optimal settlements (DP): O(n * 2^n)

## 📊 Example Output

```
=== Splitwise Expense Sharing System ===

--- Expense 1: Dinner (Equal Split) ---
Alice pays $60 for dinner, split equally among 3 people
Each person owes: $20.00

--- Expense 2: Movie Tickets (Percentage Split) ---
Bob pays $45 for movie tickets
Split: Alice=40%, Bob=30%, Charlie=30%
Alice owes: $18.00
Bob owes: $13.50
Charlie owes: $13.50

=== INDIVIDUAL BALANCES ===
Alice's total balance: $22.00
Bob's total balance: $-6.50
Charlie's total balance: $-33.50

=== SIMPLIFIED SETTLEMENTS ===
Number of transactions: 2
Bob pays Alice $6.50
Charlie pays Alice $33.50

=== OPTIMAL MINIMUM SETTLEMENTS (DP) ===
Minimum transactions needed (using DP): 2
```

## 🛠️ Requirements

- C++17 or higher
- g++ (MinGW on Windows) or any C++17 compliant compiler
- CMake 3.10+ (optional, for CMake build)
- Visual Studio Code (optional, for IDE integration)

## 💡 Discussion Topics for Interview

1. **Design Patterns**
   - How Factory pattern enables extensibility
   - How Observer pattern decouples components
   - When to use each pattern

2. **Algorithms**
   - Trade-offs between different settlement algorithms
   - Why DP is better than backtracking
   - Real-world considerations (transaction limits, etc.)

3. **Data Structures**
   - Why use map for balances
   - Custom hash functions for User and UserPair
   - Memory vs. time trade-offs

4. **Scalability**
   - How to handle thousands of users
   - Database integration considerations
   - Concurrent expense additions

5. **Additional Features**
   - Groups and categories
   - Recurring expenses
   - Multiple currencies
   - Notifications and reminders

## 📚 Code Highlights

### Factory Pattern Usage
```cpp
auto equalSplit = SplitFactory::createSplit("EQUAL");
auto shares = equalSplit->calculateSplit(60.0, participants, splitDetails);
```

### Observer Pattern Usage
```cpp
ExpenseManager expenseManager;
BalanceSheet balanceSheet;
expenseManager.addObserver(&balanceSheet);
expenseManager.addExpense(expense); // Automatically notifies balanceSheet
```

## ✨ Future Enhancements

- Add ExactSplit for custom amounts per person
- Implement Group management
- Add expense categories and tags
- Support for multiple currencies
- Add expense history and analytics
- Implement undo/redo functionality
- Add persistence (database integration)

## 📄 License

This is an educational project for interview preparation.

---

**Ready for your SDE2 interview discussion! 🚀**

The system is designed to be discussed within 1 hour, covering:
- Core design (15 min)
- Factory pattern (10 min)
- Observer pattern (10 min)
- Algorithms (15 min)
- Extensibility & trade-offs (10 min)
