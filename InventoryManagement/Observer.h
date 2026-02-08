#ifndef OBSERVER_H
#define OBSERVER_H

#include "Product.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

// InventoryObserver interface (from Java: InventoryObserver.java)
class InventoryObserver {
public:
    virtual ~InventoryObserver() = default;
    virtual void update(Product* product) = 0;
};

// ============== Concrete Observers ==============

// SupplierNotifier.java equivalent
// Used to notify the suppliers regarding inventory changes
class SupplierNotifier : public InventoryObserver {
private:
    std::string supplierName;
    std::string contactEmail;

public:
    SupplierNotifier(const std::string& supplierName, const std::string& contactEmail)
        : supplierName(supplierName), contactEmail(contactEmail) {}

    void update(Product* product) override {
        if (product->getQuantity() < product->getThreshold()) {
            // Send email notification to supplier
            std::cout << "Notification sent to " << supplierName
                      << " for low stock of " << product->getName() << std::endl;
        }
    }
};

// DashboardAlertSystem.java equivalent
// Second concrete observer - Dashboard alert system
class DashboardAlertSystem : public InventoryObserver {
private:
    std::string alertLevel;
    std::vector<std::string> adminUsers;

public:
    DashboardAlertSystem(const std::string& alertLevel, 
                         const std::vector<std::string>& adminUsers)
        : alertLevel(alertLevel), adminUsers(adminUsers) {}

    void update(Product* product) override {
        double stockPercentage = 
            (static_cast<double>(product->getQuantity()) / product->getThreshold()) * 100;

        if (stockPercentage <= 25) {
            // Critical alert - red notification
            std::cout << "CRITICAL ALERT: " << product->getName()
                      << " stock critically low at " << product->getQuantity() << " units ("
                      << std::fixed << std::setprecision(1) << stockPercentage 
                      << "% of threshold)" << std::endl;
            notifyAdmins(product, "CRITICAL");
        } else if (stockPercentage <= 50) {
            // Warning alert - yellow notification
            std::cout << "WARNING ALERT: " << product->getName()
                      << " stock low at " << product->getQuantity() << " units ("
                      << std::fixed << std::setprecision(1) << stockPercentage 
                      << "% of threshold)" << std::endl;
            notifyAdmins(product, "WARNING");
        }
    }

private:
    void notifyAdmins(Product* product, const std::string& level) {
        for (const auto& admin : adminUsers) {
            std::cout << "Dashboard notification sent to admin: " << admin
                      << " - " << level << " level alert for " 
                      << product->getName() << std::endl;
            // Actual implementation would update dashboard UI and push notifications
        }
    }
};

#endif // OBSERVER_H
