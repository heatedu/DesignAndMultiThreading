# Elevator System - Low Level Design (C++)

A comprehensive elevator system implementation in C++ demonstrating key design patterns suitable for SDE2-level technical discussions.

## 🎯 Overview

This project implements a complete elevator control system with multiple scheduling algorithms, designed to be discussed in a 1-hour technical interview. The implementation showcases:

- **Observer Pattern** for event handling and notifications
- **Command Pattern** for request processing
- **Strategy Pattern** for pluggable scheduling algorithms
- Clean separation of concerns with modular architecture

## 📁 Project Structure

```
Elevator/
├── CommonEnums.h                    # Direction and ElevatorState enums
│
├── Patterns/
│   ├── ElevatorObserver.h          # Observer interface
│   ├── ElevatorDisplay.h/cpp       # Concrete observer implementation
│   ├── ElevatorCommand.h           # Command interface
│   ├── ElevatorRequest.h/cpp       # Concrete command for requests
│   ├── SchedulingStrategy.h        # Strategy interface
│   ├── FCFSSchedulingStrategy.h/cpp    # First-Come-First-Served
│   ├── ScanSchedulingStrategy.h/cpp    # SCAN algorithm
│   └── LookSchedulingStrategy.h/cpp    # LOOK algorithm
│
├── Core/
│   ├── Floor.h/cpp                 # Floor representation
│   ├── Elevator.h/cpp              # Elevator with state management
│   ├── ElevatorController.h/cpp    # Central controller
│   └── Building.h/cpp              # Building container
│
└── main.cpp                        # Interactive simulation
```

## 🏗️ Design Patterns Implemented

### 1. Observer Pattern
**Purpose**: Real-time event notifications

```cpp
class ElevatorObserver {
    virtual void onElevatorStateChange(Elevator* elevator, ElevatorState state) = 0;
    virtual void onElevatorFloorChange(Elevator* elevator, int floor) = 0;
};
```

**Usage**: Display panels, monitoring systems, logging

### 2. Command Pattern
**Purpose**: Encapsulate requests as objects

```cpp
class ElevatorRequest : public ElevatorCommand {
    int elevatorId;
    int floor;
    Direction requestDirection;
    bool isInternalRequest;  // Button inside vs outside elevator
};
```

**Benefits**: Request queuing, prioritization, history tracking

### 3. Strategy Pattern
**Purpose**: Interchangeable scheduling algorithms

```cpp
class SchedulingStrategy {
    virtual int getNextStop(Elevator* elevator) = 0;
};
```

**Implementations**:
- **FCFS**: Simple queue-based processing
- **SCAN**: Continues in one direction until end
- **LOOK**: Optimized SCAN, only goes to last request

## 🔧 Building the Project

### Option 1: Visual Studio IDE
1. Open `Elevator.sln` in Visual Studio 2022
2. Select configuration: Debug/Release, x64
3. Build → Build Solution (Ctrl+Shift+B)
4. Run: Debug → Start Without Debugging (Ctrl+F5)

### Option 2: Developer Command Prompt
```bash
# Open "Developer Command Prompt for VS 2022"
cd D:\DesignPatterns\MultiThreading\Elevator
msbuild Elevator.sln /p:Configuration=Debug /p:Platform=x64
cd x64\Debug
Elevator.exe
```

### Option 3: MSBuild (if in PATH)
```bash
msbuild Elevator.sln /p:Configuration=Debug /p:Platform=x64
```

## 🎮 Running the Simulation

The program provides an interactive menu:

```
========================================
   Elevator System Simulation
========================================
Building: Office Tower
Floors: 10
Elevators: 3
========================================

--- Select an option ---
1. Request elevator (external)
2. Request floor (internal)
3. Simulate next step
4. Change scheduling strategy
5. Exit simulation
```

### Example Scenario

**Setup**: 3 elevators, 10 floors

1. **External Request**: Floor 7, Direction UP
   - Elevator 1 assigned to floor 7
   
2. **Internal Request**: Elevator 1 to Floor 9
   - Passenger inside selects floor 9
   
3. **Simulate Step**: Execute movement
   - Elevator 1 moves: 1→2→3→4→5→6→7 (STOPPED)
   
4. **Simulate Step**: Continue
   - Elevator 1 moves: 7→8→9 (STOPPED, IDLE)

## 📊 Scheduling Algorithms Comparison

### FCFS (First-Come-First-Served)
- **Logic**: Process requests in arrival order
- **Pros**: Simple, fair
- **Cons**: Inefficient, lots of direction changes
- **Use Case**: Low-traffic buildings

### SCAN
- **Logic**: Move in one direction to the end, then reverse
- **Pros**: Predictable, no starvation
- **Cons**: May pass floors unnecessarily
- **Use Case**: High-traffic buildings

### LOOK
- **Logic**: Move in one direction to last request, then reverse
- **Pros**: More efficient than SCAN
- **Cons**: Slightly complex logic
- **Use Case**: Optimal for most scenarios

## 🎯 Interview Discussion Points

### Core Concepts (15-20 min)
- System requirements and constraints
- Class responsibilities and relationships
- State management (Direction, ElevatorState)
- Request types (internal vs external)

### Design Patterns (15-20 min)
- Why Observer? → Decoupled event handling
- Why Command? → Request encapsulation and queuing
- Why Strategy? → Runtime algorithm switching

### Scheduling Algorithms (10-15 min)
- Trade-offs between FCFS, SCAN, LOOK
- Handling edge cases (same floor, direction changes)
- Performance considerations

### Extensions & Edge Cases (10-15 min)
- **Multiple elevators**: Load balancing strategies
- **Priority requests**: Emergency, VIP floors
- **Maintenance mode**: Taking elevators offline
- **Power failures**: State persistence
- **Concurrent requests**: Thread safety (if time permits)

## 🚀 Key Features

✅ **Modular Design**: Easy to extend and maintain  
✅ **Multiple Algorithms**: Switch strategies at runtime  
✅ **Event-Driven**: Observer pattern for real-time updates  
✅ **Type Safety**: Strong typing with enums  
✅ **Memory Safe**: Proper RAII and smart pointers  
✅ **Interactive**: Test scenarios in real-time  

## 🔍 Code Highlights

### Elevator State Management
```cpp
void Elevator::setState(ElevatorState newState) {
    this->state = newState;
    notifyStateChange(newState);  // Observer pattern
}
```

### Request Processing
```cpp
void Elevator::addRequest(const ElevatorRequest& request) {
    if (!isDuplicate(request)) {
        requests.push(request);
    }
    
    if (state == ElevatorState::IDLE) {
        determineDirection(request.getFloor());
        setState(ElevatorState::MOVING);
    }
}
```

### Strategy Selection
```cpp
controller->setSchedulingStrategy(
    std::make_unique<ScanSchedulingStrategy>()
);
```

## 📝 Complexity Analysis

- **Time Complexity**:
  - FCFS: O(1) per request
  - SCAN/LOOK: O(n) where n = pending requests
  
- **Space Complexity**: O(n) for request queue

## 🎓 Learning Outcomes

After studying this implementation, you'll understand:
- How to apply design patterns in real systems
- Trade-offs in scheduling algorithms
- State management in complex systems
- Event-driven architecture
- Clean code principles and SOLID design

## 📚 References

- [Design Elevator System - CodeWithAryan](https://codewitharyan.com/tech-blogs/design-elevator-system)
- Gang of Four Design Patterns
- Operating Systems: Disk Scheduling Algorithms (SCAN, LOOK)

## 🤝 Interview Tips

1. **Start with requirements**: Clarify floors, elevators, constraints
2. **Draw diagrams**: Class diagram, state machine, sequence diagram
3. **Discuss trade-offs**: Why this pattern? What are alternatives?
4. **Handle edge cases**: What if elevator is full? Power failure?
5. **Think scalability**: 100 floors? 50 elevators?

---

**Author**: Interview Preparation Project  
**Purpose**: SDE2 Technical Discussion  
**Time**: Designed for 1-hour discussion  
**Difficulty**: Medium to Hard

