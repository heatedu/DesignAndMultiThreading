#include <iostream>
#include "CacheManager.h"

int main() {

    auto& cache = CacheManager::getInstance();

    cache.put("A", "1");
    cache.put("B", "2");
    cache.put("C", "3");
    cache.put("D", "4"); // eviction cascade

    auto val = cache.get("A");

    if (val)
        std::cout << "Found: " << *val << "\n";
    else
        std::cout << "Not Found\n";

    return 0;
}
