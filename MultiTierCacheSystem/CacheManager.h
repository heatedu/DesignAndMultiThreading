#pragma once
#include <memory>
#include "CacheTier.h"
#include "CacheFactory.h"

class CacheManager {

private:
    std::shared_ptr<CacheTier> topTier;

    CacheManager() {
        auto ram = CacheFactory::createTier(3);
        auto ssd = CacheFactory::createTier(5);
        auto hdd = CacheFactory::createTier(10);

        ram->setNextTier(ssd);
        ssd->setNextTier(hdd);

        topTier = ram;
    }

public:
    static CacheManager& getInstance() {
        static CacheManager instance;
        return instance;
    }

    void put(std::string key, std::string value) {
        topTier->put(CacheEntry(key, value));
    }

    std::optional<std::string> get(std::string key) {
        return topTier->get(key);
    }
};
