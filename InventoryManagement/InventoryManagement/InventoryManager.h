#ifndef INVENTORYMANAGER_H
#define INVENTORYMANAGER_H

#include "Warehouse.h"
#include "ProductFactory.h"
#include "ReplenishmentStrategy.h"
#include "Observer.h"
#include <vector>
#include <algorithm>
#include <mutex>

// InventoryManager class implementing Singleton Pattern (from Java: InventoryManager.java)
class InventoryManager {
private:
    // Singleton instance
    static InventoryManager* instance;
    static std::mutex mutex_;

    // System components
    std::vector<Warehouse*> warehouses;
    ProductFactory productFactory;
    std::vector<InventoryObserver*> observers;
    ReplenishmentStrategy* replenishmentStrategy;

    // Private constructor to prevent instantiation from outside
    InventoryManager() : replenishmentStrategy(nullptr) {
        // Initialize collections and dependencies
    }

public:
    // Delete copy/move
    InventoryManager(const InventoryManager&) = delete;
    InventoryManager& operator=(const InventoryManager&) = delete;

    // Static method to get the singleton instance with thread safety
    static InventoryManager* getInstance() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (instance == nullptr) {
            instance = new InventoryManager();
        }
        return instance;
    }

    // Get product factory
    ProductFactory& getProductFactory() {
        return productFactory;
    }

    // Observer pattern methods (from Java: InventoryManager.java)
    void addObserver(InventoryObserver* observer) {
        observers.push_back(observer);
    }

    void removeObserver(InventoryObserver* observer) {
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end()
        );
    }

    void notifyObservers(Product* product) {
        for (auto* observer : observers) {
            observer->update(product);
        }
    }

    // Strategy pattern method
    void setReplenishmentStrategy(ReplenishmentStrategy* strategy) {
        this->replenishmentStrategy = strategy;
    }

    // Warehouse management
    void addWarehouse(Warehouse* warehouse) {
        warehouses.push_back(warehouse);
    }

    void removeWarehouse(Warehouse* warehouse) {
        warehouses.erase(
            std::remove(warehouses.begin(), warehouses.end(), warehouse),
            warehouses.end()
        );
    }

    // Product inventory operations
    Product* getProductBySku(const std::string& sku) {
        for (auto* warehouse : warehouses) {
            Product* product = warehouse->getProductBySku(sku);
            if (product != nullptr) {
                return product;
            }
        }
        return nullptr;
    }

    // Check stock levels and apply replenishment strategy if needed
    void checkAndReplenish(const std::string& sku) {
        Product* product = getProductBySku(sku);
        if (product != nullptr) {
            // If product is below threshold, notify observers
            if (product->getQuantity() < product->getThreshold()) {
                notifyObservers(product);
                // Apply current replenishment strategy
                if (replenishmentStrategy != nullptr) {
                    replenishmentStrategy->replenish(product);
                }
            }
        }
    }

    // Global inventory check
    void performInventoryCheck() {
        for (auto* warehouse : warehouses) {
            for (auto* product : warehouse->getAllProducts()) {
                if (product->getQuantity() < product->getThreshold()) {
                    notifyObservers(product);
                    if (replenishmentStrategy != nullptr) {
                        replenishmentStrategy->replenish(product);
                    }
                }
            }
        }
    }
};

// Static member definitions
InventoryManager* InventoryManager::instance = nullptr;
std::mutex InventoryManager::mutex_;

#endif // INVENTORYMANAGER_H
