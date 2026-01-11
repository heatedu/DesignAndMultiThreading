#include "CashDispenser.h"
#include <iostream>

CashDispenser::CashDispenser() {
    // Initialize with common denominations
    cashInventory[100] = 50;
    cashInventory[50] = 100;
    cashInventory[20] = 100;
    cashInventory[10] = 100;
}

void CashDispenser::initializeCash(int denomination, int count) {
    cashInventory[denomination] = count;
}

bool CashDispenser::dispenseCash(double amount) {
    std::map<int, int> toDispense;
    double remaining = amount;
    
    // Greedy approach: use largest denominations first
    for (auto it = cashInventory.rbegin(); it != cashInventory.rend(); ++it) {
        int denom = it->first;
        int available = it->second;
        int needed = static_cast<int>(remaining / denom);
        
        if (needed > 0 && available > 0) {
            int dispense = std::min(needed, available);
            toDispense[denom] = dispense;
            remaining -= dispense * denom;
        }
    }
    
    if (remaining > 0.01) { // Account for floating point precision
        std::cout << "Cannot dispense exact amount!\n";
        return false;
    }
    
    // Deduct from inventory
    for (const auto& pair : toDispense) {
        cashInventory[pair.first] -= pair.second;
        std::cout << "Dispensing: " << pair.second << " x $" << pair.first << "\n";
    }
    
    return true;
}

double CashDispenser::getTotalCash() const {
    double total = 0;
    for (const auto& pair : cashInventory) {
        total += pair.first * pair.second;
    }
    return total;
}

void CashDispenser::displayInventory() const {
    std::cout << "\n--- Cash Inventory ---\n";
    for (const auto& pair : cashInventory) {
        std::cout << "$" << pair.first << ": " << pair.second << " notes\n";
    }
    std::cout << "Total: $" << getTotalCash() << "\n";
}

