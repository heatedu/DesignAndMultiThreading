# Cache System - Interview Discussion Guide

A structured 1-hour interview guide for discussing this cache implementation with an SDE2.

---

## ⏱️ Time Allocation (60 minutes)

| Phase | Duration | Focus |
|-------|----------|-------|
| Introduction & Problem Statement | 5 min | Understanding requirements |
| Architecture Overview | 10 min | High-level design discussion |
| Deep Dive: Thread Safety | 12 min | Concurrency patterns |
| Deep Dive: LRU Algorithm | 10 min | Data structures & complexity |
| Deep Dive: Write Policies | 8 min | Trade-offs & strategies |
| Extensions & Trade-offs | 10 min | Production concerns |
| Q&A | 5 min | Open discussion |

---

## 📋 Phase 1: Introduction & Problem Statement (5 min)

### Opening Question
**Interviewer**: "Walk me through your understanding of this cache system."

### Key Points to Cover
```cpp
// Core Requirements:
✓ Key-value cache with limited capacity
✓ LRU eviction when full
✓ Write-through policy to persistent storage
✓ Thread-safe operations
✓ "Read your own writes" consistency
✓ Extensible design
```

### Candidate Should Mention:
1. **Cache capacity**: Fixed size, needs eviction
2. **Persistence**: Cache + DB storage layers
3. **Concurrency**: Multiple threads accessing simultaneously
4. **Consistency**: Strong consistency for same-key operations

---

## 🏗️ Phase 2: Architecture Overview (10 min)

### Diagram Discussion

```
┌─────────────────────────────────────────┐
│           Client Code                    │
└──────────────┬──────────────────────────┘
               │
               ▼
┌─────────────────────────────────────────┐
│         Cache<K,V>                       │
│  - accessData(key) → Future<V>          │
│  - updateData(key, value) → Future<void>│
└──┬────────┬────────┬──────────┬─────────┘
   │        │        │          │
   ▼        ▼        ▼          ▼
┌─────┐ ┌──────┐ ┌──────┐ ┌────────────┐
│Cache│ │  DB  │ │Write │ │  Eviction  │
│Stor │ │ Stor │ │Policy│ │  Algorithm │
└─────┘ └──────┘ └──────┘ └────────────┘
   │                          │
   ▼                          ▼
┌─────────────┐    ┌──────────────────┐
│ConcurrentMap│    │DoublyLinkedList  │
│<K,V>        │    │  + HashMap<K,Node>│
└─────────────┘    └──────────────────┘
```

### Questions to Ask Candidate

**Q1**: "Why separate CacheStorage and DBStorage interfaces?"

**Expected Answer**:
- **Separation of concerns**: Cache vs persistent storage
- **Testing**: Mock implementations
- **Flexibility**: Swap storage backends (Redis, Memcached, etc.)
- **Single Responsibility Principle**

**Q2**: "Explain the Strategy Pattern usage here."

**Expected Answer**:
```cpp
// Strategy Pattern allows runtime policy selection:

// Different Write Policies
WritePolicy<K,V>
├── WriteThroughPolicy    // Immediate DB write
├── WriteBackPolicy       // Delayed batch writes
└── WriteAroundPolicy     // Skip cache, write to DB

// Different Eviction Algorithms
EvictionAlgorithm<K>
├── LRUEvictionAlgorithm  // Least Recently Used
├── LFUEvictionAlgorithm  // Least Frequently Used
└── FIFOEvictionAlgorithm // First In First Out
```

### Code Walkthrough
```cpp
// Dependency Injection enables Strategy Pattern
Cache<string, string> cache(
    cacheStorage,      // What storage?
    dbStorage,         // What database?
    writePolicy,       // How to write?
    evictionAlg,       // How to evict?
    numExecutors       // How many threads?
);
```

---

## 🔒 Phase 3: Deep Dive - Thread Safety (12 min)

### The Core Challenge
**Q**: "How do you ensure 'read your own writes' consistency?"

### Key Design: KeyBasedExecutor

```cpp
class KeyBasedExecutor {
    ExecutorService[] executors;  // Fixed pool of threads
    
    template<typename K>
    int getExecutorIndexForKey(const K& key) {
        return hash(key) % numExecutors;
    }
    
    template<typename K, typename Func>
    auto submitTask(const K& key, Func&& func) {
        int index = getExecutorIndexForKey(key);
        return executors[index].submit(func);
    }
};
```

### Discussion Points

**1. Thread Affinity Pattern**
```cpp
// Same key always goes to same thread
hash("A") % 4 = 2  → Thread 2
hash("B") % 4 = 1  → Thread 1
hash("A") % 4 = 2  → Thread 2 (again!)

// Guarantees:
✓ Operations on "A" are serialized
✓ No race conditions for same key
✓ "Read your own writes" automatically satisfied
```

**2. Why Not Global Lock?**
```cpp
// ❌ Bad: Global lock
mutex globalLock;
void put(K key, V value) {
    lock_guard<mutex> lock(globalLock);  // Serializes EVERYTHING
    cache[key] = value;
}

// ✅ Good: Key-based locks
void put(K key, V value) {
    int threadId = hash(key) % numThreads;
    executors[threadId].submit([=]() {
        cache[key] = value;  // Only this key blocked
    });
}
```

**3. Cross-Thread Eviction Challenge**

```cpp
// Problem: Key "A" on Thread 2, Key "B" on Thread 1
// Thread 2 needs to evict "B" from cache

// Solution in updateData():
if (currentIndex == evictedIndex) {
    // Same thread, remove directly
    cacheStorage->remove(evictedKey);
} else {
    // Different thread, submit and wait
    auto removalFuture = keyBasedExecutor.submitTask(
        evictedKey, [...]() { 
            cacheStorage->remove(evictedKey); 
        }
    );
    removalFuture.get();  // Wait for completion
}
```

**Q**: "What's the trade-off of key-based affinity?"

**Expected Answer**:
- ✅ **Pros**: No global lock, parallel ops on different keys, ordering guaranteed
- ❌ **Cons**: Hot keys bottleneck one thread, uneven load distribution, limited parallelism

---

## 📊 Phase 4: Deep Dive - LRU Algorithm (10 min)

### Data Structure Design

```cpp
class LRUEvictionAlgorithm<K> {
    DoublyLinkedList<K> dll;              // Head = LRU, Tail = MRU
    unordered_map<K, DLLNode<K>*> nodeMap;  // O(1) lookup
    mutex mutex;                           // Thread safety
};
```

### Visual Representation

```
Initial State:
dll: [A] ⇄ [B] ⇄ [C]
     LRU           MRU
     
Access key "A":
dll: [B] ⇄ [C] ⇄ [A]
     LRU           MRU
     
Evict:
dll: [C] ⇄ [A]
     LRU    MRU
(B removed)
```

### Implementation Details

**Q**: "Walk me through `keyAccessed()` implementation."

```cpp
void keyAccessed(const K& key) {
    lock_guard<mutex> lock(mutex);
    
    if (nodeMap.contains(key)) {
        // Existing key: move to tail (MRU)
        DLLNode<K>* node = nodeMap[key];
        dll.detachNode(node);      // Remove from current position
        dll.addNodeAtTail(node);   // Add to tail (MRU)
    } else {
        // New key: add to tail
        DLLNode<K>* newNode = new DLLNode<K>(key);
        dll.addNodeAtTail(newNode);
        nodeMap[key] = newNode;
    }
}
```

**Q**: "Why doubly linked list instead of array or single linked list?"

**Expected Answer**:
| Operation | Array | Single LL | Doubly LL |
|-----------|-------|-----------|-----------|
| Access tail | O(1) | O(n) | O(1) |
| Remove head | O(n) | O(1) | O(1) |
| Remove middle | O(n) | O(n)* | O(1) |
| Move to tail | O(n) | O(n) | O(1) |

*Need to find previous node

**Complexity Analysis**:
- `keyAccessed()`: **O(1)** (hash lookup + DLL ops)
- `evictKey()`: **O(1)** (remove head)
- Space: **O(n)** where n = cache capacity

### Edge Cases
```cpp
// Empty cache
evictKey() → nullopt

// Single element
[A] → evict → []

// Capacity = 1
[A] → access(B) → evict A → [B]
```

---

## ✍️ Phase 5: Deep Dive - Write Policies (8 min)

### Write-Through Implementation

```cpp
class WriteThroughPolicy : public WritePolicy<K,V> {
    void write(const K& key, const V& value,
               CacheStorage<K,V>* cache,
               DBStorage<K,V>* db) override {
        
        // Parallel writes using std::async
        auto cacheFuture = async(launch::async, [&]() {
            cache->put(key, value);
        });
        
        auto dbFuture = async(launch::async, [&]() {
            db->write(key, value);
        });
        
        // Wait for both
        cacheFuture.get();
        dbFuture.get();
    }
};
```

### Comparison of Write Policies

**1. Write-Through** (Implemented)
```
Client → [Cache + DB] (parallel) → Response
         └─────┬─────┘
         Wait for both

✓ Strong consistency
✓ No data loss
✗ Higher latency
✗ DB becomes bottleneck
```

**2. Write-Back** (Future extension)
```
Client → [Cache] → Response (fast!)
         └→ [Queue] → DB (async, batched)

✓ Low latency
✓ Batch optimization
✗ Risk of data loss
✗ Complex failure handling
```

**3. Write-Around** (Future extension)
```
Client → [DB only] → Response
Cache miss → Load from DB → Cache

✓ No cache pollution
✗ Cache miss on next read
✗ No benefit for write-heavy keys
```

**Q**: "When would you choose Write-Back over Write-Through?"

**Expected Answer**:
- **Write-Back**: High write volume, tolerable data loss risk (logs, metrics)
- **Write-Through**: Financial data, strong consistency required
- **Write-Around**: Write-once-read-never data, large objects

---

## 🚀 Phase 6: Extensions & Production Concerns (10 min)

### Extension Ideas

**1. Add LFU (Least Frequently Used)**

```cpp
class LFUEvictionAlgorithm : public EvictionAlgorithm<K> {
    unordered_map<K, int> frequency;
    map<int, set<K>> freqBuckets;  // freq → keys with that freq
    
    void keyAccessed(const K& key) override {
        int oldFreq = frequency[key];
        frequency[key]++;
        
        freqBuckets[oldFreq].erase(key);
        freqBuckets[oldFreq + 1].insert(key);
    }
    
    optional<K> evictKey() override {
        auto [minFreq, keys] = *freqBuckets.begin();
        K evictKey = *keys.begin();
        // Remove from buckets and frequency map
        return evictKey;
    }
};
```

**2. TTL (Time-To-Live) Support**

```cpp
struct CacheEntry<V> {
    V value;
    chrono::time_point<chrono::system_clock> expiry;
};

// In accessData():
if (isExpired(entry)) {
    remove(key);
    throw runtime_error("Key expired");
}
```

**3. Distributed Cache (Advanced)**

```cpp
// Consistent hashing for multi-node cache
class ConsistentHash {
    map<size_t, string> ring;  // hash → node
    
    string getNode(const K& key) {
        size_t hash = hashFunc(key);
        auto it = ring.lower_bound(hash);
        return (it == ring.end()) ? ring.begin()->second : it->second;
    }
};
```

### Production Considerations

**Q**: "What would you change for production?"

**Expected Answers**:

1. **Monitoring & Metrics**
```cpp
class CacheMetrics {
    atomic<uint64_t> hits{0};
    atomic<uint64_t> misses{0};
    atomic<uint64_t> evictions{0};
    
    double hitRatio() { return hits / (hits + misses); }
};
```

2. **Error Handling**
```cpp
// Current: throws exceptions
// Production: Error codes, retry logic, circuit breakers

enum class CacheError {
    KEY_NOT_FOUND,
    CAPACITY_EXCEEDED,
    DB_UNAVAILABLE,
    TIMEOUT
};

Result<V, CacheError> accessData(const K& key);
```

3. **Resource Limits**
```cpp
// Memory limits
size_t maxMemoryBytes;
size_t currentMemoryUsage;

// Connection pooling
DBConnectionPool dbPool(maxConnections);
```

4. **Graceful Shutdown**
```cpp
// Flush dirty entries (for write-back)
// Wait for in-flight operations
// Save cache state to disk
```

---

## ❓ Phase 7: Q&A (5 min)

### Common Follow-up Questions

**Q1**: "How would you handle cache stampede?"

**Answer**: Request coalescing - if key is being fetched, queue additional requests
```cpp
unordered_map<K, shared_future<V>> inflightRequests;
```

**Q2**: "How to prevent hot key bottleneck?"

**Answer**: 
- Replicate hot keys across multiple threads
- Use probabilistic data structures (Count-Min Sketch)
- Client-side caching

**Q3**: "How to test this system?"

**Answer**:
- Unit tests: Mock storage implementations
- Concurrency tests: Multiple threads, race conditions
- Load tests: Measure throughput and latency
- Chaos tests: Inject failures (DB down, thread hangs)

---

## 📝 Summary Checklist

By end of interview, candidate should demonstrate understanding of:

- ✅ Strategy Pattern for extensibility
- ✅ Thread safety via key-based affinity
- ✅ LRU algorithm O(1) complexity
- ✅ Trade-offs between write policies
- ✅ Futures for async operations
- ✅ Template-based generic design
- ✅ Production considerations

---

## 🎯 Success Criteria

| Level | Criteria |
|-------|----------|
| **Strong Hire** | Explains all design decisions, suggests improvements, discusses trade-offs deeply |
| **Hire** | Understands architecture, explains LRU & thread safety, answers most questions |
| **Maybe** | Understands basic concepts but struggles with concurrency or trade-offs |
| **No Hire** | Cannot explain key components or design rationale |

---

**Total Time**: 60 minutes  
**Difficulty**: SDE2 Level  
**Focus Areas**: System Design, Concurrency, Data Structures, Design Patterns

