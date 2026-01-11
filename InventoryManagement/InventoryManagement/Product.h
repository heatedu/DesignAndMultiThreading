#ifndef PRODUCT_H
#define PRODUCT_H

#include "Enums.h"
#include <string>
#include <ctime>

// Abstract Product class (from Java: Product.java)
class Product {
private:
    std::string sku;
    std::string name;
    double price;
    int quantity;
    int threshold;
    ProductCategory category;

public:
    Product() : price(0), quantity(0), threshold(0), category(ProductCategory::OTHER) {}
    virtual ~Product() = default;

    // Getters
    std::string getSku() const { return sku; }
    std::string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    int getThreshold() const { return threshold; }
    ProductCategory getCategory() const { return category; }

    // Setters
    void setSku(const std::string& s) { sku = s; }
    void setName(const std::string& n) { name = n; }
    void setPrice(double p) { price = p; }
    void setQuantity(int q) { quantity = q; }
    void setThreshold(int t) { threshold = t; }
    void setCategory(ProductCategory c) { category = c; }

    // Stock operations
    void addStock(int amount) { quantity += amount; }
    void removeStock(int amount) { quantity -= amount; }
};

// ============== Concrete Products ==============

// ElectronicsProduct.java equivalent
class ElectronicsProduct : public Product {
private:
    std::string brand;
    int warrantyPeriod;

public:
    ElectronicsProduct(const std::string& sku, const std::string& name, 
                       double price, int quantity, int threshold)
        : warrantyPeriod(12) {
        setSku(sku);
        setName(name);
        setPrice(price);
        setQuantity(quantity);
        setCategory(ProductCategory::ELECTRONICS);
        setThreshold(threshold);
    }

    // Getters and setters
    std::string getBrand() const { return brand; }
    void setBrand(const std::string& b) { brand = b; }
    
    int getWarrantyPeriod() const { return warrantyPeriod; }
    void setWarrantyPeriod(int w) { warrantyPeriod = w; }
};

// ClothingProduct.java equivalent
class ClothingProduct : public Product {
private:
    std::string size;
    std::string color;

public:
    ClothingProduct(const std::string& sku, const std::string& name,
                    double price, int quantity, int threshold) {
        setSku(sku);
        setName(name);
        setPrice(price);
        setQuantity(quantity);
        setCategory(ProductCategory::CLOTHING);
        setThreshold(threshold);
    }

    // Getters and setters
    std::string getSize() const { return size; }
    void setSize(const std::string& s) { size = s; }
    
    std::string getColor() const { return color; }
    void setColor(const std::string& c) { color = c; }
};

// GroceryProduct.java equivalent
class GroceryProduct : public Product {
private:
    time_t expiryDate;
    bool refrigerated;

public:
    GroceryProduct(const std::string& sku, const std::string& name,
                   double price, int quantity, int threshold)
        : expiryDate(0), refrigerated(false) {
        setSku(sku);
        setName(name);
        setPrice(price);
        setQuantity(quantity);
        setCategory(ProductCategory::GROCERY);
        setThreshold(threshold);
    }

    // Getters and setters
    time_t getExpiryDate() const { return expiryDate; }
    void setExpiryDate(time_t date) { expiryDate = date; }
    
    bool isRefrigerated() const { return refrigerated; }
    void setRefrigerated(bool r) { refrigerated = r; }
};

#endif // PRODUCT_H
