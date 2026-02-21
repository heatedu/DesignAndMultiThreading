# Multi-Tier Cache System (C++ LLD)

## Overview

A multi-tier cache system with **RAM → SSD → HDD** layers. Each tier has independent capacity and uses an **LRU eviction policy**. When a tier is full, the least recently used entry cascades down. When a key is found in a lower tier, it gets promoted back to RAM.

## Design Patterns Used

| Pattern                  	  | Where                        	|
|-----------------------------|---------------------------------|
| **Singleton**            	  | `CacheManager`               	|
| **Factory**              	  | `CacheFactory::createTier()` 	|
| **Strategy**             	  | `EvictionPolicy` / `LRUEviction`|
| **Chain of Responsibility** | `CacheTier::nextTier`           |

## File Structure

```
MultiTierCacheSystem/
├── CacheEntry.h          // Key-Value entity
├── EvictionPolicy.h      // Strategy interface
├── LRUEviction.h         // LRU concrete strategy
├── CacheTier.h           // Chain node (RAM/SSD/HDD)
├── CacheFactory.h        // Factory for creating tiers
├── CacheManager.h        // Singleton entry point
└── main.cpp              // Demo
```

## API

```cpp
CacheManager& cache = CacheManager::getInstance();
cache.put("key", "value");
std::optional<std::string> val = cache.get("key");
```

## How Move-Down (Eviction Cascade) Works

Refer to `CacheTier::put()` — when `storage.size() >= capacity`, the LRU victim is evicted and pushed to `nextTier->put(evicted)`.

### Walk-through (RAM capacity = 3, SSD capacity = 5, HDD capacity = 10)

```
put("A", "1")    RAM: [A]              SSD: []         HDD: []
put("B", "2")    RAM: [B, A]           SSD: []         HDD: []
put("C", "3")    RAM: [C, B, A]        SSD: []         HDD: []
                 ↑ RAM is now full (3/3)

put("D", "4")    RAM is full → LRU victim is "A"
                 "A" moves DOWN to SSD via nextTier->put(evicted)
                 
                 RAM: [D, C, B]        SSD: [A]        HDD: []
                      ↑ "A" evicted         ↑ "A" landed here

put("E", "5")    RAM full → LRU victim is "B"
                 RAM: [E, D, C]        SSD: [B, A]     HDD: []

put("F", "6")    RAM full → LRU victim is "C"
                 RAM: [F, E, D]        SSD: [C, B, A]  HDD: []
```

If SSD also fills up (5/5), its LRU victim cascades further down to HDD — same logic, recursively.

```
// Continuing... after more puts, SSD fills to 5/5:
                 RAM: [H, G, F]    SSD: [E, D, C, B, A]    HDD: []
                                        ↑ SSD full (5/5)

put("I", "9")    RAM full → "F" evicted to SSD
                 SSD full  → "A" evicted to HDD (SSD's LRU victim)

                 RAM: [I, H, G]    SSD: [F, E, D, C, B]    HDD: [A]
                                        ↑ "F" arrived           ↑ "A" cascaded here
```

### Code responsible (`CacheTier.h`):

```cpp
void put(CacheEntry entry) {
    if (storage.size() >= capacity) {
        auto victim = evictionPolicy->evictKey();   // LRU picks least recent

        if (victim && nextTier) {
            CacheEntry evicted(*victim, storage[*victim]);
            storage.erase(*victim);
            nextTier->put(evicted);                 // ← MOVE DOWN to next tier
        }
    }
    storage[entry.key] = entry.value;
    evictionPolicy->recordInsertion(entry.key);
}
```

## How Promotion Works

Refer to `CacheTier::get()` — when a key is NOT found in the current tier but IS found in a lower tier, it gets **promoted** back up to the top tier by calling `put()` on itself.

### Walk-through

Starting state (from above):
```
RAM: [D, C, B]        SSD: [A]        HDD: []
      ↑ most recent
```

Now call `get("A")`:

```
Step 1:  RAM.get("A")  → not found in RAM storage
Step 2:  RAM asks nextTier (SSD) → SSD.get("A")
Step 3:  SSD finds "A" in its storage → returns "1"
Step 4:  Back in RAM.get(), val has a value →
         RAM.put(CacheEntry("A", "1"))    ← PROMOTION
         This inserts "A" into RAM (evicting RAM's LRU if full)

Result:  RAM: [A, D, C]       SSD: [A]        HDD: []
               ↑ promoted!
         (Note: "B" was evicted from RAM → moved to SSD)
         RAM: [A, D, C]       SSD: [B, A]     HDD: []
```

### Code responsible (`CacheTier.h`):

```cpp
std::optional<std::string> get(const std::string& key) {
    if (storage.count(key)) {
        evictionPolicy->recordAccess(key);
        return storage[key];                        // found locally
    }

    if (nextTier) {
        auto val = nextTier->get(key);
        if (val) {
            put(CacheEntry(key, *val));             // ← PROMOTION to this tier
        }
        return val;
    }
    return {};
}
```

> **Key insight:** Promotion triggers a `put()` which may itself trigger an eviction cascade. So reading a cold key can cause a chain reaction: promote to RAM → evict RAM's LRU to SSD → evict SSD's LRU to HDD.

## Build & Run

Open `MultiTierCacheSystem.sln` in Visual Studio 2022 → Build → Run (Ctrl+F5).

**Expected output:**
```
Found: 1
```
