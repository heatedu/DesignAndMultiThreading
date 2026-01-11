# Cache System - Quick Reference

Quick cheat sheet for the cache implementation. Perfect for last-minute interview prep.

---

## 🚀 Quick Start

```cpp
// Create components
auto cache = new InMemoryCacheStorage<string, string>(5);
auto db = new SimpleDBStorage<string, string>();
auto policy = new WriteThroughPolicy<string, string>();
auto eviction = new LRUEvictionAlgorithm<string>();

// Create cache system
Cache<string, string> cacheSystem(cache, db, policy, eviction, 4);

// Use the cache
cacheSystem.updateData("key", "value").get();  // Write
string val = cacheSystem.accessData("key").get();  // Read
```

---

## 📊 Key Interfaces

### CacheStorage<K, V>
```cpp
void put(const K& key, const V& value);
V get(const K& key);
void remove(const K& key);
bool containsKey(const K& key);
int size() const;
int getCapacity() const;
```

### DBStorage<K, V>
```cpp
void write(const K& key, const V& value);
V read(const K& key);
void remove(const K& key);
```

### WritePolicy<K, V>
```cpp
void write(const K& key, const V& value,
           CacheStorage<K,V>* cache,
           DBStorage<K,V>* db);
```

### EvictionAlgorithm<K>
```cpp
void keyAccessed(const K& key);
optional<K> evictKey();
```

---

## 🔑 Design Patterns

| Pattern | Usage | Location |
|---------|-------|----------|
| **Strategy** | Pluggable policies | WritePolicy, EvictionAlgorithm |
| **Template Method** | Interface + implementations | All interfaces |
| **Dependency Injection** | Component wiring | Cache constructor |
| **RAII** | Resource cleanup | Destructors |

---

## 🧵 Thread Safety Model

```
Key Hashing → Thread Assignment → Serialized Execution

hash("A") % 4 = 2 → Thread 2 → [Op1, Op2, Op3] (sequential)
hash("B") % 4 = 1 → Thread 1 → [Op1, Op2] (sequential)
hash("C") % 4 = 2 → Thread 2 → (waits for "A" ops)

✓ Same key operations: Serialized (thread-safe)
✓ Different key operations: Parallel (high throughput)
✓ "Read your own writes": Guaranteed
```

---

## 📈 Complexity Analysis

| Operation | Time | Space |
|-----------|------|-------|
| `accessData(key)` | O(1) amortized | O(1) |
| `updateData(key, value)` | O(1) amortized | O(1) |
| LRU `keyAccessed()` | O(1) | O(1) |
| LRU `evictKey()` | O(1) | O(1) |
| Total Space | O(n) | n = capacity |

---

## 🏗️ LRU Data Structure

```cpp
DoublyLinkedList<K> dll;  // Head=LRU, Tail=MRU
unordered_map<K, DLLNode<K>*> nodeMap;

// Operations:
keyAccessed(key):
  1. Find node in map: O(1)
  2. Detach from list: O(1)
  3. Add to tail: O(1)
  
evictKey():
  1. Get head node: O(1)
  2. Remove from list: O(1)
  3. Remove from map: O(1)
```

---

## ⚖️ Write Policy Trade-offs

### Write-Through (Implemented)
```
Client → [Cache + DB] → Response
Latency: High | Consistency: Strong | Data Loss: None
```

### Write-Back (Extension)
```
Client → [Cache] → Response | DB ← [Async Flush]
Latency: Low | Consistency: Eventual | Data Loss: Possible
```

### Write-Around (Extension)
```
Client → [DB] → Response | Cache: Miss → Load
Latency: Medium | Consistency: Strong | Cache Hit: Lower
```

---

## 🔧 KeyBasedExecutor Internals

```cpp
class KeyBasedExecutor {
  vector<ExecutorThread> executors;  // Fixed pool
  
  submitTask(key, func):
    1. index = hash(key) % numExecutors
    2. executors[index].enqueue(func)
    3. return future<T>
    
  ExecutorThread::run():
    while (!stopped):
      task = queue.pop()  // Blocks if empty
      task.execute()
};
```

---

## 🎯 Interview Talking Points

### 1. Why This Design?
- **Interfaces**: Testability, flexibility, extensibility
- **Strategy Pattern**: Runtime policy selection
- **Key-based affinity**: Ordering + "read your own writes"
- **Futures**: Non-blocking async operations

### 2. Critical Design Decisions

**Q: Why not global lock?**
```cpp
// ❌ Global lock: Serializes all operations
mutex globalMutex;
void put(K k, V v) { lock(globalMutex); cache[k]=v; }

// ✅ Key-based: Only same-key operations serialized
executor[hash(k)%N].submit([=]() { cache[k]=v; });
```

**Q: Why concurrent writes in WriteThroughPolicy?**
```cpp
// Parallel DB + Cache writes save latency
async([&](){ cache.put(k,v); });  // ~5ms
async([&](){ db.write(k,v); });   // ~20ms
// Total: max(5ms, 20ms) = 20ms instead of 25ms
```

**Q: Why separate eviction tracking?**
```cpp
// Decouples eviction logic from storage
// Easy to swap LRU ↔ LFU ↔ FIFO
cache.setEvictionPolicy(new LFUEvictionAlgorithm());
```

### 3. Edge Cases Handled

```cpp
// ✓ Empty cache eviction → returns nullopt
// ✓ Capacity = 1 → works correctly
// ✓ Cross-thread eviction → submits removal to correct thread
// ✓ Concurrent same-key ops → serialized by executor
// ✓ Exception handling → propagated via futures
```

### 4. Extension Points

```cpp
// New Eviction Algorithm
class LFUEvictionAlgorithm : public EvictionAlgorithm<K> { ... }

// New Write Policy
class WriteBackPolicy : public WritePolicy<K,V> { ... }

// New Storage Backend
class RedisStorage : public CacheStorage<K,V> { ... }

// Usage (no code changes to Cache class!)
Cache cache(redisStorage, mysqlDB, writeBackPolicy, lfu, 4);
```

---

## 🐛 Common Pitfalls

### ❌ Don't Do This
```cpp
// 1. Using global lock
mutex globalLock;
cache.put() { lock(globalLock); ... }  // Kills parallelism

// 2. Forgetting to call .get() on futures
cache.updateData(k, v);  // Doesn't wait!

// 3. Mixing executor threads
cache[k] = v;  // Bypasses executor, breaks ordering

// 4. Deleting components before shutdown
delete cacheStorage;  // Cache still using it!
cache.shutdown();     // Too late
```

### ✅ Do This Instead
```cpp
// 1. Key-based affinity
executor.submitTask(key, [](){ ... });

// 2. Wait for operations
auto future = cache.updateData(k, v);
future.get();  // Ensures completion

// 3. Always use Cache methods
cache.updateData(k, v).get();  // Routes through executor

// 4. Proper cleanup order
cache.shutdown();      // First
delete components;     // Then
```

---

## 📊 Performance Characteristics

```
Cache Size: 1000 items
Threads: 4
Operations: 1M mixed read/write

Throughput: ~200K ops/sec
Avg Latency: 20μs (cache hit)
P99 Latency: 50μs
Hit Ratio: 95%

Bottlenecks:
- Hot keys: Limited by single thread
- Write-through: DB write latency
- Eviction: Lock contention in LRU
```

---

## 🔬 Testing Strategy

```cpp
// Unit Tests
TEST(LRU, BasicEviction) { /* Add n+1 items, check first evicted */ }
TEST(WriteThrough, BothStoragesUpdated) { /* Mock storages */ }

// Concurrency Tests  
TEST(ThreadSafety, ConcurrentSameKey) { /* 100 threads, same key */ }
TEST(ThreadSafety, ParallelDifferentKeys) { /* Verify no contention */ }

// Integration Tests
TEST(EndToEnd, ReadYourOwnWrites) { /* Write then immediate read */ }
TEST(EndToEnd, Eviction) { /* Fill cache, verify LRU evicted */ }

// Load Tests
TEST(Performance, Throughput) { /* Measure ops/sec */ }
TEST(Performance, Latency) { /* P50, P95, P99 */ }
```

---

## 💡 One-Liners for Interview

| Question | Answer |
|----------|--------|
| **Why Strategy Pattern?** | "Swap algorithms without changing Cache code" |
| **Why key-based threads?** | "Ordering guarantees + 'read your own writes'" |
| **LRU complexity?** | "O(1) all operations via HashMap + DLL" |
| **Thread safety?** | "Same-key serialized, different-keys parallel" |
| **Write-Through trade-off?** | "High consistency, higher latency" |
| **Production ready?** | "Add metrics, error codes, TTL, connection pooling" |

---

## 🎓 Key Takeaways

1. **Design Patterns**: Strategy for extensibility, DI for composition
2. **Concurrency**: Key-based affinity balances safety and performance  
3. **Data Structures**: HashMap + DLL gives O(1) LRU
4. **Futures**: Enable async operations without blocking
5. **Trade-offs**: Consistency vs performance, simplicity vs features

---

**Time to Master**: 3-4 hours hands-on  
**Interview Time**: 1 hour discussion  
**Difficulty**: SDE2 Level

For detailed explanations, see: `INTERVIEW_GUIDE.md`

