#pragma once
#include "CacheStorage.h"
#include "DBStorage.h"

// Strategy interface for write policies
template<typename K, typename V>
class WritePolicy {
public:
    virtual ~WritePolicy() = default;

    // Write to both cache and DB storage (implementation depends on policy)
    virtual void write(const K& key, const V& value, 
                      CacheStorage<K, V>* cacheStorage, 
                      DBStorage<K, V>* dbStorage) = 0;
};

