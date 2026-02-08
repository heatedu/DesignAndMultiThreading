#pragma once
#include "ISeatLockStrategy.h"
#include "../models/SeatLock.h"
#include <map>
#include <mutex>

// Concrete strategy for in-memory locking

/*Expired locks remain in the locks map but are ignored during all operations.No background thread or timer cleans them up.This is a simple, common approach - the trade - off is memory isn't immediately freed, but you avoid the complexity of timer threads.*/

class InMemoryLockStrategy : public ISeatLockStrategy {
private:
    int lockTimeout;
    std::map<int, std::map<int, SeatLock>> locks; // showId -> (seatId -> SeatLock)
    std::mutex locksMutex;

public:
    explicit InMemoryLockStrategy(int timeout);
    
    void lockSeats(std::shared_ptr<Show> show, 
                  const std::vector<std::shared_ptr<Seat>>& seats, 
                  const User& user) override;
    void unlockSeats(std::shared_ptr<Show> show, 
                    const std::vector<std::shared_ptr<Seat>>& seats, 
                    const User& user) override;
    bool validateLock(std::shared_ptr<Show> show, 
                     std::shared_ptr<Seat> seat, 
                     const User& user) override;
    std::vector<std::shared_ptr<Seat>> getLockedSeats(std::shared_ptr<Show> show) override;
};

