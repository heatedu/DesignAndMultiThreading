#pragma once
#include "CacheStorage.h"
#include "DBStorage.h"
#include "WritePolicy.h"
#include "EvictionAlgorithm.h"
#include "KeyBasedExecutor.h"
#include <future>
#include <memory>
#include <iostream>

// Core Cache class that integrates all components
template<typename K, typename V>
class Cache {
private:
    CacheStorage<K, V>* cacheStorage;
    DBStorage<K, V>* dbStorage;
    WritePolicy<K, V>* writePolicy;
    EvictionAlgorithm<K>* evictionAlgorithm;
    KeyBasedExecutor keyBasedExecutor;

public:
    Cache(CacheStorage<K, V>* cacheStor,
          DBStorage<K, V>* dbStor,
          WritePolicy<K, V>* writePol,
          EvictionAlgorithm<K>* evictAlg,
          int numExecutors)
        : cacheStorage(cacheStor),
          dbStorage(dbStor),
          writePolicy(writePol),
          evictionAlgorithm(evictAlg),
          keyBasedExecutor(numExecutors) {}

    // Read data from cache (updates eviction algorithm)
    std::future<V> accessData(const K& key) {
        return keyBasedExecutor.submitTask(key, [this, key]() -> V {
            if (!cacheStorage->containsKey(key)) {
                throw std::runtime_error("Key not found in cache");
            }
            evictionAlgorithm->keyAccessed(key);
            return cacheStorage->get(key);
        });
    }

    // Write/update data in cache and DB
    std::future<void> updateData(const K& key, const V& value) {
        return keyBasedExecutor.submitTask(key, [this, key, value]() -> void {
            if (cacheStorage->containsKey(key)) {
                // Update existing key
                writePolicy->write(key, value, cacheStorage, dbStorage);
                evictionAlgorithm->keyAccessed(key);
            } else {
                // New key: check if cache is full
                if (cacheStorage->size() >= cacheStorage->getCapacity()) {
                    auto evictedKeyOpt = evictionAlgorithm->evictKey();
                    if (evictedKeyOpt.has_value()) {
                        K evictedKey = evictedKeyOpt.value();
                        
                        int currentIndex = keyBasedExecutor.getExecutorIndexForKey(key);
                        int evictedIndex = keyBasedExecutor.getExecutorIndexForKey(evictedKey);
                        
                        if (currentIndex == evictedIndex) {
                            // Same thread, remove directly
                            cacheStorage->remove(evictedKey);
                        } else {
                            // Different thread, submit removal task and wait
                            auto removalFuture = keyBasedExecutor.submitTask(evictedKey, [this, evictedKey]() {
                                cacheStorage->remove(evictedKey);
                            });
                            removalFuture.get();
                        }
                    }
                }
                
                // Write new key/value
                writePolicy->write(key, value, cacheStorage, dbStorage);
                evictionAlgorithm->keyAccessed(key);
            }
        });
    }

    void shutdown() {
        keyBasedExecutor.shutdown();
    }
};

