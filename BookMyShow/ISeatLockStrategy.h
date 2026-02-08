#pragma once
#include <vector>
#include <memory>
#include "../models/Show.h"
#include "../models/Seat.h"
#include "../models/User.h"

// Strategy Pattern for seat locking
class ISeatLockStrategy {
public:
    virtual ~ISeatLockStrategy() = default;
    
    virtual void lockSeats(std::shared_ptr<Show> show, 
                          const std::vector<std::shared_ptr<Seat>>& seats, 
                          const User& user) = 0;
    virtual void unlockSeats(std::shared_ptr<Show> show, 
                            const std::vector<std::shared_ptr<Seat>>& seats, 
                            const User& user) = 0;
    virtual bool validateLock(std::shared_ptr<Show> show, 
                             std::shared_ptr<Seat> seat, 
                             const User& user) = 0;
    virtual std::vector<std::shared_ptr<Seat>> getLockedSeats(std::shared_ptr<Show> show) = 0;
};

