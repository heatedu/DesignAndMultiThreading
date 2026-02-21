#pragma once
#include <memory>
#include "CacheTier.h"
#include "LRUEviction.h"

class CacheFactory {
public:
    static std::shared_ptr<CacheTier>
    createTier(size_t capacity) {

        return std::make_shared<CacheTier>(
            capacity,
            std::make_shared<LRUEviction>());
    }
};
