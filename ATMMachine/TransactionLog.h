#pragma once
#include <string>
#include <vector>
#include <ctime>

struct Transaction {
    std::string cardNumber;
    std::string type;
    double amount;
    time_t timestamp;
    bool success;
};

class TransactionLog {
private:
    std::vector<Transaction> logs;

public:
    void logTransaction(const std::string& cardNum, const std::string& type, 
                       double amount, bool success);
    void displayLogs() const;
};

