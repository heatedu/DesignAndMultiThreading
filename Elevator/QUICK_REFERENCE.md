# Elevator System - Quick Reference Card

## 🎯 1-Minute Overview

**What**: Low-level design of elevator system in C++  
**Patterns**: Observer, Command, Strategy  
**Algorithms**: FCFS, SCAN, LOOK  
**Time**: 1-hour interview discussion  
**Level**: SDE2

---

## 📁 Project Files (23 files)

### Core Classes (8 files)
- `Building.h/cpp` - Container for elevators and floors
- `Elevator.h/cpp` - Individual elevator with state machine
- `ElevatorController.h/cpp` - Central coordinator
- `Floor.h/cpp` - Floor representation

### Patterns (13 files)
**Observer**:
- `ElevatorObserver.h` - Interface
- `ElevatorDisplay.h/cpp` - Concrete observer

**Command**:
- `ElevatorCommand.h` - Interface
- `ElevatorRequest.h/cpp` - Concrete command

**Strategy**:
- `SchedulingStrategy.h` - Interface
- `FCFSSchedulingStrategy.h/cpp` - First-Come-First-Served
- `ScanSchedulingStrategy.h/cpp` - SCAN algorithm
- `LookSchedulingStrategy.h/cpp` - LOOK algorithm

### Other (2 files)
- `CommonEnums.h` - Direction, ElevatorState
- `main.cpp` - Interactive simulation

---

## 🏗️ Class Hierarchy

```
Building
  └─ ElevatorController
       ├─ Elevator (1..N)
       │    ├─ ElevatorObserver* (0..N)
       │    └─ queue<ElevatorRequest>
       ├─ Floor (1..N)
       └─ SchedulingStrategy*
            ├─ FCFSSchedulingStrategy
            ├─ ScanSchedulingStrategy
            └─ LookSchedulingStrategy
```

---

## 🎨 Design Patterns

### Observer Pattern
**Purpose**: Notify displays when elevator state changes  
**Key Classes**: `ElevatorObserver`, `ElevatorDisplay`  
**Benefit**: Decoupled event handling

### Command Pattern
**Purpose**: Encapsulate requests as objects  
**Key Classes**: `ElevatorCommand`, `ElevatorRequest`  
**Benefit**: Request queuing, logging, replay

### Strategy Pattern
**Purpose**: Pluggable scheduling algorithms  
**Key Classes**: `SchedulingStrategy`, `FCFS`, `SCAN`, `LOOK`  
**Benefit**: Runtime algorithm switching

---

## 🔄 Elevator States

```
IDLE → MOVING → STOPPED → MOVING → ... → IDLE
                    ↓
              MAINTENANCE
```

**IDLE**: No requests, waiting  
**MOVING**: Traveling between floors  
**STOPPED**: At a floor, doors open  
**MAINTENANCE**: Out of service

---

## 🧭 Direction States

```
IDLE ⇄ UP ⇄ DOWN ⇄ IDLE
```

**IDLE**: No direction, stationary  
**UP**: Moving upward  
**DOWN**: Moving downward

---

## 📊 Scheduling Algorithms

### FCFS (First-Come-First-Served)
```
Queue: [8, 2, 6, 1]
Path: 5→8→2→6→1
```
- ✅ Simple, fair
- ❌ Inefficient, many direction changes

### SCAN
```
Queue: [8, 2, 6, 1]
Path: 5→6→8→10(end)→2→1
```
- ✅ Efficient, no starvation
- ❌ Goes to end even if no request

### LOOK
```
Queue: [8, 2, 6, 1]
Path: 5→6→8(last)→2→1
```
- ✅ Most efficient
- ❌ Slightly complex

---

## 🎮 Menu Options

```
1. Request elevator (external)  → Floor button
2. Request floor (internal)     → Elevator button
3. Simulate next step           → Execute movement
4. Change scheduling strategy   → Switch algorithm
5. Exit simulation              → Quit
```

---

## 🔧 Build Commands

### Visual Studio
```
Ctrl+Shift+B  (Build)
Ctrl+F5       (Run without debugging)
```

### Command Line
```cmd
msbuild Elevator.sln /p:Configuration=Debug /p:Platform=x64
cd x64\Debug
Elevator.exe
```

---

## 💡 Key Interview Points

### Requirements (2 min)
- Number of floors, elevators
- Request types (internal/external)
- Direction preference (UP/DOWN)

### Design Patterns (15 min)
- **Observer**: Why? → Decoupled notifications
- **Command**: Why? → Request encapsulation
- **Strategy**: Why? → Algorithm flexibility

### Algorithms (10 min)
- **FCFS**: Simple but inefficient
- **SCAN**: Efficient, predictable
- **LOOK**: Optimal for most cases

### Extensions (5 min)
- Multiple elevator selection
- Priority requests
- Maintenance mode
- Thread safety

---

## 🎯 Code Walkthrough Order

1. **Start with main.cpp** (entry point)
2. **Show Building** (container)
3. **Explain ElevatorController** (coordinator)
4. **Deep dive Elevator** (core logic)
5. **Demonstrate Observer** (event handling)
6. **Show Command** (request processing)
7. **Explain Strategy** (scheduling)
8. **Walk through one algorithm** (SCAN)

---

## 📝 Common Questions & Answers

**Q: Why Observer pattern?**  
A: Decouples elevator from displays, easy to add new observers

**Q: Internal vs External requests?**  
A: Internal = button inside elevator, External = floor button

**Q: SCAN vs LOOK difference?**  
A: SCAN goes to end, LOOK only to last request

**Q: How to handle multiple elevators?**  
A: Selection strategies: nearest, same direction, least loaded

**Q: Thread safety?**  
A: Add mutex on request queue, atomic state changes

**Q: How to test?**  
A: Unit tests per class, mock observers, strategy pattern helps

---

## 🚀 Demo Scenario (5 min)

```
Setup: 10 floors, 3 elevators, SCAN algorithm

Step 1: External request Floor 7, UP
  → Elevator 1 assigned

Step 2: Simulate
  → Elevator 1: 1→2→3→4→5→6→7 (STOPPED)

Step 3: Internal request Floor 9
  → Passenger selects destination

Step 4: Simulate
  → Elevator 1: 7→8→9 (STOPPED, IDLE)

Step 5: Switch to FCFS
  → Show algorithm flexibility

Step 6: Multiple requests
  → Show FCFS vs SCAN efficiency
```

---

## 🎓 SOLID Principles

**S**: Each class has single responsibility  
**O**: Open for extension (new strategies), closed for modification  
**L**: Strategies are interchangeable  
**I**: Small, focused interfaces  
**D**: Depend on abstractions (interfaces), not concrete classes

---

## 📊 Complexity

| Operation | Time | Space |
|-----------|------|-------|
| Add Request | O(1) | O(1) |
| FCFS Next | O(1) | O(n) |
| SCAN Next | O(n) | O(n) |
| LOOK Next | O(n) | O(n) |

---

## 🔍 Edge Cases

- ✅ Elevator at requested floor
- ✅ Empty request queue
- ✅ Direction change at boundary
- ✅ Duplicate requests
- ✅ Invalid floor numbers
- ✅ Same floor, different directions

---

## 📚 Files to Review Before Interview

1. **README.md** - Overview and features
2. **INTERVIEW_GUIDE.md** - Discussion timeline
3. **ARCHITECTURE.md** - Diagrams and structure
4. **TEST_SCENARIOS.md** - Example scenarios
5. **This file** - Quick reference

---

## ⏱️ Time Allocation (1 hour)

- **10 min**: Requirements & high-level design
- **20 min**: Design patterns discussion
- **15 min**: Scheduling algorithms
- **10 min**: Edge cases & extensions
- **5 min**: Code walkthrough & Q&A

---

## 🎯 Success Checklist

Before Interview:
- [ ] Build project successfully
- [ ] Run and test all menu options
- [ ] Review all design patterns
- [ ] Understand all 3 algorithms
- [ ] Prepare 2-3 demo scenarios
- [ ] Know edge cases

During Interview:
- [ ] Ask clarifying questions
- [ ] Draw diagrams
- [ ] Explain trade-offs
- [ ] Handle edge cases
- [ ] Discuss extensions
- [ ] Write clean code

---

## 🔗 Quick Links

| File | Purpose |
|------|---------|
| README.md | Project overview |
| INTERVIEW_GUIDE.md | Discussion guide |
| ARCHITECTURE.md | Design diagrams |
| TEST_SCENARIOS.md | Test cases |
| BUILD_INSTRUCTIONS.md | How to build |

---

## 💻 Key Code Snippets

### Observer Notification
```cpp
void Elevator::setState(ElevatorState newState) {
    this->state = newState;
    for (auto observer : observers) {
        observer->onElevatorStateChange(this, newState);
    }
}
```

### Command Execution
```cpp
void ElevatorRequest::execute() {
    if (isInternalRequest)
        controller->requestFloor(elevatorId, floor);
    else
        controller->requestElevator(elevatorId, floor, direction);
}
```

### Strategy Usage
```cpp
int nextStop = schedulingStrategy->getNextStop(elevator);
elevator->moveToNextStop(nextStop);
```

---

## 🎨 ASCII Art Reminder

```
    ┌───┐
    │ 9 │  ← Current Floor
    ├───┤
    │ 8 │
    ├───┤
    │ 7 │  ← Request
    ├───┤
    │...│
    └───┘
```

---

## 🏆 Interview Tips

1. **Start simple**, then extend
2. **Ask questions** before coding
3. **Draw diagrams** to explain
4. **Think aloud** while coding
5. **Test your logic** with examples
6. **Discuss trade-offs** explicitly
7. **Consider scalability** early
8. **Write clean, readable code**

---

## 🎯 Final Checklist

**Technical**:
- ✅ Compiles without errors
- ✅ Runs without crashes
- ✅ All patterns implemented
- ✅ All algorithms working
- ✅ Edge cases handled

**Interview Prep**:
- ✅ Can explain design decisions
- ✅ Know algorithm trade-offs
- ✅ Prepared demo scenarios
- ✅ Ready for extensions
- ✅ Confident with code

---

**Print this card and keep it handy during interview prep! 🚀**

**Good luck! You've got this! 💪**

