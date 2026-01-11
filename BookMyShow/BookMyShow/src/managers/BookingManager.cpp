#include "../../include/managers/BookingManager.h"
#include <stdexcept>
#include <algorithm>

BookingManager::BookingManager(std::shared_ptr<ISeatLockStrategy> strategy)
    : lockStrategy(strategy), bookingCounter(0) {}

std::string BookingManager::createBooking(const User& user, std::shared_ptr<Show> show,
                                          const std::vector<std::shared_ptr<Seat>>& seats) {
    std::lock_guard<std::mutex> lock(bookingMutex);
    
    // Check if seats already booked
    if (isAnySeatAlreadyBooked(show, seats)) {
        throw std::runtime_error("Seat already booked");
    }
    
    // Lock seats
    lockStrategy->lockSeats(show, seats, user);
    
    // Create booking
    std::string id = std::to_string(++bookingCounter);
    auto booking = std::make_shared<Booking>(id, show, user, seats);
    bookings[id] = booking;
    
    return id;
}

std::shared_ptr<Booking> BookingManager::getBooking(const std::string& bookingId) {
    auto it = bookings.find(bookingId);
    if (it == bookings.end()) {
        throw std::runtime_error("Booking not found");
    }
    return it->second;
}

void BookingManager::confirmBooking(const std::string& bookingId, const User& user) {
    std::lock_guard<std::mutex> lock(bookingMutex);
    
    auto booking = getBooking(bookingId);
    
    // Validate user
    if (!(booking->getUser() == user)) {
        throw std::runtime_error("Cannot confirm booking for another user");
    }
    
    // Validate locks
    for (const auto& seat : booking->getSeatsBooked()) {
        if (!lockStrategy->validateLock(booking->getShow(), seat, user)) {
            throw std::runtime_error("Lock expired or invalid");
        }
    }
    
    booking->confirmBooking();
}

std::vector<std::shared_ptr<Seat>> BookingManager::getAvailableSeats(std::shared_ptr<Show> show) {
    auto allSeats = show->getScreen()->getSeats();
    auto bookedSeats = getBookedSeats(show);
    auto lockedSeats = lockStrategy->getLockedSeats(show);
    
    // Combine booked and locked
    std::vector<std::shared_ptr<Seat>> unavailable = bookedSeats;
    unavailable.insert(unavailable.end(), lockedSeats.begin(), lockedSeats.end());
    
    // Remove unavailable from all
    std::vector<std::shared_ptr<Seat>> available;
    for (const auto& seat : allSeats) {
        bool isAvailable = true;
        for (const auto& unavailSeat : unavailable) {
            if (seat->getSeatId() == unavailSeat->getSeatId()) {
                isAvailable = false;
                break;
            }
        }
        if (isAvailable) {
            available.push_back(seat);
        }
    }
    
    return available;
}

std::vector<std::shared_ptr<Seat>> BookingManager::getBookedSeats(std::shared_ptr<Show> show) {
    std::vector<std::shared_ptr<Seat>> bookedSeats;
    
    for (const auto& [id, booking] : bookings) {
        if (*(booking->getShow()) == *show && booking->isConfirmed()) {
            auto seats = booking->getSeatsBooked();
            bookedSeats.insert(bookedSeats.end(), seats.begin(), seats.end());
        }
    }
    
    return bookedSeats;
}

bool BookingManager::isAnySeatAlreadyBooked(std::shared_ptr<Show> show,
                                            const std::vector<std::shared_ptr<Seat>>& seats) {
    auto bookedSeats = getBookedSeats(show);
    
    for (const auto& seat : seats) {
        for (const auto& booked : bookedSeats) {
            if (seat->getSeatId() == booked->getSeatId()) {
                return true;
            }
        }
    }
    
    return false;
}

