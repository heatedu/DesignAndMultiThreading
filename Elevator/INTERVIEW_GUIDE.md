# Elevator System - Interview Discussion Guide

## 📋 1-Hour Interview Timeline

### Phase 1: Requirements & High-Level Design (10 min)

#### Initial Questions to Ask
- How many floors in the building?
- How many elevators?
- Types of requests: internal (inside elevator) vs external (floor buttons)?
- Do we need to handle direction preference (UP/DOWN buttons)?
- Any special requirements? (emergency, maintenance, priority)

#### High-Level Components
```
Building
  └── ElevatorController
        ├── Elevator 1
        ├── Elevator 2
        └── Elevator N
        
Floor Buttons (External) → ElevatorController → Assign Elevator
Elevator Buttons (Internal) → Specific Elevator → Add to Queue
```

#### Core Classes
1. **Elevator**: Individual elevator car with state
2. **ElevatorController**: Manages all elevators, assigns requests
3. **Building**: Container for floors and controller
4. **Floor**: Represents a floor in the building

---

### Phase 2: Design Patterns Discussion (20 min)

#### 2.1 Observer Pattern (5-7 min)

**Problem**: How do we notify displays, monitoring systems when elevator state changes?

**Solution**: Observer Pattern

```cpp
// Observer Interface
class ElevatorObserver {
    virtual void onElevatorStateChange(Elevator*, ElevatorState) = 0;
    virtual void onElevatorFloorChange(Elevator*, int floor) = 0;
};

// Concrete Observer
class ElevatorDisplay : public ElevatorObserver {
    void onElevatorStateChange(Elevator* e, ElevatorState state) {
        cout << "Elevator " << e->getId() << " is " << state << endl;
    }
};
```

**Benefits**:
- Decoupled components
- Multiple observers (display, logger, analytics)
- Easy to add new observers without modifying Elevator class

**Discussion Points**:
- What if we didn't use Observer? → Tight coupling, hard to extend
- How many observers can we have? → Unlimited
- Performance considerations? → O(n) notification time

---

#### 2.2 Command Pattern (5-7 min)

**Problem**: How do we encapsulate elevator requests for queuing, logging, undo?

**Solution**: Command Pattern

```cpp
// Command Interface
class ElevatorCommand {
    virtual void execute() = 0;
};

// Concrete Command
class ElevatorRequest : public ElevatorCommand {
    int elevatorId;
    int floor;
    Direction direction;
    bool isInternalRequest;
    
    void execute() {
        if (isInternalRequest)
            controller->requestFloor(elevatorId, floor);
        else
            controller->requestElevator(elevatorId, floor, direction);
    }
};
```

**Benefits**:
- Requests as first-class objects
- Can queue, log, replay requests
- Supports undo/redo (if needed)
- Separates request creation from execution

**Discussion Points**:
- Internal vs External requests? → Internal: inside elevator, External: floor button
- Why not just call methods directly? → Lose flexibility, can't queue/log
- How to handle request cancellation? → Add cancel() method

---

#### 2.3 Strategy Pattern (5-7 min)

**Problem**: Different scheduling algorithms for different scenarios

**Solution**: Strategy Pattern

```cpp
// Strategy Interface
class SchedulingStrategy {
    virtual int getNextStop(Elevator* elevator) = 0;
};

// Concrete Strategies
class FCFSSchedulingStrategy : public SchedulingStrategy { ... };
class ScanSchedulingStrategy : public SchedulingStrategy { ... };
class LookSchedulingStrategy : public SchedulingStrategy { ... };
```

**Benefits**:
- Runtime algorithm switching
- Easy to add new algorithms
- Testable in isolation

**Discussion Points**:
- When to use FCFS vs SCAN vs LOOK?
- Can we combine strategies? → Yes, time-based or load-based
- How to choose optimal strategy? → Building traffic patterns

---

### Phase 3: Scheduling Algorithms Deep Dive (15 min)

#### 3.1 FCFS (First-Come-First-Served)

**Algorithm**:
```
1. Take first request from queue
2. Move elevator to that floor
3. Remove request, repeat
```

**Example**:
```
Current Floor: 5
Requests: [8, 2, 6, 1]

Movement: 5→8 (stop) → 8→2 (stop) → 2→6 (stop) → 6→1 (stop)
Total: 3 + 6 + 4 + 5 = 18 floors
```

**Pros**: Simple, fair, no starvation  
**Cons**: Inefficient, lots of direction changes  
**Use Case**: Low-traffic buildings

---

#### 3.2 SCAN Algorithm

**Algorithm**:
```
1. Continue in current direction until no more requests
2. Reverse direction
3. Repeat
```

**Example**:
```
Current Floor: 5, Direction: UP
Requests: [8, 2, 6, 1]

Movement: 5→6 (stop) →8 (stop) → 8→2 (stop) →1 (stop)
Total: 1 + 2 + 6 + 1 = 10 floors
```

**Key Code**:
```cpp
int ScanSchedulingStrategy::getNextStop(Elevator* elevator) {
    // Separate requests into UP and DOWN
    vector<int> upFloors, downFloors;
    
    // If moving UP, serve all UP requests first
    if (direction == UP && !upFloors.empty())
        return upFloors[0];  // Closest up floor
    
    // Switch direction when no more requests in current direction
    return switchDirection(elevator, downFloors);
}
```

**Pros**: Predictable, efficient, no starvation  
**Cons**: May go to end even if no requests  
**Use Case**: High-traffic buildings

---

#### 3.3 LOOK Algorithm

**Algorithm**:
```
1. Continue in current direction to LAST request (not end)
2. Reverse direction
3. Repeat
```

**Example**:
```
Current Floor: 5, Direction: UP
Requests: [8, 2, 6, 1]

Movement: 5→6 (stop) →8 (stop) → 8→2 (stop) →1 (stop)
Total: 1 + 2 + 6 + 1 = 10 floors
(Same as SCAN in this case, but avoids going to floor 10 if no request)
```

**Key Difference from SCAN**:
```cpp
// SCAN: Goes to end (floor 1 or floor 10)
// LOOK: Goes to last request only

int LookSchedulingStrategy::getNextStop(Elevator* elevator) {
    // Find requests between current floor and primary target
    for (auto req : requests) {
        if (travelDirection == UP && 
            req.floor > currentFloor && 
            req.floor <= primaryTarget) {
            // Serve intermediate stops
        }
    }
}
```

**Pros**: More efficient than SCAN  
**Cons**: Slightly more complex  
**Use Case**: Optimal for most scenarios

---

### Phase 4: Edge Cases & Extensions (10 min)

#### 4.1 Edge Cases

**1. Elevator at Requested Floor**
```cpp
if (currentFloor == requestedFloor) {
    return currentFloor;  // Already there
}
```

**2. Direction Change Handling**
```
Scenario: Elevator at floor 10, moving DOWN, requests at [6, 2]
External request: Floor 4, Direction UP

Solution: 
- Complete DOWN journey: 10→6→2
- Then reverse and serve: 2→4 (UP)
```

**3. Duplicate Requests**
```cpp
void Elevator::addRequest(const ElevatorRequest& req) {
    if (!isDuplicate(req)) {
        requests.push(req);
    }
}
```

**4. Empty Request Queue**
```cpp
if (requests.empty()) {
    direction = IDLE;
    state = IDLE;
    return currentFloor;
}
```

---

#### 4.2 Extensions

**1. Multiple Elevator Selection**
```cpp
Elevator* selectBestElevator(int floor, Direction dir) {
    // Strategy 1: Nearest elevator
    // Strategy 2: Same direction preference
    // Strategy 3: Least loaded
    // Strategy 4: Combination
}
```

**2. Priority Requests**
```cpp
enum Priority { NORMAL, HIGH, EMERGENCY };

class PriorityRequest : public ElevatorRequest {
    Priority priority;
};

// Use priority queue instead of FIFO queue
priority_queue<PriorityRequest> requests;
```

**3. Load Balancing**
```cpp
class LoadBalancer {
    int getElevatorLoad(Elevator* e) {
        return e->getRequestsQueue().size();
    }
    
    Elevator* selectLeastLoaded() {
        // Return elevator with minimum pending requests
    }
};
```

**4. Maintenance Mode**
```cpp
enum ElevatorState {
    IDLE, MOVING, STOPPED, MAINTENANCE
};

void Elevator::enterMaintenance() {
    // Complete current requests
    // Stop accepting new requests
    // Move to ground floor
    setState(MAINTENANCE);
}
```

**5. Thread Safety (Advanced)**
```cpp
class ThreadSafeElevator : public Elevator {
    mutable std::mutex mtx;
    
    void addRequest(const ElevatorRequest& req) {
        std::lock_guard<std::mutex> lock(mtx);
        Elevator::addRequest(req);
    }
};
```

---

### Phase 5: Code Walkthrough & Q&A (5 min)

#### Key Code Sections to Review

**1. Elevator Movement**
```cpp
void Elevator::moveToNextStop(int nextStop) {
    while (currentFloor != nextStop) {
        if (direction == UP) currentFloor++;
        else currentFloor--;
        
        notifyFloorChange(currentFloor);
        
        if (currentFloor == nextStop) {
            completeArrival();
        }
    }
}
```

**2. Controller Step Simulation**
```cpp
void ElevatorController::step() {
    for (auto elevator : elevators) {
        if (!elevator->getRequestsQueue().empty()) {
            int nextStop = schedulingStrategy->getNextStop(elevator);
            elevator->moveToNextStop(nextStop);
        }
    }
}
```

**3. Strategy Switching**
```cpp
// Runtime strategy change
controller->setSchedulingStrategy(
    std::make_unique<ScanSchedulingStrategy>()
);
```

---

## 🎯 Common Interview Questions

### Q1: Why use design patterns?
**A**: 
- **Observer**: Decouples event handling, multiple listeners
- **Command**: Request encapsulation, queuing, logging
- **Strategy**: Runtime algorithm switching, testability

### Q2: How to handle multiple elevators?
**A**: 
- Nearest elevator selection
- Same direction preference
- Load balancing
- Combination of factors

### Q3: SCAN vs LOOK difference?
**A**:
- **SCAN**: Goes to end of building in current direction
- **LOOK**: Only goes to last request, more efficient

### Q4: Internal vs External requests?
**A**:
- **Internal**: Button inside elevator (passenger selects floor)
- **External**: Button on floor (call elevator with direction)

### Q5: How to prevent starvation?
**A**:
- SCAN/LOOK algorithms guarantee eventual service
- FCFS is fair but inefficient
- Can add request aging/priority

### Q6: Thread safety considerations?
**A**:
- Mutex on request queue
- Atomic state changes
- Lock-free queues for performance
- Reader-writer locks for status queries

### Q7: How to test this system?
**A**:
- Unit tests for each class
- Mock observers for testing notifications
- Strategy pattern makes algorithms testable in isolation
- Integration tests for full scenarios

### Q8: Scalability concerns?
**A**:
- 100 floors: No major changes needed
- 50 elevators: Need better selection algorithm
- Distributed system: Message queue for requests
- Real-time constraints: Optimize algorithm complexity

---

## 📊 Complexity Analysis

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Add Request | O(1) | O(1) |
| FCFS Next Stop | O(1) | O(n) |
| SCAN Next Stop | O(n) | O(n) |
| LOOK Next Stop | O(n) | O(n) |
| Notify Observers | O(m) | O(m) |

Where:
- n = number of pending requests
- m = number of observers

---

## 🎓 Key Takeaways

1. **Design Patterns**: Not just theoretical, solve real problems
2. **Trade-offs**: Every algorithm has pros/cons
3. **Extensibility**: Good design makes future changes easy
4. **State Management**: Critical in complex systems
5. **Separation of Concerns**: Each class has single responsibility

---

## 💡 Pro Tips for Interview

1. **Ask clarifying questions** before coding
2. **Start with simple solution**, then optimize
3. **Draw diagrams** to explain design
4. **Discuss trade-offs** explicitly
5. **Think about edge cases** proactively
6. **Consider scalability** from the start
7. **Write clean, readable code**
8. **Test your logic** with examples

---

## 🔗 Related Concepts

- **Operating Systems**: Disk scheduling (SCAN, C-SCAN, LOOK)
- **Networking**: Request routing, load balancing
- **Databases**: Query scheduling, transaction management
- **Real-time Systems**: Priority scheduling, deadline guarantees

---

**Good luck with your interview! 🚀**

