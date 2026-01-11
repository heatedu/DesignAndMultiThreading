# Rate Limiter - Interview Quick Reference

## Problem Statement
Design a rate limiter that controls the rate at which requests can be processed, supporting both global and per-user limiting with thread safety.

## Key Requirements
- ✅ Token Bucket algorithm (extensible to others)
- ✅ Configurable capacity & refill rate
- ✅ Thread-safe concurrent operations
- ✅ Asynchronous request processing
- ✅ Per-user and global rate limiting

## Core APIs

### IRateLimiter Interface
```cpp
bool giveAccess(const std::string& rateLimitKey);
    // Returns true if request allowed, false if blocked
    // Empty key = global limiting, non-empty = per-user

void updateConfiguration(const std::unordered_map<std::string, int>& config);
    // Dynamically adjust refreshRate

void shutdown();
    // Clean up threads and resources
```

## Component Breakdown

### 1. Bucket (Inner Class in TokenBucketStrategy)
**Purpose**: Thread-safe token storage
```cpp
class Bucket {
    int tokens;
    std::mutex lock;
    
    bool tryConsume();           // O(1) - lock, decrement, unlock
    void refill(rate, capacity); // O(1) - lock, add tokens, unlock
}
```

### 2. TokenBucketStrategy
**Purpose**: Implements Token Bucket algorithm
```cpp
- bucketCapacity: max tokens
- refreshRate: tokens added per second
- globalBucket: for empty keys
- userBuckets: unordered_map<string, unique_ptr<Bucket>>
- refillThread: background thread calling refill() every 1s
- mapMutex: protects userBuckets map during bucket creation
```

**Key Method**: `giveAccess(key)`
1. Check if key empty → use globalBucket
2. Else → lookup/create bucket in userBuckets (with mapMutex)
3. Call bucket->tryConsume()

### 3. RateLimiterFactory
**Purpose**: Create rate limiters via Factory pattern
```cpp
static unordered_map<RateLimiterType, FactoryFunction> factories;

// Usage:
auto limiter = RateLimiterFactory::createLimiter(
    RateLimiterType::TOKEN_BUCKET, 
    {{"capacity", 5}, {"refreshRate", 1}}
);
```

### 4. RateLimiterController
**Purpose**: Async request processing with thread pool
```cpp
- workers: vector<thread>
- tasks: queue<function<void()>>
- queueMutex + condition_variable
- rateLimiter: unique_ptr<IRateLimiter>

std::future<bool> processRequest(key);
    // Enqueues task, notifies worker, returns future
```

## Thread Safety Strategy

| Component | Mechanism | Why |
|-----------|-----------|-----|
| Bucket | `std::mutex` | Protects token count during consume/refill |
| userBuckets map | `std::mutex` (mapMutex) | Protects map during bucket lookup/creation |
| Task queue | `std::mutex` + `condition_variable` | Thread pool coordination |
| Refill thread | Atomic bool `running` | Clean shutdown |

## Token Bucket Algorithm Flow

**Request Processing**:
```
1. Request arrives with key (or empty for global)
2. Controller enqueues task to thread pool
3. Worker thread calls rateLimiter->giveAccess(key)
4. TokenBucketStrategy:
   a. Get/create bucket for key (lock mapMutex if needed)
   b. Call bucket->tryConsume() (locks bucket mutex)
   c. If tokens > 0: decrement, return true
   d. Else: return false
5. Controller prints result, resolves future
```

**Token Refilling**:
```
Background thread loop (every 1000ms):
1. Refill globalBucket
2. Lock mapMutex
3. Iterate all userBuckets, call refill() on each
4. Unlock mapMutex
5. Sleep 1s, repeat
```

## Example Walkthrough

**Config**: capacity=5, refreshRate=1

**Scenario**: 10 burst requests (global)
```
Request 1-5: tokens=5,4,3,2,1 → ✅ Allowed
Request 6-10: tokens=0 → ❌ Blocked

Wait 5 seconds → refill adds 5 tokens (5 * 1 token/sec)

Next 10 requests:
Request 1-5: tokens=5,4,3,2,1 → ✅ Allowed
Request 6-10: tokens=0 → ❌ Blocked
```

## Design Patterns Used

1. **Strategy Pattern**: `IRateLimiter` interface allows swapping algorithms
2. **Factory Pattern**: `RateLimiterFactory` creates appropriate strategies
3. **Thread Pool Pattern**: Controller manages worker threads
4. **RAII**: Smart pointers (`unique_ptr`) for automatic cleanup

## Common Interview Questions

### Q1: Why mutex per bucket instead of global lock?
**A**: Fine-grained locking improves concurrency. Different users don't block each other.

### Q2: What if refill thread crashes?
**A**: Use `std::thread` with proper exception handling. In production, add health checks and restart logic.

### Q3: How to scale horizontally?
**A**: Replace in-memory buckets with Redis:
- Use Redis hash for bucket state
- Use INCR/DECR for atomic token operations
- Use Lua script for atomic read-modify-write

### Q4: Why not std::async for every request?
**A**: Thread creation overhead. Thread pool reuses threads for better performance.

### Q5: How to add Fixed Window algorithm?
```cpp
// In RateLimiterFactory:
factories[RateLimiterType::FIXED_WINDOW] = [](config) {
    return std::make_unique<FixedWindowStrategy>(
        config["windowSize"], config["maxRequests"]
    );
};

// Implement FixedWindowStrategy : public IRateLimiter
class FixedWindowStrategy : public IRateLimiter {
    // Track request count + window start time
    // Reset count when window expires
};
```

### Q6: Memory concerns with many users?
**A**: 
- LRU eviction for inactive buckets
- Periodic cleanup thread removing old entries
- DB persistence with in-memory cache

## Time Complexity Analysis

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| giveAccess() | O(1) avg | Hash lookup + mutex lock |
| refill() | O(n) | n = active buckets |
| updateConfiguration() | O(1) | Atomic write |
| shutdown() | O(t) | t = thread count |

## Code Quality Points to Mention

1. ✅ **RAII**: `unique_ptr` prevents memory leaks
2. ✅ **const correctness**: Pass `const std::string&` for keys
3. ✅ **Exception safety**: Mutex uses RAII via `lock_guard`
4. ✅ **Move semantics**: `std::move` for task functions
5. ✅ **Atomic types**: `atomic<bool>` for thread communication

## Quick Demo Commands (Visual Studio)

```bash
# Build
MSBuild RateLimiter.vcxproj /p:Configuration=Debug

# Or in VS:
Ctrl+Shift+B (Build Solution)
F5 (Run with debugging)
Ctrl+F5 (Run without debugging)
```

## Interview Flow (60 min)

| Time | Phase | Topics |
|------|-------|--------|
| 0-10 | Requirements | Clarify algorithms, thread safety, global vs per-user |
| 10-25 | Design | Strategy+Factory patterns, thread safety approach |
| 25-50 | Implementation | Walk through Bucket, TokenBucketStrategy, Controller |
| 50-60 | Discussion | Extensions, trade-offs, production considerations |

## Key Talking Points

1. **Why Token Bucket?** Allows bursts while maintaining average rate
2. **Thread Safety**: Multiple layers - bucket mutex, map mutex, atomic bool
3. **Extensibility**: Easy to add Fixed Window, Sliding Window, etc.
4. **Production Ready**: Shutdown logic, exception safety, configurable
5. **Trade-offs**: Memory (per-user buckets) vs Fairness vs Simplicity

## Common Pitfalls to Avoid

❌ Forgetting to lock map when creating new buckets
❌ Not joining refill thread in shutdown
❌ Using shared global mutex (poor concurrency)
❌ Not handling empty string vs nullptr for global limiting
❌ Race condition between refill and consume

✅ Our implementation handles all of these!

