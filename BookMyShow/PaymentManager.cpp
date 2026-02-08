#include "../../include/managers/PaymentManager.h"
#include <iostream>
#include <stdexcept>

PaymentManager::PaymentManager(std::shared_ptr<PaymentStrategy> strategy,
                               std::shared_ptr<BookingManager> bookingMgr)
    : paymentStrategy(strategy), bookingManager(bookingMgr) {}

void PaymentManager::processPayment(const std::string& bookingId, const User& user) {
    std::lock_guard<std::mutex> lock(paymentMutex);
    
    if (paymentStrategy->processPayment()) {
        bookingManager->confirmBooking(bookingId, user);
        std::cout << "Payment successful for booking: " << bookingId << std::endl;
    } else {
        recordPaymentFailure(bookingId, user);
    }
}

void PaymentManager::recordPaymentFailure(const std::string& bookingId, const User& user) {
    auto booking = bookingManager->getBooking(bookingId);
    
    if (!(booking->getUser() == user)) {
        throw std::runtime_error("Only booking owner can report payment failure");
    }
    
    paymentFailures[bookingId]++;
    std::cout << "Payment failed for booking: " << bookingId 
              << " (Attempts: " << paymentFailures[bookingId] << ")" << std::endl;
}

