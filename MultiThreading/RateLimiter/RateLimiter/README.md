# Rate Limiter - Token Bucket Implementation (C++)

## Overview
Production-ready Rate Limiter system implementing the **Token Bucket algorithm** with thread safety, async processing, and extensibility for other algorithms.

## Key Features
- ✅ **Token Bucket Algorithm** - Configurable capacity & refill rate
- ✅ **Thread Safety** - Mutex-based synchronization for concurrent access
- ✅ **Per-User & Global Limiting** - Support for both modes
- ✅ **Async Processing** - Thread pool with futures
- ✅ **Extensible Design** - Factory & Strategy patterns for new algorithms
- ✅ **Auto Token Refill** - Background thread manages token replenishment

## Project Structure
```
RateLimiter/
├── IRateLimiter.h              # Interface for all rate limiting strategies
├── TokenBucketStrategy.h       # Token Bucket implementation with inner Bucket class
├── RateLimiterType.h           # Enum for different algorithm types
├── RateLimiterFactory.h        # Factory pattern for creating rate limiters
├── RateLimiterController.h     # Controller with async processing & thread pool
└── main.cpp                    # Demo scenarios
```

## Core Components

### 1. IRateLimiter (Interface)
```cpp
bool giveAccess(const std::string& rateLimitKey);
void updateConfiguration(const std::unordered_map<std::string, int>& config);
void shutdown();
```

### 2. TokenBucketStrategy
- **Inner Bucket Class**: Thread-safe token consumption/refill with mutex
- **Global Bucket**: For requests with empty key
- **Per-User Buckets**: `unordered_map<string, unique_ptr<Bucket>>`
- **Refill Thread**: Background thread adds tokens every 1 second

### 3. RateLimiterFactory
- Creates rate limiters based on `RateLimiterType`
- Supports dynamic registration of new strategies
- Static factory functions for each type

### 4. RateLimiterController
- Thread pool for concurrent request processing
- Returns `std::future<bool>` for async results
- Handles configuration updates

## Build & Run (Visual Studio)

1. Open `RateLimiter.sln` in Visual Studio
2. Build Solution (Ctrl+Shift+B)
3. Run (F5 or Ctrl+F5)

**Requirements**: Visual Studio 2022, C++17 or higher, Windows SDK 10.0

## Demo Scenarios

**Example 1**: Global burst (10 requests, capacity 5)
- First 5 allowed, rest blocked

**Example 2**: After 5s wait
- Tokens refilled, requests allowed again

**Example 3**: Per-user limiting
- Each user gets own bucket (user1, user2, user3)

**Example 4**: High concurrency
- 20 concurrent requests demonstrating thread safety

## Interview Discussion Points (1 hour)

### Phase 1: Requirements (10 min)
- Rate limiting algorithms (Token Bucket, Fixed Window, etc.)
- Thread safety requirements
- Global vs per-user limiting
- Token refill strategy

### Phase 2: Design (15 min)
- **Strategy Pattern**: `IRateLimiter` interface for extensibility
- **Factory Pattern**: `RateLimiterFactory` for creating strategies
- **Thread Safety**: Mutex for buckets, ConcurrentHashMap equivalent
- **Async Processing**: Thread pool with futures

### Phase 3: Implementation (25 min)
- Inner `Bucket` class with `tryConsume()` and `refill()`
- Background refill thread using `std::thread`
- Controller thread pool with task queue
- Per-user bucket management with `unordered_map`

### Phase 4: Testing & Extensions (10 min)
- Run demo scenarios
- Discuss: sliding window, leaky bucket, distributed systems
- Discuss: Redis-backed buckets for horizontal scaling

## Key Design Decisions

| Challenge | Solution |
|-----------|----------|
| Thread Safety | Mutex per bucket + map mutex for bucket creation |
| Token Refill | Dedicated thread with 1s interval |
| Extensibility | Strategy + Factory patterns |
| Async Processing | Custom thread pool with futures |
| Per-User Limiting | `unordered_map` with on-demand bucket creation |

## Algorithm Complexity

- **giveAccess()**: O(1) amortized (map lookup + mutex lock)
- **refill()**: O(n) where n = number of active buckets
- **Memory**: O(u) where u = number of unique users

## Future Extensions

1. **Fixed Window Counter**: Track requests per time window
2. **Sliding Window Log**: Precise timestamp-based limiting
3. **Leaky Bucket**: Constant output rate
4. **Distributed**: Redis/Memcached for shared state

## References
- [Code with Aryan - Rate Limiter](https://codewitharyan.com/tech-blogs/design-rate-limiter)
- Token Bucket Algorithm (Wikipedia)
- System Design: Rate Limiting patterns

