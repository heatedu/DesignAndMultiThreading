#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../enums/BookingStatus.h"
#include "Show.h"
#include "Seat.h"
#include "User.h"

class Booking {
private:
    std::string id;
    std::shared_ptr<Show> show;
    std::vector<std::shared_ptr<Seat>> seatsBooked;
    User user;
    BookingStatus bookingStatus;

public:
    Booking(const std::string& bookingId, std::shared_ptr<Show> showPtr,
            const User& bookingUser, const std::vector<std::shared_ptr<Seat>>& seats);
    
    bool isConfirmed() const;
    void confirmBooking();
    void expireBooking();
    
    std::string getId() const;
    std::shared_ptr<Show> getShow() const;
    std::vector<std::shared_ptr<Seat>> getSeatsBooked() const;
    User getUser() const;
    BookingStatus getBookingStatus() const;
};

