#pragma once
#include <stdexcept>

// Interface for database/persistent storage operations
template<typename K, typename V>
class DBStorage {
public:
    virtual ~DBStorage() = default;

    virtual void write(const K& key, const V& value) = 0;
    virtual V read(const K& key) = 0;
    virtual void remove(const K& key) = 0;
};

