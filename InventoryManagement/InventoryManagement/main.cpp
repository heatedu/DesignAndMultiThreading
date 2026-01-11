// Main.cpp - Demonstrating all patterns (without Builder)
#include "InventoryManager.h"
#include <iostream>

int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "   INVENTORY MANAGEMENT SYSTEM" << std::endl;
    std::cout << "============================================" << std::endl;

    // Get the singleton instance of InventoryManager
    InventoryManager* inventoryManager = InventoryManager::getInstance();

    // ========== OBSERVER PATTERN SETUP ==========
    std::cout << "\n--- Setting up Observers ---" << std::endl;
    SupplierNotifier supplierNotifier("ABC Suppliers Inc.", "supplier@abc.com");
    std::vector<std::string> adminUsers = {"admin@company.com", "manager@company.com"};
    DashboardAlertSystem dashboardAlert("HIGH", adminUsers);
    
    inventoryManager->addObserver(&supplierNotifier);
    inventoryManager->addObserver(&dashboardAlert);

    // ========== CREATE WAREHOUSES ==========
    std::cout << "\n--- Creating Warehouses ---" << std::endl;
    Warehouse warehouse1("Warehouse 1");
    Warehouse warehouse2("Warehouse 2");
    inventoryManager->addWarehouse(&warehouse1);
    inventoryManager->addWarehouse(&warehouse2);

    // ========== FACTORY PATTERN ==========
    std::cout << "\n--- Creating Products using Factory Pattern ---" << std::endl;
    ProductFactory& productFactory = inventoryManager->getProductFactory();
    
    Product* laptop = productFactory.createProduct(
        ProductCategory::ELECTRONICS, "SKU123", "Laptop", 1000.0, 50, 25);
    
    Product* tShirt = productFactory.createProduct(
        ProductCategory::CLOTHING, "SKU456", "T-Shirt", 20.0, 200, 100);
    
    Product* apple = productFactory.createProduct(
        ProductCategory::GROCERY, "SKU789", "Apple", 1.0, 100, 200);

    // Add products to warehouses (will trigger observers for low stock)
    std::cout << "\n--- Adding Products to Warehouses ---" << std::endl;
    warehouse1.addProduct(laptop, 15);      // Below threshold (25) - will trigger observers
    warehouse1.addProduct(tShirt, 20);      // Below threshold (100) - will trigger observers
    warehouse2.addProduct(apple, 50);       // Below threshold (200) - will trigger observers

    // ========== STRATEGY PATTERN ==========
    std::cout << "\n--- Setting Just-In-Time Strategy ---" << std::endl;
    JustInTimeStrategy jitStrategy;
    inventoryManager->setReplenishmentStrategy(&jitStrategy);

    // ========== PERFORM INVENTORY CHECK ==========
    std::cout << "\n--- Performing Inventory Check (Observers + Strategy) ---" << std::endl;
    inventoryManager->performInventoryCheck();

    // ========== SWITCH STRATEGY ==========
    std::cout << "\n--- Switching to Bulk Order Strategy ---" << std::endl;
    BulkOrderStrategy bulkStrategy;
    inventoryManager->setReplenishmentStrategy(&bulkStrategy);

    std::cout << "\n--- Checking Specific Product: SKU123 ---" << std::endl;
    inventoryManager->checkAndReplenish("SKU123");

    std::cout << "\n============================================" << std::endl;
    std::cout << "              DEMO COMPLETE" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "\nDesign Patterns Demonstrated:" << std::endl;
    std::cout << "1. Singleton Pattern - InventoryManager" << std::endl;
    std::cout << "2. Factory Pattern - ProductFactory" << std::endl;
    std::cout << "3. Observer Pattern - SupplierNotifier, DashboardAlertSystem" << std::endl;
    std::cout << "4. Strategy Pattern - JustInTimeStrategy, BulkOrderStrategy" << std::endl;
    std::cout << "============================================" << std::endl;

    return 0;
}
