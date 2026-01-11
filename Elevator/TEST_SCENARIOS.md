# Elevator System - Test Scenarios

## 🧪 Test Scenarios for Interview Discussion

---

## Scenario 1: Basic Single Request

### Setup
- **Building**: 10 floors, 1 elevator
- **Elevator 1**: Floor 1, IDLE
- **Strategy**: FCFS

### Test Case
```
Step 1: External request from Floor 5, Direction UP
Step 2: Simulate

Expected:
- Elevator 1 moves: 1→2→3→4→5
- State: STOPPED at floor 5
- Direction: UP (waiting for passenger to select destination)
```

### Commands
```
Choice: 1 (Request elevator)
Elevator ID: 1
Floor: 5
Direction: 1 (UP)

Choice: 3 (Simulate)
```

---

## Scenario 2: Internal + External Requests

### Setup
- **Building**: 10 floors, 1 elevator
- **Elevator 1**: Floor 3, IDLE
- **Strategy**: SCAN

### Test Case
```
Step 1: External request Floor 7, UP
Step 2: Simulate (elevator moves to 7)
Step 3: Internal request Floor 9 (passenger boards at 7)
Step 4: Simulate (elevator moves to 9)

Expected Movement:
3→4→5→6→7 (STOPPED, picks up passenger)
7→8→9 (STOPPED, passenger exits)
State: IDLE at floor 9
```

### Commands
```
1 → 1 → 7 → 1
3
2 → 1 → 9
3
```

---

## Scenario 3: SCAN Algorithm - Multiple Requests

### Setup
- **Building**: 10 floors, 1 elevator
- **Elevator 1**: Floor 5, IDLE
- **Strategy**: SCAN

### Test Case
```
Requests (in order):
1. Floor 8, UP
2. Floor 3, DOWN
3. Floor 9, UP
4. Floor 2, DOWN

Expected Movement (SCAN):
5→6→7→8 (STOPPED) →9 (STOPPED) [continues UP direction]
9→8→7→6→5→4→3 (STOPPED) →2 (STOPPED) [then DOWN]

Total: 4 + 1 + 6 + 1 = 12 floors
```

### Why SCAN is Better
```
FCFS would do:
5→8 (3) → 8→3 (5) → 3→9 (6) → 9→2 (7) = 21 floors
SCAN: 12 floors (43% more efficient!)
```

---

## Scenario 4: Direction Change Handling

### Setup
- **Building**: 10 floors, 1 elevator
- **Elevator 1**: Floor 10, Moving DOWN
- **Strategy**: SCAN

### Test Case
```
Existing requests: [7, 5, 2] (all internal, going down)
New request: Floor 8, Direction UP (external)

Expected Behavior:
1. Complete DOWN journey: 10→7 (STOPPED) →5 (STOPPED) →2 (STOPPED)
2. Change direction to UP
3. Serve new request: 2→8 (STOPPED)

Why? SCAN completes current direction before reversing
```

### Discussion Point
> "Should we pick up the Floor 8 UP request while going down?"
> 
> **SCAN**: No, complete current direction first
> **LOOK**: Could optimize by checking if request matches direction

---

## Scenario 5: Same Floor, Different Directions

### Setup
- **Building**: 10 floors, 1 elevator
- **Elevator 1**: Floor 5, IDLE

### Test Case
```
Request 1: Floor 7, UP
Request 2: Floor 7, DOWN

Expected Behavior:
1. Elevator goes to floor 7
2. Serves UP request first (or based on arrival order)
3. Stays at floor 7 for DOWN request
4. Waits for passenger selection

Edge Case: What if both passengers board?
- Internal requests will determine final destinations
```

---

## Scenario 6: Multiple Elevators - Load Balancing

### Setup
- **Building**: 10 floors, 3 elevators
- **Elevator 1**: Floor 1, IDLE
- **Elevator 2**: Floor 5, MOVING UP (going to 8)
- **Elevator 3**: Floor 10, IDLE

### Test Case
```
New request: Floor 6, Direction DOWN

Selection Strategies:

1. Nearest Elevator:
   - E1: |6-1| = 5 floors
   - E2: |6-5| = 1 floor (but busy)
   - E3: |6-10| = 4 floors
   → Choose E2 (nearest)

2. Least Loaded:
   - E1: 0 pending requests
   - E2: 1 pending request
   - E3: 0 pending requests
   → Choose E1 or E3

3. Same Direction Preference:
   - E2 is going UP, request is DOWN
   → Deprioritize E2
   → Choose E3 (closer than E1)

Best: Combination of factors
```

---

## Scenario 7: FCFS vs SCAN Comparison

### Setup
- **Building**: 10 floors, 1 elevator
- **Elevator 1**: Floor 5, IDLE

### Test Case
```
Requests: [2, 8, 3, 9, 1]

FCFS:
5→2 (3) → 2→8 (6) → 8→3 (5) → 3→9 (6) → 9→1 (8)
Total: 28 floors

SCAN (assuming first request sets direction DOWN):
5→3 (2) →2 (1) →1 (1) [reverse] 1→8 (7) →9 (1)
Total: 12 floors

Efficiency: SCAN is 57% better!
```

---

## Scenario 8: LOOK Algorithm Optimization

### Setup
- **Building**: 10 floors, 1 elevator
- **Elevator 1**: Floor 5, Moving UP
- **Strategy**: LOOK

### Test Case
```
Requests: [7, 9] (internal, going up)
New request: Floor 8, UP (external)

SCAN behavior:
5→7 (STOP) →8 (STOP) →9 (STOP) →10 (no request, but goes anyway)
Total: 5 floors

LOOK behavior:
5→7 (STOP) →8 (STOP) →9 (STOP, last request, reverse)
Total: 4 floors

LOOK saves 1 floor by not going to 10
```

---

## Scenario 9: Priority Request (Extension)

### Setup
- **Building**: 10 floors, 1 elevator
- **Elevator 1**: Floor 5, IDLE
- **Strategy**: Priority-based

### Test Case
```
Requests:
1. Floor 8, NORMAL priority
2. Floor 3, NORMAL priority
3. Floor 6, EMERGENCY priority

Expected Behavior:
1. Process EMERGENCY first: 5→6
2. Then NORMAL in order: 6→8, 8→3

Priority Queue:
[EMERGENCY: 6] → [NORMAL: 8] → [NORMAL: 3]
```

---

## Scenario 10: Maintenance Mode

### Setup
- **Building**: 10 floors, 2 elevators
- **Elevator 1**: Floor 7, MOVING (going to 9)
- **Elevator 2**: Floor 3, IDLE

### Test Case
```
Action: Put Elevator 1 in MAINTENANCE

Expected Behavior:
1. Complete current request: 7→9 (STOPPED)
2. Stop accepting new requests
3. Move to ground floor: 9→1
4. State: MAINTENANCE

New requests automatically assigned to Elevator 2
```

---

## 🎯 Edge Cases to Discuss

### Edge Case 1: Elevator Already at Requested Floor
```cpp
if (currentFloor == requestedFloor) {
    // Open doors immediately
    setState(STOPPED);
    return currentFloor;
}
```

### Edge Case 2: All Elevators Full
```cpp
if (allElevatorsFull()) {
    // Queue request for next available elevator
    // Or reject request with error message
}
```

### Edge Case 3: Request for Invalid Floor
```cpp
if (floor < 1 || floor > numberOfFloors) {
    throw InvalidFloorException(floor);
}
```

### Edge Case 4: Duplicate Requests
```cpp
// Same floor, same direction
if (isDuplicate(request)) {
    // Don't add to queue
    return;
}
```

### Edge Case 5: Direction Change at Boundary
```cpp
// At floor 1, can only go UP
if (currentFloor == 1) {
    direction = UP;
}
// At floor 10, can only go DOWN
if (currentFloor == numberOfFloors) {
    direction = DOWN;
}
```

---

## 🧮 Performance Metrics

### Metrics to Track
1. **Average Wait Time**: Time from request to pickup
2. **Average Travel Time**: Time from pickup to destination
3. **Elevator Utilization**: % of time elevator is moving
4. **Request Queue Length**: Number of pending requests
5. **Direction Changes**: How often elevator reverses

### Example Calculation
```
Scenario: 10 requests over 5 minutes

FCFS:
- Total floors traveled: 80
- Average wait time: 45 seconds
- Direction changes: 8

SCAN:
- Total floors traveled: 50
- Average wait time: 30 seconds
- Direction changes: 2

SCAN is 37.5% more efficient in floors traveled
SCAN is 33% faster in wait time
```

---

## 🎬 Demo Script for Interview

### 5-Minute Demo
```
1. Start simulation
   "Let me show you the system in action..."

2. Create external request
   "A person on floor 7 wants to go UP"
   → 1 → 1 → 7 → 1

3. Simulate movement
   "The elevator moves from floor 1 to 7"
   → 3

4. Create internal request
   "The passenger selects floor 9"
   → 2 → 1 → 9

5. Simulate movement
   "The elevator continues to floor 9"
   → 3

6. Show strategy change
   "Now let's switch to SCAN algorithm"
   → 4 → 1

7. Create multiple requests
   "Multiple people request elevators"
   → 1 → 1 → 3 → 2
   → 1 → 1 → 8 → 1
   → 2 → 1 → 5

8. Simulate with SCAN
   "Watch how SCAN optimizes the path"
   → 3 (multiple times)
```

---

## 📊 Test Coverage Checklist

### Functional Tests
- ✅ Single elevator, single request
- ✅ Single elevator, multiple requests
- ✅ Multiple elevators, load balancing
- ✅ Internal vs external requests
- ✅ Direction changes
- ✅ Same floor requests

### Algorithm Tests
- ✅ FCFS correctness
- ✅ SCAN correctness
- ✅ LOOK correctness
- ✅ Strategy switching at runtime

### Edge Cases
- ✅ Empty request queue
- ✅ Elevator at requested floor
- ✅ Invalid floor numbers
- ✅ Duplicate requests
- ✅ Boundary conditions (floor 1, floor N)

### Performance Tests
- ✅ Large number of requests
- ✅ Large number of floors
- ✅ Multiple elevators coordination
- ✅ Algorithm efficiency comparison

---

## 🎯 Interview Success Criteria

### Demonstrates Understanding Of:
1. ✅ Object-oriented design principles
2. ✅ Design patterns (Observer, Command, Strategy)
3. ✅ Algorithm trade-offs (FCFS vs SCAN vs LOOK)
4. ✅ State management
5. ✅ Edge case handling
6. ✅ Code organization and modularity
7. ✅ Testing strategies
8. ✅ Scalability considerations

### Bonus Points For:
- 🌟 Discussing thread safety
- 🌟 Proposing optimizations
- 🌟 Considering real-world constraints
- 🌟 Suggesting monitoring/metrics
- 🌟 Thinking about failure modes

---

**Use these scenarios to practice and prepare for your interview! 🚀**

