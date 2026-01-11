#ifndef REPLENISHMENTSTRATEGY_H
#define REPLENISHMENTSTRATEGY_H

#include "Product.h"
#include <iostream>

// Interface for different replenishment strategies (from Java: ReplenishmentStrategy.java)
class ReplenishmentStrategy {
public:
    virtual ~ReplenishmentStrategy() = default;
    // Method to replenish stock for a given product
    virtual void replenish(Product* product) = 0;
};

// Just-In-Time replenishment strategy (from Java: JustInTimeStrategy.java)
class JustInTimeStrategy : public ReplenishmentStrategy {
public:
    void replenish(Product* product) override {
        // Implement Just-In-Time replenishment logic
        std::cout << "Applying Just-In-Time replenishment for " << product->getName() << std::endl;
        // Calculate optimal order quantity based on demand rate
    }
};

// Bulk Order replenishment strategy (from Java: BulkOrderStrategy.java)
class BulkOrderStrategy : public ReplenishmentStrategy {
public:
    void replenish(Product* product) override {
        // Implement Bulk Order replenishment logic
        std::cout << "Applying Bulk Order replenishment for " << product->getName() << std::endl;
        // Order in large quantities to minimize order costs
    }
};

#endif // REPLENISHMENTSTRATEGY_H
