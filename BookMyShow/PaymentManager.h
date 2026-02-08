#pragma once
#include <map>
#include <memory>
#include <mutex>
#include "../models/User.h"
#include "../strategies/PaymentStrategy.h"
#include "BookingManager.h"

// Merged PaymentController + PaymentService
class PaymentManager {
private:
    std::map<std::string, int> paymentFailures;
    std::shared_ptr<PaymentStrategy> paymentStrategy;
    std::shared_ptr<BookingManager> bookingManager;
    std::mutex paymentMutex;

public:
    PaymentManager(std::shared_ptr<PaymentStrategy> strategy,
                   std::shared_ptr<BookingManager> bookingMgr);
    
    void processPayment(const std::string& bookingId, const User& user);
    void recordPaymentFailure(const std::string& bookingId, const User& user);
};

