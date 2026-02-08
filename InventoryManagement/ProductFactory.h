#ifndef PRODUCTFACTORY_H
#define PRODUCTFACTORY_H

#include "Product.h"
#include <stdexcept>

// Product Factory class implementing Factory Pattern (from Java: ProductFactory.java)
class ProductFactory {
public:
    Product* createProduct(ProductCategory category, 
                           const std::string& sku, 
                           const std::string& name, 
                           double price, 
                           int quantity, 
                           int threshold) {
        switch (category) {
            case ProductCategory::ELECTRONICS:
                return new ElectronicsProduct(sku, name, price, quantity, threshold);
            case ProductCategory::CLOTHING:
                return new ClothingProduct(sku, name, price, quantity, threshold);
            case ProductCategory::GROCERY:
                return new GroceryProduct(sku, name, price, quantity, threshold);
            default:
                throw std::invalid_argument("Unsupported product category");
        }
    }
};

#endif // PRODUCTFACTORY_H
