#include "TransactionLog.h"
#include <iostream>
#include <iomanip>

void TransactionLog::logTransaction(const std::string& cardNum, const std::string& type, 
                                   double amount, bool success) {
    Transaction t;
    t.cardNumber = cardNum;
    t.type = type;
    t.amount = amount;
    t.timestamp = time(nullptr);
    t.success = success;
    logs.push_back(t);
}

void TransactionLog::displayLogs() const {
    std::cout << "\n--- Transaction Logs ---\n";
    for (const auto& log : logs) {
        std::cout << "Card: " << log.cardNumber 
                  << " | Type: " << log.type 
                  << " | Amount: $" << log.amount 
                  << " | Status: " << (log.success ? "SUCCESS" : "FAILED") 
                  << "\n";
    }
}

