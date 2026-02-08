#pragma once
#include <memory>
#include <chrono>
#include "Seat.h"
#include "Show.h"
#include "User.h"

class SeatLock {
private:
    std::shared_ptr<Seat> seat;
    std::shared_ptr<Show> show;
    int timeoutInSeconds;
    std::chrono::system_clock::time_point lockTime;
    User lockedBy;

public:
    SeatLock(std::shared_ptr<Seat> seatPtr, std::shared_ptr<Show> showPtr,
             int timeout, std::chrono::system_clock::time_point time, const User& user);
    
    bool isLockExpired() const;
    
    std::shared_ptr<Seat> getSeat() const;
    std::shared_ptr<Show> getShow() const;
    int getTimeoutInSeconds() const;
    std::chrono::system_clock::time_point getLockTime() const;
    User getLockedBy() const;
};

