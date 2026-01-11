#pragma once
#include <stdexcept>
#include <string>

// Interface for cache storage operations
template<typename K, typename V>
class CacheStorage {
public:
    virtual ~CacheStorage() = default;

    virtual void put(const K& key, const V& value) = 0;
    virtual V get(const K& key) = 0;
    virtual void remove(const K& key) = 0;
    virtual bool containsKey(const K& key) = 0;
    virtual int size() const = 0;
    virtual int getCapacity() const = 0;
};

