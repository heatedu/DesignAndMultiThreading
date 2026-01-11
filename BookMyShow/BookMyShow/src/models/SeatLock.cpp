#include "../../include/models/SeatLock.h"

SeatLock::SeatLock(std::shared_ptr<Seat> seatPtr, std::shared_ptr<Show> showPtr,
                   int timeout, std::chrono::system_clock::time_point time, const User& user)
    : seat(seatPtr), show(showPtr), timeoutInSeconds(timeout), lockTime(time), lockedBy(user) {}

bool SeatLock::isLockExpired() const {
    auto expirationTime = lockTime + std::chrono::seconds(timeoutInSeconds);
    auto currentTime = std::chrono::system_clock::now();
    return currentTime >= expirationTime;
}

std::shared_ptr<Seat> SeatLock::getSeat() const {
    return seat;
}

std::shared_ptr<Show> SeatLock::getShow() const {
    return show;
}

int SeatLock::getTimeoutInSeconds() const {
    return timeoutInSeconds;
}

std::chrono::system_clock::time_point SeatLock::getLockTime() const {
    return lockTime;
}

User SeatLock::getLockedBy() const {
    return lockedBy;
}

