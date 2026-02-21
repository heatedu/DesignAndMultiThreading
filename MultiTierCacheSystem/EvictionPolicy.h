#pragma once
#include <string>
#include <optional>
#include "CacheEntry.h"

class EvictionPolicy {
public:
    virtual void recordAccess(const std::string& key) = 0;
    virtual void recordInsertion(const std::string& key) = 0;
    virtual std::optional<std::string> evictKey() = 0;
    virtual ~EvictionPolicy() = default;
};
