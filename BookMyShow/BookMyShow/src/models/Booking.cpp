#include "../../include/models/Booking.h"
#include <stdexcept>

Booking::Booking(const std::string& bookingId, std::shared_ptr<Show> showPtr,
                 const User& bookingUser, const std::vector<std::shared_ptr<Seat>>& seats)
    : id(bookingId), show(showPtr), user(bookingUser), seatsBooked(seats), 
      bookingStatus(BookingStatus::CREATED) {}

bool Booking::isConfirmed() const {
    return bookingStatus == BookingStatus::CONFIRMED;
}

void Booking::confirmBooking() {
    if (bookingStatus != BookingStatus::CREATED) {
        throw std::runtime_error("Cannot confirm a booking that is not in the Created state.");
    }
    bookingStatus = BookingStatus::CONFIRMED;
}

void Booking::expireBooking() {
    if (bookingStatus != BookingStatus::CREATED) {
        throw std::runtime_error("Cannot expire a booking that is not in the Created state.");
    }
    bookingStatus = BookingStatus::EXPIRED;
}

std::string Booking::getId() const {
    return id;
}

std::shared_ptr<Show> Booking::getShow() const {
    return show;
}

std::vector<std::shared_ptr<Seat>> Booking::getSeatsBooked() const {
    return seatsBooked;
}

User Booking::getUser() const {
    return user;
}

BookingStatus Booking::getBookingStatus() const {
    return bookingStatus;
}

