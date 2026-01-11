#pragma once
#include <string>
#include "Account.h"

class Card {
private:
    std::string cardNumber;
    std::string pin;
    Account* account;

public:
    Card(const std::string& cardNum, const std::string& pinCode, Account* acc);
    
    std::string getCardNumber() const;
    bool validatePin(const std::string& enteredPin) const;
    Account* getAccount() const;
};

