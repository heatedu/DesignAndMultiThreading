#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include "Product.h"
#include <map>
#include <vector>
#include <iostream>

// Warehouse class (from Java: Warehouse.java)
class Warehouse {
private:
    int id;
    std::string name;
    std::string location;
    std::map<std::string, Product*> products;  // SKU -> Product

public:
    Warehouse(int id, const std::string& name, const std::string& location)
        : id(id), name(name), location(location) {}

    // Convenience constructor (matches Java: Warehouse("Warehouse 1"))
    Warehouse(const std::string& name) 
        : id(0), name(name), location("") {}

    ~Warehouse() {
        for (auto& pair : products) {
            delete pair.second;
        }
    }

    // Getters and setters
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getLocation() const { return location; }
    void setLocation(const std::string& loc) { location = loc; }

    // Add a product to the warehouse (from Java: addProduct)
    void addProduct(Product* product, int quantity) {
        std::string sku = product->getSku();
        
        if (products.find(sku) != products.end()) {
            // Product exists, update quantity
            Product* existingProduct = products[sku];
            existingProduct->addStock(quantity);
            delete product;  // Don't need the new one
        } else {
            // New product, add to inventory
            product->setQuantity(quantity);
            products[sku] = product;
        }
        
        std::cout << quantity << " units of " << products[sku]->getName()
                  << " (SKU: " << sku << ") added to " << name
                  << ". New quantity: " << getAvailableQuantity(sku) << std::endl;
    }

    // Remove a product from the warehouse (from Java: removeProduct)
    bool removeProduct(const std::string& sku, int quantity) {
        auto it = products.find(sku);
        
        if (it != products.end()) {
            Product* product = it->second;
            int currentQuantity = product->getQuantity();
            
            if (currentQuantity >= quantity) {
                // Sufficient inventory to remove
                product->removeStock(quantity);
                std::cout << quantity << " units of " << product->getName()
                          << " (SKU: " << sku << ") removed from " << name
                          << ". Remaining quantity: " << product->getQuantity() << std::endl;
                
                // If quantity becomes zero, consider removing the product entirely
                if (product->getQuantity() == 0) {
                    // Remove products with zero quantity
                    delete product;
                    products.erase(it);
                    std::cout << "Product removed from inventory as quantity is now zero." << std::endl;
                }
                return true;
            } else {
                std::cout << "Error: Insufficient inventory. Requested: "
                          << quantity << ", Available: " << currentQuantity << std::endl;
                return false;
            }
        } else {
            std::cout << "Error: Product with SKU " << sku << " not found in " << name << std::endl;
            return false;
        }
    }

    // Get available quantity of a product
    int getAvailableQuantity(const std::string& sku) const {
        auto it = products.find(sku);
        if (it != products.end()) {
            return it->second->getQuantity();
        }
        return 0;  // Product not found
    }

    // Get a product by SKU
    Product* getProductBySku(const std::string& sku) {
        auto it = products.find(sku);
        return it != products.end() ? it->second : nullptr;
    }

    // Get all products in this warehouse
    std::vector<Product*> getAllProducts() {
        std::vector<Product*> result;
        for (auto& pair : products) {
            result.push_back(pair.second);
        }
        return result;
    }
};

#endif // WAREHOUSE_H
