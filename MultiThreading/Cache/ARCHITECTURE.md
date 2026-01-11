# Cache System - Architecture Documentation

Detailed technical architecture of the C++ cache system implementation.

---

## 🎯 System Overview

This is a **production-ready, thread-safe cache system** implementing:
- Key-value storage with limited capacity
- LRU (Least Recently Used) eviction
- Write-through persistence policy
- Thread-safe concurrent operations
- "Read your own writes" consistency
- Strategy pattern for extensibility

**Target Audience**: SDE2-level engineers  
**Language**: C++17  
**Concurrency Model**: Key-based thread affinity  
**Design Patterns**: Strategy, Template Method, Dependency Injection, RAII

---

## 🏗️ Layered Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    Client Application                    │
│                  (main.cpp, test code)                   │
└────────────────────────┬────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────┐
│                   Core Cache Layer                       │
│            Cache<K,V> (cache orchestrator)               │
│  • accessData(key) → future<V>                           │
│  • updateData(key, value) → future<void>                 │
└───┬─────────┬─────────┬──────────┬──────────────────────┘
    │         │         │          │
    ▼         ▼         ▼          ▼
┌─────────┐ ┌─────┐ ┌────────┐ ┌──────────────┐
│ Cache   │ │ DB  │ │ Write  │ │  Eviction    │
│ Storage │ │Store│ │ Policy │ │  Algorithm   │
│Interface│ │Iface│ │Interface│ │  Interface   │
└────┬────┘ └──┬──┘ └───┬────┘ └──────┬───────┘
     │         │        │              │
     ▼         ▼        ▼              ▼
┌────────────────────────────────────────────┐
│         Concrete Implementations            │
│  • InMemoryCacheStorage (ConcurrentHashMap)│
│  • SimpleDBStorage (Mock DB)               │
│  • WriteThroughPolicy (Async writes)       │
│  • LRUEvictionAlgorithm (DLL + HashMap)    │
└────────────────────────────────────────────┘
                    │
                    ▼
┌────────────────────────────────────────────┐
│          Infrastructure Layer               │
│  • KeyBasedExecutor (Thread pool)          │
│  • DoublyLinkedList (LRU data structure)   │
│  • std::future (Async operations)          │
│  • std::mutex (Synchronization)            │
└────────────────────────────────────────────┘
```

---

## 📦 Component Details

### 1. Core Cache Class

**File**: `Cache.h`

```cpp
template<typename K, typename V>
class Cache {
private:
    CacheStorage<K, V>* cacheStorage;
    DBStorage<K, V>* dbStorage;
    WritePolicy<K, V>* writePolicy;
    EvictionAlgorithm<K>* evictionAlgorithm;
    KeyBasedExecutor keyBasedExecutor;

public:
    Cache(...);  // Constructor with dependency injection
    
    // Async read operation
    std::future<V> accessData(const K& key);
    
    // Async write operation
    std::future<void> updateData(const K& key, const V& value);
    
    void shutdown();
};
```

**Responsibilities**:
- Orchestrates all cache operations
- Routes operations to correct thread via KeyBasedExecutor
- Coordinates cache, DB, write policy, and eviction algorithm
- Handles eviction when cache is full
- Manages cross-thread eviction scenarios

**Key Design Decisions**:
1. **Dependency Injection**: Components passed to constructor (testability, flexibility)
2. **Futures**: All operations return `std::future<T>` for async execution
3. **Template specialization**: Special handling for string keys (better error messages)

---

### 2. Storage Layer

#### 2.1 CacheStorage Interface

**File**: `CacheStorage.h`

```cpp
template<typename K, typename V>
class CacheStorage {
public:
    virtual void put(const K& key, const V& value) = 0;
    virtual V get(const K& key) = 0;
    virtual void remove(const K& key) = 0;
    virtual bool containsKey(const K& key) = 0;
    virtual int size() const = 0;
    virtual int getCapacity() const = 0;
};
```

**Design Rationale**:
- Pure interface for in-memory cache operations
- Enables mocking for tests
- Allows different storage backends (Redis, Memcached)

#### 2.2 InMemoryCacheStorage Implementation

**File**: `InMemoryCacheStorage.h`

```cpp
template<typename K, typename V>
class InMemoryCacheStorage : public CacheStorage<K, V> {
private:
    std::unordered_map<K, V> cache;  // Fast O(1) lookup
    int capacity;
    mutable std::mutex mutex;  // Thread safety
};
```

**Thread Safety Strategy**:
- Per-operation locking (coarse-grained)
- Mutex protects all map operations
- Note: Fine-grained locking handled at KeyBasedExecutor level

#### 2.3 DBStorage Interface & Implementation

**Files**: `DBStorage.h`, `SimpleDBStorage.h`

Represents persistent storage (database, disk, etc.).

**Current Implementation**: Mock in-memory storage (for demo)  
**Production**: Replace with actual DB client (SQL, NoSQL, etc.)

---

### 3. Strategy Layer

#### 3.1 WritePolicy Interface

**File**: `WritePolicy.h`

```cpp
template<typename K, typename V>
class WritePolicy {
public:
    virtual void write(const K& key, const V& value,
                      CacheStorage<K,V>* cache,
                      DBStorage<K,V>* db) = 0;
};
```

**Strategy Pattern Benefits**:
- Runtime policy selection
- Easy to add new policies (Write-Back, Write-Around)
- Testable in isolation

#### 3.2 WriteThroughPolicy Implementation

**File**: `WriteThroughPolicy.h`

```cpp
void write(const K& key, const V& value, ...) {
    auto cacheFuture = std::async(std::launch::async, [&]() {
        cacheStorage->put(key, value);
    });
    
    auto dbFuture = std::async(std::launch::async, [&]() {
        dbStorage->write(key, value);
    });
    
    cacheFuture.get();  // Wait for both
    dbFuture.get();
}
```

**Design Highlights**:
- **Parallel writes**: Cache and DB updated concurrently
- **Strong consistency**: Both must succeed
- **Latency**: `max(cache_time, db_time)` instead of `cache_time + db_time`

**Trade-offs**:
- ✅ Strong consistency
- ✅ No data loss on cache eviction
- ❌ Higher write latency (blocked by DB)
- ❌ DB becomes write bottleneck

---

#### 3.3 EvictionAlgorithm Interface

**File**: `EvictionAlgorithm.h`

```cpp
template<typename K>
class EvictionAlgorithm {
public:
    virtual void keyAccessed(const K& key) = 0;
    virtual std::optional<K> evictKey() = 0;
};
```

#### 3.4 LRUEvictionAlgorithm Implementation

**File**: `LRUEvictionAlgorithm.h`

```cpp
template<typename K>
class LRUEvictionAlgorithm : public EvictionAlgorithm<K> {
private:
    DoublyLinkedList<K> dll;  // Head=LRU, Tail=MRU
    std::unordered_map<K, DoublyLinkedListNode<K>*> keyToNodeMap;
    mutable std::mutex mutex;
};
```

**Data Structure Choice**:

| Structure | Access | Insert | Delete | Move to End | Space |
|-----------|--------|--------|--------|-------------|-------|
| Array | O(1) | O(n) | O(n) | O(n) | O(n) |
| Single LL | O(n) | O(1) | O(n)* | O(n) | O(n) |
| **Doubly LL** | **O(1)** | **O(1)** | **O(1)** | **O(1)** | **O(n)** |

*Needs to find previous node

**Algorithm**:

```
keyAccessed(key):
  if key exists:
    1. Find node in map: O(1)
    2. Detach from current position: O(1)
       node.prev.next = node.next
       node.next.prev = node.prev
    3. Add to tail (MRU): O(1)
       tail.next = node
       node.prev = tail
       tail = node
  else:
    1. Create new node
    2. Add to map
    3. Add to tail

evictKey():
  1. Get head node (LRU): O(1)
  2. Remove from map: O(1)
  3. Remove head from list: O(1)
     head = head.next
     head.prev = null
  4. Return evicted key
```

**Complexity**: All operations O(1) amortized

---

### 4. Infrastructure Layer

#### 4.1 KeyBasedExecutor

**File**: `KeyBasedExecutor.h`

**Purpose**: Ensures all operations for the same key execute on the same thread.

```cpp
class KeyBasedExecutor {
private:
    struct ExecutorThread {
        std::thread thread;
        std::queue<Task> taskQueue;
        std::mutex mutex;
        std::condition_variable cv;
        std::atomic<bool> shouldStop;
        
        void run() {
            while (!shouldStop) {
                Task task = dequeue();  // Blocks if empty
                task.execute();
            }
        }
    };
    
    std::vector<std::unique_ptr<ExecutorThread>> executors;
    
public:
    template<typename K, typename Func>
    auto submitTask(const K& key, Func&& func) -> std::future<...> {
        int index = hash(key) % numExecutors;
        return executors[index]->enqueue(func);
    }
};
```

**Thread Affinity Algorithm**:

```
Key "A" → hash("A") = 12345 → 12345 % 4 = 1 → Thread 1
Key "B" → hash("B") = 67890 → 67890 % 4 = 2 → Thread 2
Key "A" → hash("A") = 12345 → 12345 % 4 = 1 → Thread 1 (same!)

Operations on "A":
  Thread 1: [Op1_A, Op2_A, Op3_A] (sequential execution)
  
Operations on different keys:
  Thread 1: [Op_A1, Op_A2]  (sequential)
  Thread 2: [Op_B1, Op_B2]  (parallel with Thread 1)
```

**Guarantees**:
1. **Same-key serialization**: All ops on key K run sequentially on same thread
2. **Ordering**: Operations on same key execute in submission order
3. **"Read your own writes"**: Write followed by read both on same thread
4. **Parallelism**: Different keys can execute concurrently

**Trade-offs**:
- ✅ No race conditions for same key
- ✅ Automatic ordering guarantees
- ✅ Better than global lock (parallel different-key ops)
- ❌ Hot keys bottleneck single thread
- ❌ Uneven load distribution (hash collisions)

---

#### 4.2 DoublyLinkedList

**File**: `DoublyLinkedList.h`

Custom doubly linked list implementation for LRU tracking.

```cpp
template<typename K>
class DoublyLinkedListNode {
public:
    K value;
    DoublyLinkedListNode* prev;
    DoublyLinkedListNode* next;
};

template<typename K>
class DoublyLinkedList {
    DoublyLinkedListNode<K>* head;  // LRU end
    DoublyLinkedListNode<K>* tail;  // MRU end
    
public:
    void addNodeAtTail(DoublyLinkedListNode<K>* node);
    void detachNode(DoublyLinkedListNode<K>* node);
    DoublyLinkedListNode<K>* getHead() const;
    void removeHead();
};
```

**Why Custom Implementation?**:
- `std::list` doesn't expose node pointers
- Need O(1) detach of arbitrary node
- HashMap must store node pointers

---

## 🔄 Operation Flows

### Read Operation: accessData(key)

```
1. Client calls: cache.accessData("key1")
   └→ Returns: std::future<V>

2. Cache submits to KeyBasedExecutor:
   └→ hash("key1") % 4 = Thread 2

3. Thread 2 executes:
   ├─ Check if key in cache
   │  └→ If not: throw exception
   ├─ Update eviction algorithm
   │  └→ evictionAlg.keyAccessed("key1")
   │     └→ Move "key1" to tail (MRU)
   └─ Return value from cache

4. Future resolves with value
```

### Write Operation: updateData(key, value)

```
1. Client calls: cache.updateData("key1", "value1")
   └→ Returns: std::future<void>

2. Cache submits to KeyBasedExecutor:
   └→ hash("key1") % 4 = Thread 2

3. Thread 2 executes:
   ├─ Check if key exists in cache
   │
   ├─ If key exists (UPDATE case):
   │  ├─ Call writePolicy.write(key, value)
   │  │  ├─ Async write to cache
   │  │  └─ Async write to DB
   │  │     └─ Wait for both
   │  └─ Update eviction algorithm
   │
   └─ If key new (INSERT case):
      ├─ Check if cache full
      │  └─ If full:
      │     ├─ evictionAlg.evictKey() → returns "key2"
      │     ├─ Check if "key2" on same thread
      │     │  ├─ Same thread: remove directly
      │     │  └─ Different thread:
      │     │     └─ Submit removal to correct thread & wait
      │     └─ Remove "key2" from cache
      │
      ├─ Call writePolicy.write(key, value)
      └─ Update eviction algorithm

4. Future resolves (write complete)
```

### Cross-Thread Eviction Scenario

```
Scenario: Thread 2 needs to evict key on Thread 1

Thread 2 (inserting "F"):
  1. Cache full, need eviction
  2. evictionAlg.evictKey() → returns "A"
  3. hash("F") % 4 = 2 (current thread)
  4. hash("A") % 4 = 1 (different thread!)
  5. Submit removal task to Thread 1:
     executor.submitTask("A", [](){ cache.remove("A"); })
  6. Wait for future.get()
  7. Continue with write

Thread 1:
  1. Receives removal task for "A"
  2. Executes: cache.remove("A")
  3. Resolves future

Why this design?
  → Maintains thread affinity for "A"
  → Prevents race conditions on "A"
  → All "A" operations still on Thread 1
```

---

## 🧵 Concurrency Model

### Thread Safety Guarantees

| Scenario | Guarantee | Mechanism |
|----------|-----------|-----------|
| Same key, same thread | Sequential execution | KeyBasedExecutor |
| Same key, multiple threads | Routed to same thread | Hash-based routing |
| Different keys, different threads | Parallel execution | Separate executor threads |
| Read-your-own-writes | Guaranteed for same key | Thread affinity |
| Cache eviction | Thread-safe | Cross-thread future wait |

### Locking Hierarchy

```
Level 1: KeyBasedExecutor (no lock, queue per thread)
         └─ Each ExecutorThread has its own queue & mutex

Level 2: Cache (no lock, delegates to executor)

Level 3: CacheStorage (mutex per operation)
         ├─ InMemoryCacheStorage: std::mutex
         └─ SimpleDBStorage: std::mutex

Level 4: EvictionAlgorithm (mutex per operation)
         └─ LRUEvictionAlgorithm: std::mutex
```

**No Deadlocks**: Single lock per operation, no nested locking.

---

## 🎨 Design Patterns Applied

### 1. Strategy Pattern

**Intent**: Define a family of algorithms, encapsulate each one, make them interchangeable.

**Application**:
```cpp
// Write policies
WritePolicy<K,V> interface
├─ WriteThroughPolicy
├─ WriteBackPolicy (future)
└─ WriteAroundPolicy (future)

// Eviction algorithms
EvictionAlgorithm<K> interface
├─ LRUEvictionAlgorithm
├─ LFUEvictionAlgorithm (future)
└─ FIFOEvictionAlgorithm (future)
```

**Benefits**:
- Runtime policy selection
- Easy to add new strategies
- Testable in isolation
- Open/Closed Principle (open for extension, closed for modification)

---

### 2. Template Method Pattern

**Intent**: Define skeleton of algorithm, let subclasses override specific steps.

**Application**:
```cpp
// Interface defines contract
class CacheStorage<K,V> {
public:
    virtual void put(...) = 0;  // Subclass implements
    virtual V get(...) = 0;     // Subclass implements
};

// Concrete implementation
class InMemoryCacheStorage : public CacheStorage<K,V> {
    void put(...) override { /* specific implementation */ }
};
```

---

### 3. Dependency Injection

**Intent**: Provide dependencies from outside rather than creating internally.

**Application**:
```cpp
Cache(CacheStorage<K,V>* cache,
      DBStorage<K,V>* db,
      WritePolicy<K,V>* policy,
      EvictionAlgorithm<K>* eviction,
      int numExecutors)  // All dependencies injected
```

**Benefits**:
- Testability (inject mocks)
- Flexibility (swap implementations)
- Loose coupling

---

### 4. RAII (Resource Acquisition Is Initialization)

**Intent**: Tie resource lifetime to object lifetime.

**Application**:
```cpp
class KeyBasedExecutor {
    ~KeyBasedExecutor() {
        shutdown();  // Automatic cleanup
    }
    
    void shutdown() {
        for (auto& executor : executors) {
            executor->stop();
            executor->thread.join();  // Wait for thread
        }
    }
};
```

---

## 📊 Performance Characteristics

### Time Complexity

| Operation | Best | Average | Worst |
|-----------|------|---------|-------|
| `accessData(key)` | O(1) | O(1) | O(1) |
| `updateData(key, value)` (existing) | O(1) | O(1) | O(1) |
| `updateData(key, value)` (new) | O(1) | O(1) | O(n)* |
| LRU `keyAccessed()` | O(1) | O(1) | O(1) |
| LRU `evictKey()` | O(1) | O(1) | O(1) |

*Cross-thread eviction requires future wait (rare, amortized O(1))

### Space Complexity

- Cache: O(capacity)
- LRU tracking: O(capacity)
- Executor queues: O(pending_tasks)
- **Total**: O(capacity + pending_tasks)

### Throughput Estimates

```
Configuration:
- Cache size: 1000
- Threads: 4
- Key distribution: Uniform

Expected:
- Read throughput: 100K-500K ops/sec
- Write throughput: 50K-200K ops/sec (DB limited)
- Mixed (80% read): 80K-400K ops/sec

Bottlenecks:
- Hot keys: Single thread saturates
- Write-through: DB latency
- Eviction: LRU lock contention
```

---

## 🔮 Extension Points

### 1. New Eviction Algorithms

```cpp
class LFUEvictionAlgorithm : public EvictionAlgorithm<K> {
    unordered_map<K, int> frequency;
    map<int, set<K>> freqBuckets;  // freq → keys
    
    void keyAccessed(const K& key) override {
        // Move key to higher frequency bucket
    }
    
    optional<K> evictKey() override {
        // Return key from lowest frequency bucket
    }
};
```

### 2. New Write Policies

```cpp
class WriteBackPolicy : public WritePolicy<K,V> {
    queue<pair<K,V>> dirtyQueue;
    
    void write(...) override {
        cache->put(key, value);  // Fast cache write
        dirtyQueue.push({key, value});  // Queue for batching
        
        if (dirtyQueue.size() >= BATCH_SIZE) {
            flushBatch();  // Batch write to DB
        }
    }
};
```

### 3. TTL Support

```cpp
struct CacheEntry<V> {
    V value;
    chrono::time_point<chrono::system_clock> expiry;
};

// In accessData():
if (chrono::system_clock::now() > entry.expiry) {
    remove(key);
    throw runtime_error("Key expired");
}
```

### 4. Metrics & Monitoring

```cpp
class CacheMetrics {
    atomic<uint64_t> hits{0};
    atomic<uint64_t> misses{0};
    atomic<uint64_t> evictions{0};
    
    double hitRatio() { return hits / (hits + misses); }
};
```

---

## 🏭 Production Considerations

### 1. Error Handling
```cpp
// Current: Exceptions
// Production: Result<T, Error> type

enum class CacheError {
    KEY_NOT_FOUND, DB_UNAVAILABLE, TIMEOUT, CAPACITY_EXCEEDED
};

template<typename T, typename E>
class Result { ... };

Result<V, CacheError> accessData(const K& key);
```

### 2. Resource Limits
- Max memory usage tracking
- Connection pooling for DB
- Queue size limits per thread

### 3. Observability
- Structured logging
- Distributed tracing
- Metrics export (Prometheus, etc.)

### 4. Graceful Shutdown
- Drain executor queues
- Flush dirty entries (write-back)
- Save cache state (optional)

---

## 📚 Key Takeaways

1. **Strategy Pattern** enables runtime policy selection
2. **Key-based thread affinity** balances safety and performance
3. **Doubly linked list + HashMap** achieves O(1) LRU
4. **std::future** enables async operations without blocking
5. **Dependency injection** improves testability and flexibility
6. **Trade-offs**: Consistency vs latency, simplicity vs features

---

**Document Version**: 1.0  
**Last Updated**: 2025-01-27  
**Complexity Level**: SDE2

