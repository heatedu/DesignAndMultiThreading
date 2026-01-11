#include "../../include/strategies/InMemoryLockStrategy.h"
#include <stdexcept>

InMemoryLockStrategy::InMemoryLockStrategy(int timeout) : lockTimeout(timeout) {}

void InMemoryLockStrategy::lockSeats(std::shared_ptr<Show> show,
                                     const std::vector<std::shared_ptr<Seat>>& seats,
                                     const User& user) {
    std::lock_guard<std::mutex> guard(locksMutex);
    
    int showId = show->getId();
    
    if (locks.find(showId) == locks.end()) {
        locks[showId] = std::map<int, SeatLock>();
    }
    auto& seatLocks = locks[showId];
    
    // Check if any seat is already locked
    for (const auto& seat : seats) {
        int seatId = seat->getSeatId();
        auto it = seatLocks.find(seatId);
        if (it != seatLocks.end() && !it->second.isLockExpired()) {
            throw std::runtime_error("Seat " + std::to_string(seatId) + " is already locked");
        }
    }
    
    // Lock all seats
    auto now = std::chrono::system_clock::now();
    for (const auto& seat : seats) {
        SeatLock lock(seat, show, lockTimeout, now, user);
        seatLocks.insert_or_assign(seat->getSeatId(), lock);
    }
}

void InMemoryLockStrategy::unlockSeats(std::shared_ptr<Show> show,
                                       const std::vector<std::shared_ptr<Seat>>& seats,
                                       const User& user) {
    std::lock_guard<std::mutex> guard(locksMutex);
    
    int showId = show->getId();
    if (locks.find(showId) == locks.end()) return;
    
    auto& seatLocks = locks[showId];
    for (const auto& seat : seats) {
        int seatId = seat->getSeatId();
        auto it = seatLocks.find(seatId);
        if (it != seatLocks.end() && it->second.getLockedBy() == user) {
            seatLocks.erase(it);
        }
    }
}

bool InMemoryLockStrategy::validateLock(std::shared_ptr<Show> show,
                                        std::shared_ptr<Seat> seat,
                                        const User& user) {
    std::lock_guard<std::mutex> guard(locksMutex);
    
    int showId = show->getId();
    if (locks.find(showId) == locks.end()) return false;
    
    auto& seatLocks = locks[showId];
    int seatId = seat->getSeatId();
    auto it = seatLocks.find(seatId);
    
    if (it == seatLocks.end()) return false;
    
    const SeatLock& lock = it->second;
    return !lock.isLockExpired() && lock.getLockedBy() == user;
}

std::vector<std::shared_ptr<Seat>> InMemoryLockStrategy::getLockedSeats(std::shared_ptr<Show> show) {
    std::lock_guard<std::mutex> guard(locksMutex);
    
    std::vector<std::shared_ptr<Seat>> result;
    int showId = show->getId();
    
    if (locks.find(showId) == locks.end()) return result;
    
    auto& seatLocks = locks[showId];
    for (const auto& [seatId, lock] : seatLocks) {
        if (!lock.isLockExpired()) {
            result.push_back(lock.getSeat());
        }
    }
    
    return result;
}

