#include "Card.h"

Card::Card(const std::string& cardNum, const std::string& pinCode, Account* acc)
    : cardNumber(cardNum), pin(pinCode), account(acc) {}

std::string Card::getCardNumber() const {
    return cardNumber;
}

bool Card::validatePin(const std::string& enteredPin) const {
    return pin == enteredPin;
}

Account* Card::getAccount() const {
    return account;
}

