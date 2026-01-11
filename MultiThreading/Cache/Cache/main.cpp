#include <iostream>
#include <string>
#include <exception>
#include "Cache.h"
#include "InMemoryCacheStorage.h"
#include "SimpleDBStorage.h"
#include "WriteThroughPolicy.h"
#include "LRUEvictionAlgorithm.h"

int main() {
    try {
        std::cout << "=== Cache System Demo ===" << std::endl << std::endl;

        // Set a small capacity for the cache (5 items)
        auto cacheStorage = new InMemoryCacheStorage<std::string, std::string>(5);
        auto dbStorage = new SimpleDBStorage<std::string, std::string>();
        auto writePolicy = new WriteThroughPolicy<std::string, std::string>();
        auto evictionAlg = new LRUEvictionAlgorithm<std::string>();

        // Create cache with 4 executor threads
        Cache<std::string, std::string> cache(cacheStorage, dbStorage, writePolicy, evictionAlg, 4);

        std::cout << "Writing 5 items to cache (capacity=5)..." << std::endl;
        cache.updateData("A", "Apple").get();
        cache.updateData("B", "Banana").get();
        cache.updateData("C", "Cherry").get();
        cache.updateData("D", "Durian").get();
        cache.updateData("E", "Elderberry").get();
        std::cout << "Cache is now full." << std::endl << std::endl;

        // Next write will trigger eviction (LRU = "A")
        std::cout << "Writing 6th item (will evict LRU key 'A')..." << std::endl;
        cache.updateData("F", "Fig").get();
        std::cout << "Key 'F' added, 'A' was evicted." << std::endl << std::endl;

        // Try to read evicted key
        std::cout << "Attempting to read evicted key 'A'..." << std::endl;
        try {
            std::string valueA = cache.accessData("A").get();
            std::cout << "A: " << valueA << std::endl;
        } catch (const std::exception& e) {
            std::cout << "A is evicted or not found in cache." << std::endl;
        }
        std::cout << std::endl;

        // Read existing key
        std::cout << "Reading key 'F'..." << std::endl;
        std::string valueF = cache.accessData("F").get();
        std::cout << "F: " << valueF << std::endl << std::endl;

        // Update existing key and read it (read-your-own-writes)
        std::cout << "Updating key 'B' and reading it back..." << std::endl;
        cache.updateData("B", "Blueberry").get();
        std::string valueB = cache.accessData("B").get();
        std::cout << "B: " << valueB << std::endl << std::endl;

        // Demonstrate thread affinity with multiple operations on same key
        std::cout << "Testing thread affinity with multiple operations on 'C'..." << std::endl;
        cache.updateData("C", "Cranberry").get();
        std::string valueC = cache.accessData("C").get();
        std::cout << "C: " << valueC << std::endl << std::endl;

        std::cout << "=== Demo Complete ===" << std::endl;
        std::cout << "Key Design Points Demonstrated:" << std::endl;
        std::cout << "1. LRU Eviction: 'A' was evicted when 'F' was added" << std::endl;
        std::cout << "2. Write-Through: Data written to both cache and DB concurrently" << std::endl;
        std::cout << "3. Read-Your-Own-Writes: Updated 'B' immediately visible" << std::endl;
        std::cout << "4. Thread Affinity: Same key operations on same thread" << std::endl;
        std::cout << "5. Strategy Pattern: Pluggable eviction & write policies" << std::endl;

        cache.shutdown();

        // Cleanup
        delete cacheStorage;
        delete dbStorage;
        delete writePolicy;
        delete evictionAlg;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

