#pragma once
#include "WritePolicy.h"
#include <future>
#include <vector>

// Write-through policy: writes to cache and DB concurrently
template<typename K, typename V>
class WriteThroughPolicy : public WritePolicy<K, V> {
public:
    void write(const K& key, const V& value,
              CacheStorage<K, V>* cacheStorage,
              DBStorage<K, V>* dbStorage) override {
        
        // Write to both cache and DB concurrently
        auto cacheFuture = std::async(std::launch::async, [&]() {
            cacheStorage->put(key, value);
        });

        auto dbFuture = std::async(std::launch::async, [&]() {
            dbStorage->write(key, value);
        });

        // Wait for both to complete
        cacheFuture.get();
        dbFuture.get();
    }
};

