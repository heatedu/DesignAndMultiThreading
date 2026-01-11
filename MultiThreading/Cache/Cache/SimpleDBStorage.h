#pragma once
#include "DBStorage.h"
#include <unordered_map>
#include <mutex>
#include <stdexcept>

// Simple mock database storage using in-memory map
template<typename K, typename V>
class SimpleDBStorage : public DBStorage<K, V> {
private:
    std::unordered_map<K, V> database;
    mutable std::mutex mutex;

public:
    void write(const K& key, const V& value) override {
        std::lock_guard<std::mutex> lock(mutex);
        database[key] = value;
    }

    V read(const K& key) override {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = database.find(key);
        if (it == database.end()) {
            throw std::runtime_error("Key not found in DB");
        }
        return it->second;
    }

    void remove(const K& key) override {
        std::lock_guard<std::mutex> lock(mutex);
        if (database.find(key) == database.end()) {
            throw std::runtime_error("Key not found in DB");
        }
        database.erase(key);
    }
};

