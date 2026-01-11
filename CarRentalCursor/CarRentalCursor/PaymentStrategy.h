#ifndef PAYMENT_STRATEGY_H
#define PAYMENT_STRATEGY_H

#include "VehicleTypes.h"
#include <string>
#include <iostream>

// Strategy Pattern for Payment Processing
// Enables different payment strategies based on payment type
// Can switch between different payment strategies dynamically

// Abstract Strategy Interface
class PaymentStrategy {
protected:
    PaymentStatus status;
    double amount;
    std::string transactionId;

public:
    PaymentStrategy() : status(PaymentStatus::PENDING), amount(0.0) {}
    virtual ~PaymentStrategy() = default;

    // Core strategy method - must be implemented by concrete strategies
    virtual bool processPayment(double amt, const std::string& details) = 0;
    
    // Common methods
    virtual std::string getPaymentType() const = 0;
    virtual bool validatePayment() const = 0;
    
    PaymentStatus getStatus() const { return status; }
    std::string getTransactionId() const { return transactionId; }
    double getAmount() const { return amount; }
};

// Concrete Strategy: Credit Card Payment
class CreditCardPayment : public PaymentStrategy {
private:
    std::string cardNumber;
    std::string cvv;
    std::string expiryDate;
    std::string cardHolderName;

public:
    CreditCardPayment(const std::string& cardNum, const std::string& cvv,
                     const std::string& expiry, const std::string& name)
        : cardNumber(cardNum), cvv(cvv), expiryDate(expiry), cardHolderName(name) {}

    bool processPayment(double amt, const std::string& details) override {
        if (!validatePayment()) {
            status = PaymentStatus::FAILED;
            return false;
        }

        amount = amt;
        status = PaymentStatus::PROCESSING;
        
        // Simulate payment processing
        std::cout << "   Processing Credit Card payment of Rs." << amt << "...\n";
        std::cout << "   Card: **** **** **** " << cardNumber.substr(cardNumber.length() - 4) << "\n";
        
        // Generate transaction ID
        transactionId = "CC-" + details + "-" + std::to_string(rand() % 10000);
        
        status = PaymentStatus::COMPLETED;
        std::cout << "   Transaction ID: " << transactionId << "\n";
        return true;
    }

    std::string getPaymentType() const override {
        return "Credit Card";
    }

    bool validatePayment() const override {
        // Basic validation
        return cardNumber.length() >= 13 && cardNumber.length() <= 19 &&
               cvv.length() == 3 && !expiryDate.empty();
    }
};

// Concrete Strategy: Debit Card Payment
class DebitCardPayment : public PaymentStrategy {
private:
    std::string cardNumber;
    std::string pin;
    std::string bankName;

public:
    DebitCardPayment(const std::string& cardNum, const std::string& pin,
                    const std::string& bank)
        : cardNumber(cardNum), pin(pin), bankName(bank) {}

    bool processPayment(double amt, const std::string& details) override {
        if (!validatePayment()) {
            status = PaymentStatus::FAILED;
            return false;
        }

        amount = amt;
        status = PaymentStatus::PROCESSING;
        
        std::cout << "   Processing Debit Card payment of Rs." << amt << "...\n";
        std::cout << "   Bank: " << bankName << "\n";
        
        transactionId = "DC-" + details + "-" + std::to_string(rand() % 10000);
        
        status = PaymentStatus::COMPLETED;
        std::cout << "   Transaction ID: " << transactionId << "\n";
        return true;
    }

    std::string getPaymentType() const override {
        return "Debit Card";
    }

    bool validatePayment() const override {
        return cardNumber.length() >= 13 && pin.length() == 4;
    }
};

// Concrete Strategy: Cash Payment
class CashPayment : public PaymentStrategy {
private:
    std::string receiptNumber;
    std::string collectorName;

public:
    CashPayment(const std::string& collector = "Counter")
        : collectorName(collector) {}

    bool processPayment(double amt, const std::string& details) override {
        amount = amt;
        status = PaymentStatus::PROCESSING;
        
        std::cout << "   Processing Cash payment of Rs." << amt << "...\n";
        std::cout << "   Collected by: " << collectorName << "\n";
        
        transactionId = "CASH-" + details + "-" + std::to_string(rand() % 10000);
        receiptNumber = "RCP-" + std::to_string(rand() % 100000);
        
        status = PaymentStatus::COMPLETED;
        std::cout << "   Receipt Number: " << receiptNumber << "\n";
        std::cout << "   Transaction ID: " << transactionId << "\n";
        return true;
    }

    std::string getPaymentType() const override {
        return "Cash";
    }

    bool validatePayment() const override {
        return true; // Cash is always valid if physically received
    }

    std::string getReceiptNumber() const { return receiptNumber; }
};

// Concrete Strategy: UPI Payment
class UPIPayment : public PaymentStrategy {
private:
    std::string upiId;
    std::string appName;

public:
    UPIPayment(const std::string& upi, const std::string& app = "UPI")
        : upiId(upi), appName(app) {}

    bool processPayment(double amt, const std::string& details) override {
        if (!validatePayment()) {
            status = PaymentStatus::FAILED;
            return false;
        }

        amount = amt;
        status = PaymentStatus::PROCESSING;
        
        std::cout << "   Processing UPI payment of Rs." << amt << "...\n";
        std::cout << "   UPI ID: " << upiId << "\n";
        std::cout << "   App: " << appName << "\n";
        
        transactionId = "UPI-" + details + "-" + std::to_string(rand() % 10000);
        
        status = PaymentStatus::COMPLETED;
        std::cout << "   Transaction ID: " << transactionId << "\n";
        return true;
    }

    std::string getPaymentType() const override {
        return "UPI";
    }

    bool validatePayment() const override {
        return upiId.find('@') != std::string::npos; // Basic UPI ID validation
    }
};

// Concrete Strategy: Net Banking Payment
class NetBankingPayment : public PaymentStrategy {
private:
    std::string bankName;
    std::string accountNumber;
    std::string ifscCode;

public:
    NetBankingPayment(const std::string& bank, const std::string& account,
                     const std::string& ifsc)
        : bankName(bank), accountNumber(account), ifscCode(ifsc) {}

    bool processPayment(double amt, const std::string& details) override {
        if (!validatePayment()) {
            status = PaymentStatus::FAILED;
            return false;
        }

        amount = amt;
        status = PaymentStatus::PROCESSING;
        
        std::cout << "   Processing Net Banking payment of Rs." << amt << "...\n";
        std::cout << "   Bank: " << bankName << "\n";
        std::cout << "   Account: **** **** " << accountNumber.substr(accountNumber.length() - 4) << "\n";
        
        transactionId = "NB-" + details + "-" + std::to_string(rand() % 10000);
        
        status = PaymentStatus::COMPLETED;
        std::cout << "   Transaction ID: " << transactionId << "\n";
        return true;
    }

    std::string getPaymentType() const override {
        return "Net Banking";
    }

    bool validatePayment() const override {
        return !bankName.empty() && accountNumber.length() >= 8 && ifscCode.length() == 11;
    }
};

// Payment Context - Uses the strategy pattern
class Payment {
private:
    std::string paymentId;
    std::unique_ptr<PaymentStrategy> strategy;
    std::string reservationId;

public:
    Payment(const std::string& id, const std::string& resId,
            std::unique_ptr<PaymentStrategy> paymentStrategy)
        : paymentId(id), reservationId(resId), strategy(std::move(paymentStrategy)) {}

    // Process payment using the selected strategy
    bool process(double amount) {
        if (!strategy) return false;
        
        std::cout << "\n   Payment Method: " << strategy->getPaymentType() << "\n";
        return strategy->processPayment(amount, reservationId);
    }

    // Change payment strategy dynamically
    void setStrategy(std::unique_ptr<PaymentStrategy> newStrategy) {
        strategy = std::move(newStrategy);
    }

    PaymentStatus getStatus() const {
        return strategy ? strategy->getStatus() : PaymentStatus::FAILED;
    }

    std::string getTransactionId() const {
        return strategy ? strategy->getTransactionId() : "";
    }

    std::string getPaymentType() const {
        return strategy ? strategy->getPaymentType() : "Unknown";
    }

    std::string getPaymentId() const { return paymentId; }
};

#endif

