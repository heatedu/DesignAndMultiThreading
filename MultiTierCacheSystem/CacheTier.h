#pragma once
#include <unordered_map>
#include <memory>
#include <optional>
#include "CacheEntry.h"
#include "EvictionPolicy.h"

class CacheTier {
private:
    size_t capacity;
    std::unordered_map<std::string, std::string> storage;

    std::shared_ptr<EvictionPolicy> evictionPolicy;
    std::shared_ptr<CacheTier> nextTier; //IMPORTANT

public:
    CacheTier(size_t cap,
              std::shared_ptr<EvictionPolicy> policy)
        : capacity(cap), evictionPolicy(policy) {}

    void setNextTier(std::shared_ptr<CacheTier> next) {
        nextTier = next;
    }

    std::optional<std::string> get(const std::string& key) {

        if (storage.count(key)) {
            evictionPolicy->recordAccess(key);
            return storage[key];
        }

        if (nextTier) {
            auto val = nextTier->get(key);
            if (val) {
                put(CacheEntry(key, *val)); // promotion
            }
            return val;
        }

        return {};
    }

    void put(CacheEntry entry) {

        if (storage.size() >= capacity) {
            auto victim = evictionPolicy->evictKey();

            if (victim && nextTier) {
                CacheEntry evicted(
                    *victim,
                    storage[*victim]);

                storage.erase(*victim);

                nextTier->put(evicted); // move down
            }
        }

        storage[entry.key] = entry.value;
        evictionPolicy->recordInsertion(entry.key);
    }
};
