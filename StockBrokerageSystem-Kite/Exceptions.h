#pragma once
#include <stdexcept>
#include <string>

class InsufficientFundsException : public std::runtime_error {
public:
    explicit InsufficientFundsException(const std::string& msg)
        : std::runtime_error(msg) {}
};

class InsufficientStockException : public std::runtime_error {
public:
    explicit InsufficientStockException(const std::string& msg)
        : std::runtime_error(msg) {}
};
