#ifndef RESERVATION_H
#define RESERVATION_H

#include "VehicleTypes.h"
#include "PaymentStrategy.h"
#include <string>
#include <ctime>
#include <memory>

// Reservation with proper state management using enums
// Manages the lifecycle: PENDING -> CONFIRMED -> IN_PROGRESS -> COMPLETED
class Reservation {
private:
    std::string reservationId;
    std::string userId;
    std::string vehicleRegNumber;
    time_t startTime;
    time_t endTime;
    time_t createdAt;
    time_t pickupTime;
    time_t returnTime;
    ReservationStatus status;
    double totalCost;
    std::unique_ptr<Payment> payment;

public:
    Reservation(const std::string& resId, const std::string& userId,
        const std::string& vehicleReg, time_t start, time_t end)
        : reservationId(resId), userId(userId), vehicleRegNumber(vehicleReg),
        startTime(start), endTime(end), status(ReservationStatus::PENDING),
        totalCost(0), createdAt(time(nullptr)), pickupTime(0), returnTime(0) {}

    // Conflict detection - KEY LOGIC
    bool hasConflict(time_t start, time_t end) const {
        // Cancelled and No-show reservations don't cause conflicts
        if (status == ReservationStatus::CANCELLED || 
            status == ReservationStatus::NO_SHOW) {
            return false;
        }
        // Completed reservations only conflict if times overlap
        return !(end <= startTime || start >= endTime);
    }

    // State transition methods with proper lifecycle management
    bool confirm() {
        if (status == ReservationStatus::PENDING) {
            status = ReservationStatus::CONFIRMED;
            return true;
        }
        return false;
    }

    bool startRental() {
        if (status == ReservationStatus::CONFIRMED) {
            status = ReservationStatus::IN_PROGRESS;
            pickupTime = time(nullptr);
            return true;
        }
        return false;
    }

    bool complete() {
        if (status == ReservationStatus::IN_PROGRESS) {
            status = ReservationStatus::COMPLETED;
            returnTime = time(nullptr);
            return true;
        }
        return false;
    }

    bool cancel() {
        if (status == ReservationStatus::PENDING || 
            status == ReservationStatus::CONFIRMED) {
            status = ReservationStatus::CANCELLED;
            return true;
        }
        return false;
    }

    bool markNoShow() {
        if (status == ReservationStatus::CONFIRMED) {
            status = ReservationStatus::NO_SHOW;
            return true;
        }
        return false;
    }

    // Payment integration with Strategy pattern
    void setPayment(std::unique_ptr<Payment> pmt) {
        payment = std::move(pmt);
    }

    bool processPayment() {
        if (!payment) return false;
        
        bool success = payment->process(totalCost);
        if (success) {
            confirm(); // Move from PENDING to CONFIRMED after successful payment
        }
        return success;
    }

    // Getters
    std::string getReservationId() const { return reservationId; }
    std::string getVehicleRegNumber() const { return vehicleRegNumber; }
    std::string getUserId() const { return userId; }
    ReservationStatus getStatus() const { return status; }
    time_t getStartTime() const { return startTime; }
    time_t getEndTime() const { return endTime; }
    double getTotalCost() const { return totalCost; }
    
    Payment* getPayment() const { return payment.get(); }
    std::string getPaymentType() const {
        return payment ? payment->getPaymentType() : "None";
    }

    std::string getStatusString() const {
        switch (status) {
            case ReservationStatus::PENDING: return "Pending";
            case ReservationStatus::CONFIRMED: return "Confirmed";
            case ReservationStatus::IN_PROGRESS: return "In Progress";
            case ReservationStatus::COMPLETED: return "Completed";
            case ReservationStatus::CANCELLED: return "Cancelled";
            case ReservationStatus::NO_SHOW: return "No Show";
            default: return "Unknown";
        }
    }

    void setTotalCost(double cost) { totalCost = cost; }
};

#endif

