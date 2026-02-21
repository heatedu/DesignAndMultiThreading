#pragma once
#include "EvictionPolicy.h"
#include <list>
#include <unordered_map>

class LRUEviction : public EvictionPolicy {
private:
    std::list<std::string> lru;
    std::unordered_map<std::string,
        std::list<std::string>::iterator> map;

public:
    void recordAccess(const std::string& key) override {
        if (map.count(key)) {
            lru.erase(map[key]);
        }
        lru.push_front(key);
        map[key] = lru.begin();
    }

    void recordInsertion(const std::string& key) override {
        recordAccess(key);
    }

    std::optional<std::string> evictKey() override {
        if (lru.empty()) return {};
        std::string victim = lru.back();
        lru.pop_back();
        map.erase(victim);
        return victim;
    }
};
