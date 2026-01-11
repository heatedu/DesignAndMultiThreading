# 🚀 START HERE - Rate Limiter Project

## What is This?

A **production-ready Rate Limiter** system in C++ implementing the **Token Bucket algorithm**. Perfect for 1-hour SDE2 interview discussions covering:
- ✅ Low-level system design
- ✅ Concurrency & thread safety
- ✅ Design patterns (Strategy, Factory)
- ✅ Async processing with thread pools

## Quick Start (2 minutes)

1. **Open the project**
   ```
   Double-click: D:\DesignPatterns\MultiThreading\RateLimiter\RateLimiter.sln
   ```

2. **Build & Run**
   - Press **F5** (with debugging) or **Ctrl+F5** (without debugging)
   - OR Press **Ctrl+Shift+B** to build, then run from `x64\Debug\RateLimiter.exe`

3. **See the magic**
   - Console shows 4 demo scenarios
   - Watch token bucket rate limiting in action!

## Project Files (What to Focus On)

### 🎯 Core Implementation (Interview Focus)
| File | Purpose | Key Concepts |
|------|---------|-------------|
| `IRateLimiter.h` | Interface | Strategy pattern |
| `TokenBucketStrategy.h` | Token Bucket algorithm | Thread safety, inner Bucket class |
| `RateLimiterFactory.h` | Creates rate limiters | Factory pattern |
| `RateLimiterController.h` | Request processor | Thread pool, async processing |
| `main.cpp` | Demo scenarios | Usage examples |

### 📚 Documentation (Interview Prep)
| File | Read This For |
|------|---------------|
| `README.md` | Overview, features, quick reference |
| `INTERVIEW_GUIDE.md` | ⭐ **READ THIS FIRST!** Interview flow, talking points |
| `ARCHITECTURE.md` | Deep dive: diagrams, memory layout, concurrency |
| `BUILD_INSTRUCTIONS.md` | Build steps, troubleshooting |

## Interview Preparation (30 minutes)

### Step 1: Understand the Problem (5 min)
Read the problem statement in `INTERVIEW_GUIDE.md` section "Problem Statement"

**Key Question**: What is a rate limiter?
**Answer**: System that controls request flow by limiting requests per time period.

### Step 2: Review the Design (10 min)
Read `ARCHITECTURE.md` - System Overview section

**Key Components**:
1. **IRateLimiter** - Strategy interface
2. **TokenBucketStrategy** - Algorithm implementation with inner Bucket class
3. **RateLimiterFactory** - Creates strategies
4. **RateLimiterController** - Processes requests with thread pool

### Step 3: Understand Thread Safety (10 min)
Read `INTERVIEW_GUIDE.md` section "Thread Safety Strategy"

**Key Mechanisms**:
- **Per-bucket mutex**: Each bucket locks independently
- **Map mutex**: Protects user bucket creation
- **Thread pool**: Task queue with condition variables

### Step 4: Run & Understand (5 min)
Run the program, analyze output:
- Example 1: Burst exhausts tokens (5 allowed, 5 blocked)
- Example 2: After 5s, tokens refilled
- Example 3: Each user gets own bucket
- Example 4: Thread pool handles 20 concurrent requests

## Interview Flow (1 hour breakdown)

### Minutes 0-10: Requirements Gathering
**Interviewer**: "Design a rate limiter."

**You**: Ask clarifying questions:
- Which algorithm? (Token Bucket, Fixed Window, etc.)
- Thread safety requirements?
- Global or per-user limiting?
- How to handle token refill?

**Deliverable**: Agreed-upon requirements (see `INTERVIEW_GUIDE.md`)

### Minutes 10-25: High-Level Design
Draw these components on whiteboard:
```
Client → Controller → RateLimiter (Strategy)
                         ↓
                    TokenBucket
                      ↓
           [Global Bucket | User Buckets]
```

**Discuss**:
- Strategy pattern for extensibility
- Factory pattern for creation
- Thread pool for concurrency
- Mutex strategy for thread safety

### Minutes 25-50: Implementation Details
Walk through code:

**1. Bucket class (5 min)**
```cpp
class Bucket {
    int tokens;
    std::mutex lock;
    bool tryConsume();    // Lock → check → decrement → unlock
    void refill();        // Lock → add tokens → unlock
};
```

**2. TokenBucketStrategy (10 min)**
- Global vs per-user buckets
- Refill thread every 1 second
- Map mutex for bucket creation

**3. Controller (5 min)**
- Thread pool with task queue
- Returns `std::future<bool>`
- Handles async processing

**4. Factory (3 min)**
- Static map of factory functions
- Easy to add new strategies

**5. Thread Safety (7 min)**
- Lock hierarchy (map mutex → bucket mutex)
- No deadlocks
- Concurrent access patterns

### Minutes 50-60: Extensions & Trade-offs
**Discuss**:
- Fixed Window vs Sliding Window algorithms
- Distributed rate limiting (Redis)
- Memory vs accuracy trade-offs
- Horizontal scaling strategies

## Key Talking Points (Memorize These!)

### 1. Why Token Bucket?
"Token Bucket allows controlled bursts while maintaining average rate. Tokens accumulate up to capacity, allowing flexibility for legitimate burst traffic."

### 2. Thread Safety Approach
"I use fine-grained locking - each bucket has its own mutex, so different users don't block each other. The map mutex only protects bucket creation, not token consumption."

### 3. Extensibility
"The Strategy pattern makes it trivial to add new algorithms. Just implement IRateLimiter and register in the factory. The controller code doesn't change."

### 4. Async Processing
"The thread pool avoids thread creation overhead. Requests return futures immediately, and workers process them concurrently. This allows non-blocking operation."

### 5. Refill Strategy
"Background thread refills every second. This is simpler than on-demand refill (calculating elapsed time per request). Trade-off: slight delay in token availability."

## Code Walkthrough (What to Show)

### Demo 1: Basic Token Bucket
Show `TokenBucketStrategy::giveAccess()`:
```cpp
bool giveAccess(const std::string& rateLimitKey) {
    if (!rateLimitKey.empty()) {
        std::lock_guard<std::mutex> guard(mapMutex);
        auto it = userBuckets.find(rateLimitKey);
        if (it == userBuckets.end()) {
            userBuckets[rateLimitKey] = std::make_unique<Bucket>(bucketCapacity);
            return userBuckets[rateLimitKey]->tryConsume();
        }
        return it->second->tryConsume();
    } else {
        return globalBucket->tryConsume();
    }
}
```

**Explain**: Map lookup → bucket creation if needed → delegate to bucket

### Demo 2: Thread-Safe Consumption
Show `Bucket::tryConsume()`:
```cpp
bool tryConsume() {
    std::lock_guard<std::mutex> guard(lock);
    if (tokens > 0) {
        tokens--;
        return true;
    }
    return false;
}
```

**Explain**: Lock → check → modify → unlock. RAII ensures unlock even on exception.

### Demo 3: Async Processing
Show `RateLimiterController::processRequest()`:
```cpp
std::future<bool> processRequest(const std::string& rateLimitKey) {
    auto promise = std::make_shared<std::promise<bool>>();
    auto future = promise->get_future();
    
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        tasks.push([this, rateLimitKey, promise]() {
            bool allowed = rateLimiter->giveAccess(rateLimitKey);
            // ... print result ...
            promise->set_value(allowed);
        });
    }
    condition.notify_one();
    return future;
}
```

**Explain**: Create promise/future pair → enqueue task → notify worker → return future

## Common Interview Questions & Answers

### Q: "What if we have millions of users?"
**A**: "Current in-memory approach won't scale. I'd use Redis with TTL-based expiration for inactive users. Or implement LRU eviction in the map with a max size limit."

### Q: "How to handle distributed systems?"
**A**: "Replace in-memory buckets with Redis. Use Lua scripts for atomic operations:
```lua
local tokens = redis.call('GET', key)
if tokens > 0 then
    redis.call('DECR', key)
    return 1
else
    return 0
end
```
Multiple app servers can share the same Redis instance."

### Q: "What about Fixed Window algorithm?"
**A**: "Track request count per time window. Reset count when window expires. Simpler than Token Bucket but allows burst at window boundaries (double the rate for brief moment)."

### Q: "Why not lock-free data structures?"
**A**: "I prioritized clarity for interview setting. In production, I'd consider lock-free queues (Boost.Lockfree) or atomics for the bucket counter if profiling showed mutex contention."

### Q: "How to test thread safety?"
**A**: "I'd use thread sanitizers (TSan), run stress tests with many threads, and use tools like Helgrind (Valgrind) to detect race conditions. The demo includes high-concurrency scenario."

## Customization (Try These!)

### Change Rate Limit
Edit `main.cpp`:
```cpp
config["capacity"] = 10;      // Was: 5
config["refreshRate"] = 2;    // Was: 1
```

### Add More Users
Edit `main.cpp`:
```cpp
std::vector<std::string> users = {"user1", "user2", ..., "user100"};
```

### Adjust Thread Pool
Edit `main.cpp`:
```cpp
RateLimiterController controller(
    RateLimiterType::TOKEN_BUCKET, 
    config, 
    20  // Was: 10 threads
);
```

## Troubleshooting

### Issue: "Cannot build in Visual Studio"
- Ensure Visual Studio 2022 installed
- Check Platform Toolset: v143 (VS 2022)
- Try: Build → Rebuild Solution

### Issue: "Headers not found"
- Check all .h files are in `RateLimiter/` directory
- Verify `.vcxproj` includes all headers

### Issue: "Program crashes"
- Run with debugger (F5) to see stack trace
- Check: Did you modify shutdown logic?
- Ensure: `running = false` before joining threads

## Next Steps

1. ✅ **Run the program** - See it work!
2. ✅ **Read INTERVIEW_GUIDE.md** - Preparation material
3. ✅ **Review ARCHITECTURE.md** - Deep technical details
4. ✅ **Modify main.cpp** - Experiment with different configs
5. ✅ **Practice explaining** - Record yourself describing the design

## Resources

- [Reference Article](https://codewitharyan.com/tech-blogs/design-rate-limiter)
- Token Bucket Algorithm: Wikipedia
- C++ Concurrency in Action (Anthony Williams)
- Designing Data-Intensive Applications (Martin Kleppmann)

## Contact & Support

For questions about this implementation:
1. Review the documentation files (especially `INTERVIEW_GUIDE.md`)
2. Check `ARCHITECTURE.md` for deep dives
3. Examine inline code comments in headers

---

## 🎯 TL;DR - Quick Reference Card

**Build**: Open `RateLimiter.sln`, press **F5**

**Core Algorithm**: Token Bucket (capacity + refill rate)

**Thread Safety**: Mutex per bucket + map mutex

**Design Patterns**: Strategy (IRateLimiter) + Factory (creation)

**Async**: Thread pool with futures

**Key Interview Topics**:
- Inner Bucket class with tryConsume/refill
- Lock hierarchy (map → bucket)
- Refill thread (background, 1s interval)
- Extensibility (easy to add Fixed Window, etc.)
- Scaling (Redis for distributed)

**Time Complexity**: O(1) for giveAccess, O(n) for refill

**Memory**: ~40 bytes per user bucket

**Read First**: `INTERVIEW_GUIDE.md` → `ARCHITECTURE.md` → Code

Good luck with your interview! 🚀

