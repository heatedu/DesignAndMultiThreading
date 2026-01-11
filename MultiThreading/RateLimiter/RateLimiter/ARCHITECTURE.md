# Rate Limiter Architecture

## System Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                         Client Code (main.cpp)                   │
│                                                                   │
│  • Creates RateLimiterController                                 │
│  • Submits requests via processRequest()                         │
│  • Receives std::future<bool> results                            │
└───────────────────────────────┬─────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────────┐
│                     RateLimiterController                        │
│                                                                   │
│  ┌────────────────┐    ┌──────────────────────────────────┐    │
│  │  Thread Pool   │    │      IRateLimiter*                │    │
│  │                │    │   (Strategy Interface)            │    │
│  │  • workers[]   │────▶   • giveAccess()                  │    │
│  │  • task queue  │    │   • updateConfiguration()         │    │
│  │  • mutex       │    │   • shutdown()                    │    │
│  │  • cond_var    │    └──────────────────────────────────┘    │
│  └────────────────┘                                              │
└───────────────────────────────────┬─────────────────────────────┘
                                    │ Created by Factory
                                    ▼
┌─────────────────────────────────────────────────────────────────┐
│                     RateLimiterFactory                           │
│                                                                   │
│  static map<RateLimiterType, FactoryFunction>                   │
│                                                                   │
│  • TOKEN_BUCKET   ──▶  new TokenBucketStrategy()                │
│  • FIXED_WINDOW   ──▶  (extensible)                             │
│  • SLIDING_WINDOW ──▶  (extensible)                             │
│  • LEAKY_BUCKET   ──▶  (extensible)                             │
└───────────────────────────────────┬─────────────────────────────┘
                                    │
                                    ▼
┌─────────────────────────────────────────────────────────────────┐
│                    TokenBucketStrategy                           │
│                     (implements IRateLimiter)                    │
│                                                                   │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │              Configuration                                │  │
│  │  • bucketCapacity: int (max tokens)                       │  │
│  │  • refreshRate: atomic<int> (tokens/sec)                  │  │
│  │  • REFILL_INTERVAL_MS: 1000ms                             │  │
│  └──────────────────────────────────────────────────────────┘  │
│                                                                   │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │              Global Bucket                                │  │
│  │  globalBucket: unique_ptr<Bucket>                         │  │
│  │  (for requests with empty key)                            │  │
│  └──────────────────────────────────────────────────────────┘  │
│                                                                   │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │            Per-User Buckets                               │  │
│  │  userBuckets: unordered_map<string, unique_ptr<Bucket>>  │  │
│  │  mapMutex: mutex (protects map operations)               │  │
│  │                                                            │  │
│  │  "user1" ──▶ Bucket { tokens: 3, mutex }                 │  │
│  │  "user2" ──▶ Bucket { tokens: 5, mutex }                 │  │
│  │  "user3" ──▶ Bucket { tokens: 0, mutex }                 │  │
│  └──────────────────────────────────────────────────────────┘  │
│                                                                   │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │            Refill Thread                                  │  │
│  │  refillThread: std::thread                                │  │
│  │  running: atomic<bool>                                    │  │
│  │                                                            │  │
│  │  Loop every 1000ms:                                       │  │
│  │    1. Refill globalBucket                                 │  │
│  │    2. Lock mapMutex                                       │  │
│  │    3. Refill all userBuckets                              │  │
│  │    4. Unlock mapMutex                                     │  │
│  └──────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│                        Bucket (Inner Class)                      │
│                                                                   │
│  ┌─────────────────────────────────────────────────────────┐   │
│  │  • tokens: int                                           │   │
│  │  • lock: mutex                                           │   │
│  └─────────────────────────────────────────────────────────┘   │
│                                                                   │
│  bool tryConsume():                                              │
│    lock_guard<mutex> guard(lock);                                │
│    if (tokens > 0) {                                             │
│      tokens--;                                                   │
│      return true;                                                │
│    }                                                             │
│    return false;                                                 │
│                                                                   │
│  void refill(rate, capacity):                                    │
│    lock_guard<mutex> guard(lock);                                │
│    tokens = min(capacity, tokens + rate);                        │
└─────────────────────────────────────────────────────────────────┘
```

## Request Flow Diagram

```
┌─────────┐
│ Client  │
│ Thread  │
└────┬────┘
     │ 1. processRequest("user1")
     ▼
┌────────────────────────────┐
│ RateLimiterController      │
│                            │
│  2. Create promise/future  │
│  3. Enqueue task λ         │────┐
│  4. Notify worker          │    │
│  5. Return future          │    │
└────────────────────────────┘    │
                                  │
                  ┌───────────────┘
                  │
                  ▼
          ┌──────────────┐
          │ Worker Thread│
          │  (from pool) │
          └──────┬───────┘
                 │ 6. Dequeue task
                 ▼
    ┌─────────────────────────────┐
    │ Execute task:               │
    │   allowed = rateLimiter->   │
    │     giveAccess("user1")     │
    └──────────┬──────────────────┘
               │
               ▼
    ┌─────────────────────────────┐
    │ TokenBucketStrategy::       │
    │   giveAccess("user1")       │
    │                             │
    │ 7. Lock mapMutex            │
    │ 8. Find/create user bucket  │
    │ 9. Unlock mapMutex          │
    │ 10. Call bucket->tryConsume()│
    └──────────┬──────────────────┘
               │
               ▼
    ┌─────────────────────────────┐
    │ Bucket::tryConsume()        │
    │                             │
    │ 11. Lock bucket mutex       │
    │ 12. Check tokens > 0?       │
    │     YES: decrement, true    │
    │     NO:  false              │
    │ 13. Unlock bucket mutex     │
    └──────────┬──────────────────┘
               │
               ▼ (return bool)
    ┌─────────────────────────────┐
    │ Worker Thread               │
    │                             │
    │ 14. Print result            │
    │ 15. promise->set_value()    │
    └─────────────────────────────┘
               │
               ▼
    ┌─────────────────────────────┐
    │ Client Thread               │
    │                             │
    │ 16. future.get() returns    │
    │     true/false              │
    └─────────────────────────────┘
```

## Refill Thread Flow

```
┌─────────────────────────────────────┐
│  Refill Thread                      │
│  (runs in background)               │
└──────────┬──────────────────────────┘
           │
           ▼
    ┌─────────────────────┐
    │ while (running) {   │
    └──────┬──────────────┘
           │
           ▼
    ┌─────────────────────────────┐
    │ sleep(1000ms)               │
    └──────┬──────────────────────┘
           │
           ▼
    ┌─────────────────────────────┐
    │ Refill Global Bucket:       │
    │   globalBucket->refill(     │
    │     refreshRate, capacity)  │
    └──────┬──────────────────────┘
           │
           ▼
    ┌─────────────────────────────┐
    │ Lock mapMutex               │
    └──────┬──────────────────────┘
           │
           ▼
    ┌─────────────────────────────┐
    │ for (auto& [key, bucket]:   │
    │      userBuckets) {         │
    │   bucket->refill(           │
    │     refreshRate, capacity)  │
    │ }                           │
    └──────┬──────────────────────┘
           │
           ▼
    ┌─────────────────────────────┐
    │ Unlock mapMutex             │
    └──────┬──────────────────────┘
           │
           └─────▶ (repeat loop)
```

## Thread Safety Model

### Lock Hierarchy (to prevent deadlocks)

```
Level 1: mapMutex (TokenBucketStrategy)
  └─▶ Protects userBuckets map during lookup/insert

Level 2: Bucket::lock (individual bucket mutex)
  └─▶ Protects token count during consume/refill

Level 3: queueMutex (RateLimiterController)
  └─▶ Protects task queue in thread pool

Rule: Always acquire locks in order: mapMutex → Bucket::lock → queueMutex
```

### Concurrent Access Scenarios

#### Scenario 1: Multiple requests for same user
```
Thread A: giveAccess("user1")
Thread B: giveAccess("user1")

Both threads:
1. Lock mapMutex briefly to get bucket pointer
2. Unlock mapMutex
3. Lock bucket->lock independently
4. Perform token operation
5. Unlock bucket->lock

✅ No deadlock: Different buckets can be accessed in parallel
✅ Safe: Same bucket serialized by bucket mutex
```

#### Scenario 2: Refill + Request simultaneously
```
Refill Thread: Iterating userBuckets
Request Thread: giveAccess("user1")

Refill Thread:
1. Locks mapMutex (prevents map modification)
2. Locks each bucket->lock (one at a time)
3. Refills tokens
4. Unlocks bucket->lock
5. Moves to next bucket
6. Unlocks mapMutex

Request Thread:
1. Waits for mapMutex if refill is iterating
2. Gets bucket pointer
3. Unlocks mapMutex
4. Locks bucket->lock (might wait if refill is on same bucket)
5. Consumes token
6. Unlocks bucket->lock

✅ Safe: Map iteration protected by mapMutex
✅ Safe: Token operations protected by bucket mutex
```

#### Scenario 3: New user bucket creation
```
Thread A: giveAccess("newUser") - first request
Thread B: giveAccess("newUser") - simultaneous request

Both threads:
1. Lock mapMutex
2. Check if bucket exists
   - First thread: Creates bucket
   - Second thread: Finds existing bucket
3. Unlock mapMutex

✅ Safe: Only one thread creates bucket
✅ Efficient: No duplicate buckets
```

## Memory Layout

```
TokenBucketStrategy Object:
├─ bucketCapacity: 4 bytes
├─ refreshRate: 8 bytes (atomic<int>)
├─ globalBucket: 8 bytes ptr → Bucket(4 bytes tokens + mutex)
├─ userBuckets: ~64 bytes (map overhead)
│  ├─ "user1" → unique_ptr → Bucket (~40 bytes)
│  ├─ "user2" → unique_ptr → Bucket (~40 bytes)
│  └─ "userN" → unique_ptr → Bucket (~40 bytes)
├─ mapMutex: ~40 bytes
├─ refillThread: ~8 bytes (thread handle)
└─ running: 1 byte (atomic<bool>)

Total per strategy: ~200 bytes + (40 bytes × num_users)

Example: 1000 users ≈ 40KB memory
```

## Design Patterns Applied

### 1. Strategy Pattern
```
IRateLimiter (interface)
    ▲
    │
    ├── TokenBucketStrategy
    ├── FixedWindowStrategy (future)
    ├── SlidingWindowStrategy (future)
    └── LeakyBucketStrategy (future)

Benefit: Swap algorithms without changing controller
```

### 2. Factory Pattern
```
RateLimiterFactory::createLimiter(TOKEN_BUCKET, config)
    → returns unique_ptr<IRateLimiter>

Benefit: Centralized creation logic, extensible
```

### 3. RAII (Resource Acquisition Is Initialization)
```
unique_ptr<Bucket>        → auto-deletes on scope exit
lock_guard<mutex>         → auto-unlocks on scope exit
~RateLimiterController()  → joins threads on destruction

Benefit: No memory leaks, exception-safe
```

### 4. Thread Pool Pattern
```
Controller maintains worker threads that consume tasks from queue

Benefit: Avoid thread creation overhead, bounded concurrency
```

## Extension Points

### Adding Fixed Window Algorithm

```cpp
// 1. Create FixedWindowStrategy.h
class FixedWindowStrategy : public IRateLimiter {
private:
    struct Window {
        int requestCount;
        std::chrono::time_point<std::chrono::steady_clock> windowStart;
        int maxRequests;
        std::chrono::seconds windowSize;
        std::mutex lock;
    };
    
    std::unordered_map<std::string, Window> windows;
    
public:
    bool giveAccess(const std::string& key) override {
        // Check if current window expired
        // If yes, reset count and update windowStart
        // If no, check count < maxRequests
    }
};

// 2. Register in RateLimiterFactory
factories[RateLimiterType::FIXED_WINDOW] = [](config) {
    return std::make_unique<FixedWindowStrategy>(
        config["maxRequests"], config["windowSize"]
    );
};

// 3. Use in main.cpp
RateLimiterController controller(
    RateLimiterType::FIXED_WINDOW,
    {{"maxRequests", 100}, {"windowSize", 60}}
);
```

## Performance Characteristics

| Operation | Best Case | Worst Case | Average |
|-----------|-----------|------------|---------|
| giveAccess() | O(1) | O(log n) | O(1) |
| Bucket creation | O(1) | O(log n) | O(1) |
| Refill (all buckets) | O(n) | O(n) | O(n) |
| updateConfiguration() | O(1) | O(1) | O(1) |
| shutdown() | O(t) | O(t) | O(t) |

n = number of unique users/keys
t = number of threads

## Scalability Analysis

### Vertical Scaling (Single Machine)
```
Bottleneck: CPU (mutex contention)

Solution:
- Increase thread pool size
- Shard buckets across multiple TokenBucketStrategy instances
- Use lock-free data structures (future optimization)

Estimated capacity: ~10K req/sec on modern CPU
```

### Horizontal Scaling (Distributed)
```
Challenge: Shared state across machines

Solution:
- Replace in-memory buckets with Redis
- Use Redis INCR/DECR for atomic operations
- Use Lua scripts for atomic check-and-decrement

Example Redis key structure:
  rate_limiter:user1:tokens → "5"
  rate_limiter:user1:last_refill → "1640000000"
```

## Error Handling Strategy

```cpp
// Exception safety guaranteed by:
1. RAII: unique_ptr auto-cleanup
2. lock_guard: Mutex auto-unlock even on exception
3. atomic<bool>: Safe cross-thread communication
4. No raw pointers: Prevents memory leaks
5. No throw in destructors: Safe cleanup

// Potential failure modes:
1. Thread creation fails → std::system_error thrown
   Mitigation: Catch in constructor, fail fast
   
2. Memory allocation fails → std::bad_alloc thrown
   Mitigation: Controller catches, returns error future
   
3. Mutex deadlock → Prevented by lock hierarchy
```

## Testing Strategy

```
Unit Tests:
├─ Bucket::tryConsume() correctness
├─ Bucket::refill() boundary conditions
├─ TokenBucketStrategy::giveAccess() per-user isolation
├─ RateLimiterFactory::createLimiter() all types
└─ RateLimiterController::shutdown() clean resource release

Integration Tests:
├─ Concurrent requests (same user)
├─ Concurrent requests (different users)
├─ Refill + consume simultaneously
├─ Configuration updates during operation
└─ Stress test: 1000 users, 10K requests

Load Tests:
├─ Throughput: requests/second
├─ Latency: p50, p95, p99
├─ Memory: growth over time
└─ CPU: utilization under load
```

