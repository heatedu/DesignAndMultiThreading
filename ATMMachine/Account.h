#pragma once
#include <string>

class Account {
private:
    std::string accountNumber;
    double balance;

public:
    Account(const std::string& accNum, double bal);
    
    std::string getAccountNumber() const;
    double getBalance() const;
    bool withdraw(double amount);
    void deposit(double amount);
};

