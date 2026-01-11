#pragma once
#include <map>
#include <vector>
#include <memory>
#include <atomic>
#include <mutex>
#include "../models/Booking.h"
#include "../models/Show.h"
#include "../models/Seat.h"
#include "../models/User.h"
#include "../strategies/ISeatLockStrategy.h"

// Merged BookingController + BookingService + SeatAvailabilityService
class BookingManager {
private:
    std::map<std::string, std::shared_ptr<Booking>> bookings;
    std::shared_ptr<ISeatLockStrategy> lockStrategy;
    std::atomic<int> bookingCounter;
    std::mutex bookingMutex;
    
    bool isAnySeatAlreadyBooked(std::shared_ptr<Show> show, 
                                const std::vector<std::shared_ptr<Seat>>& seats);

public:
    explicit BookingManager(std::shared_ptr<ISeatLockStrategy> strategy);
    
    // Booking operations
    std::string createBooking(const User& user, std::shared_ptr<Show> show,
                             const std::vector<std::shared_ptr<Seat>>& seats);
    std::shared_ptr<Booking> getBooking(const std::string& bookingId);
    void confirmBooking(const std::string& bookingId, const User& user);
    
    // Seat availability
    std::vector<std::shared_ptr<Seat>> getAvailableSeats(std::shared_ptr<Show> show);
    std::vector<std::shared_ptr<Seat>> getBookedSeats(std::shared_ptr<Show> show);
};

