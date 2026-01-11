#pragma once
#include <map>

class CashDispenser {
private:
    std::map<int, int> cashInventory; // denomination -> count

public:
    CashDispenser();
    
    void initializeCash(int denomination, int count);
    bool dispenseCash(double amount);
    double getTotalCash() const;
    void displayInventory() const;
};

