# Cache System - Low Level Design (C++)

A production-ready, thread-safe cache system demonstrating key design patterns and concurrency concepts. Perfect for SDE2 interview discussions (1 hour session).

## 🎯 Key Features

- **Thread-Safe Operations**: Key-based thread affinity ensures "read your own writes" consistency
- **LRU Eviction**: Efficient Least Recently Used eviction algorithm
- **Write-Through Policy**: Concurrent writes to cache and persistent storage
- **Strategy Pattern**: Pluggable eviction algorithms and write policies
- **Generic Design**: Template-based implementation for any key-value types
- **Production-Ready**: Proper error handling, RAII, and modern C++17

## 🏗️ Architecture

### Core Components

```
Cache System
├── Interfaces
│   ├── CacheStorage<K,V>    - In-memory cache operations
│   ├── DBStorage<K,V>        - Persistent storage operations
│   ├── WritePolicy<K,V>      - Write strategy interface
│   └── EvictionAlgorithm<K>  - Eviction strategy interface
│
├── Implementations
│   ├── InMemoryCacheStorage  - Concurrent hash map based cache
│   ├── SimpleDBStorage       - Mock database storage
│   ├── WriteThroughPolicy    - Concurrent write to cache & DB
│   └── LRUEvictionAlgorithm  - LRU using doubly linked list
│
├── Utilities
│   ├── DoublyLinkedList      - Custom DLL for LRU tracking
│   └── KeyBasedExecutor      - Thread pool with key affinity
│
└── Core
    └── Cache<K,V>            - Main cache orchestrator
```

## 🔑 Design Patterns Used

1. **Strategy Pattern**: Interchangeable eviction algorithms and write policies
2. **Template Method**: Abstract interfaces with concrete implementations
3. **RAII**: Resource management through destructors
4. **Dependency Injection**: Components injected into Cache constructor

## 🚀 Quick Start

### Build & Run (Visual Studio)

1. Open `Cache.sln` in Visual Studio 2022 or later
2. Build the solution (Ctrl+Shift+B)
3. Run (F5 or Ctrl+F5)

### Expected Output

```
=== Cache System Demo ===

Writing 5 items to cache (capacity=5)...
Cache is now full.

Writing 6th item (will evict LRU key 'A')...
Key 'F' added, 'A' was evicted.

Attempting to read evicted key 'A'...
A is evicted or not found in cache.

Reading key 'F'...
F: Fig

Updating key 'B' and reading it back...
B: Blueberry

=== Demo Complete ===
```

## 💡 Interview Discussion Points

### 1. **Thread Safety** (10 mins)
   - How does KeyBasedExecutor ensure same-key operations are serialized?
   - Why use key-based hashing instead of global locks?
   - Thread affinity guarantees "read your own writes"

### 2. **Eviction Algorithm** (10 mins)
   - LRU implementation: DoublyLinkedList + HashMap
   - O(1) access, update, and eviction
   - How to extend to LFU or mixed policies?

### 3. **Write Policies** (10 mins)
   - Write-Through: Concurrent writes using std::async
   - Future extensions: Write-Back, Write-Around
   - Trade-offs: Consistency vs Performance

### 4. **Concurrency Design** (15 mins)
   - Why key-based executors instead of thread-per-request?
   - How futures enable async operations
   - Cross-executor eviction handling

### 5. **Extensibility** (10 mins)
   - Adding new eviction algorithms (LFU, FIFO)
   - Adding new write policies
   - Supporting different storage backends

### 6. **Production Concerns** (5 mins)
   - Error handling strategy
   - Resource cleanup and shutdown
   - Memory management considerations

## 📝 Key Classes

### Cache<K, V>
Main orchestrator that integrates all components.

```cpp
Cache(CacheStorage<K,V>* cacheStorage,
      DBStorage<K,V>* dbStorage,
      WritePolicy<K,V>* writePolicy,
      EvictionAlgorithm<K>* evictionAlg,
      int numExecutors);

std::future<V> accessData(const K& key);
std::future<void> updateData(const K& key, const V& value);
```

### KeyBasedExecutor
Ensures all operations for the same key execute on the same thread.

```cpp
template<typename K, typename Func>
auto submitTask(const K& key, Func&& func) 
    -> std::future<decltype(func())>;
```

### LRUEvictionAlgorithm<K>
O(1) eviction using doubly linked list.

```cpp
void keyAccessed(const K& key) override;
std::optional<K> evictKey() override;
```

## 🔧 Technical Requirements

- **C++17 or later** (for std::optional, structured bindings)
- **Visual Studio 2022** (or any C++17 compliant compiler)
- **Windows 10+** (for Visual Studio; adaptable to Linux/Mac)

## 📚 Files Structure

```
Cache/
├── Cache.sln                      # Visual Studio solution
├── README.md                      # This file
├── INTERVIEW_GUIDE.md             # Detailed interview discussion guide
└── Cache/
    ├── Cache.vcxproj              # Visual Studio project
    ├── Main.cpp                   # Demo program
    │
    ├── Interfaces/
    │   ├── CacheStorage.h
    │   ├── DBStorage.h
    │   ├── WritePolicy.h
    │   └── EvictionAlgorithm.h
    │
    ├── Implementations/
    │   ├── InMemoryCacheStorage.h
    │   ├── SimpleDBStorage.h
    │   ├── WriteThroughPolicy.h
    │   └── LRUEvictionAlgorithm.h
    │
    └── Utilities/
        ├── DoublyLinkedList.h
        └── KeyBasedExecutor.h
```

## 🎓 Learning Objectives

After discussing this code, you should understand:

1. ✅ How to design thread-safe cache systems
2. ✅ Strategy pattern for pluggable algorithms
3. ✅ LRU implementation details
4. ✅ Key-based thread affinity patterns
5. ✅ Async operations using futures
6. ✅ Template-based generic programming in C++
7. ✅ Trade-offs in cache design decisions

## 🚦 Extension Ideas

1. **Add LFU Eviction**: Implement Least Frequently Used algorithm
2. **Write-Back Policy**: Batch writes to DB for performance
3. **TTL Support**: Time-to-live for cache entries
4. **Cache Warming**: Pre-populate cache on startup
5. **Metrics**: Hit/miss ratio tracking
6. **Distributed Cache**: Consistent hashing across nodes

## 📖 Reference

Based on: [Design Cache System](https://codewitharyan.com/tech-blogs/design-cache)

## 👤 Author

Interview-ready implementation for SDE2 discussions.

---
**Time Estimate**: 1 hour discussion covering all major design points

