#pragma once
#include "CacheStorage.h"
#include <unordered_map>
#include <mutex>
#include <stdexcept>

// Concrete implementation of in-memory cache storage
template<typename K, typename V>
class InMemoryCacheStorage : public CacheStorage<K, V> {
private:
    std::unordered_map<K, V> cache;
    int capacity;
    mutable std::mutex mutex;

public:
    explicit InMemoryCacheStorage(int cap) : capacity(cap) {}

    void put(const K& key, const V& value) override {
        std::lock_guard<std::mutex> lock(mutex);
        cache[key] = value;
    }

    V get(const K& key) override {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = cache.find(key);
        if (it == cache.end()) {
            throw std::runtime_error("Key not in cache");
        }
        return it->second;
    }

    void remove(const K& key) override {
        std::lock_guard<std::mutex> lock(mutex);
        if (cache.find(key) == cache.end()) {
            throw std::runtime_error("Key not in cache");
        }
        cache.erase(key);
    }

    bool containsKey(const K& key) override {
        std::lock_guard<std::mutex> lock(mutex);
        return cache.find(key) != cache.end();
    }

    int size() const override {
        std::lock_guard<std::mutex> lock(mutex);
        return static_cast<int>(cache.size());
    }

    int getCapacity() const override {
        return capacity;
    }
};

