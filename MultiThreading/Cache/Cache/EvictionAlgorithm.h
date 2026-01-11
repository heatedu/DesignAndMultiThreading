#pragma once
#include <optional>

// Strategy interface for eviction algorithms
template<typename K>
class EvictionAlgorithm {
public:
    virtual ~EvictionAlgorithm() = default;

    // Notify that a key was accessed (for tracking usage)
    virtual void keyAccessed(const K& key) = 0;

    // Select and return a key to evict (returns nullopt if no key to evict)
    virtual std::optional<K> evictKey() = 0;
};

