#pragma once
#include <string>

class CacheEntry {
public:
    std::string key;
    std::string value;

    CacheEntry(std::string k, std::string v)
        : key(k), value(v) {}
};
