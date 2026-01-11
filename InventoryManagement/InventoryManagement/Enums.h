#ifndef ENUMS_H
#define ENUMS_H

#include <string>

// Enum for product categories (from Java: ProductCategory.java)
enum class ProductCategory {
    ELECTRONICS,
    CLOTHING,
    GROCERY,
    FURNITURE,
    OTHER
};

enum class InventoryOperation {
    ADD, 
    REMOVE, 
    TRANSFER, 
    ADJUST
};

inline std::string categoryToString(ProductCategory category) {
    switch (category) {
        case ProductCategory::ELECTRONICS: return "ELECTRONICS";
        case ProductCategory::CLOTHING: return "CLOTHING";
        case ProductCategory::GROCERY: return "GROCERY";
        case ProductCategory::FURNITURE: return "FURNITURE";
        case ProductCategory::OTHER: return "OTHER";
        default: return "UNKNOWN";
    }
}

#endif // ENUMS_H
