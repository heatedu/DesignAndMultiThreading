#pragma once
#include "EvictionAlgorithm.h"
#include <list>
#include <unordered_map>
#include <mutex>
#include <optional>

// LRU (Least Recently Used) eviction algorithm using std::list
template<typename K>
class LRUEvictionAlgorithm : public EvictionAlgorithm<K> {
private:
    std::list<K> cache;  // Front = MRU (Most Recently Used), Back = LRU
    std::unordered_map<K, typename std::list<K>::iterator> mp;
    mutable std::mutex mutex;

    // Refresh position: move key to front (MRU)
    void refreshPosition(const K& key) {
        cache.erase(mp[key]);
        cache.push_front(key);
        mp[key] = cache.begin();
    }

public:
    void keyAccessed(const K& key) override {
        std::lock_guard<std::mutex> lock(mutex);
        
        if (mp.count(key)) {
            // Existing key: move to front (most recently used)
            refreshPosition(key);
        } else {
            // New key: add to front
            cache.push_front(key);
            mp[key] = cache.begin();
        }
    }

    std::optional<K> evictKey() override {
        std::lock_guard<std::mutex> lock(mutex);
        
        if (cache.empty()) {
            return std::nullopt;
        }
        
        // Evict from back (least recently used)
        K evictKey = cache.back();
        mp.erase(evictKey);
        cache.pop_back();
        
        return evictKey;
    }
};

