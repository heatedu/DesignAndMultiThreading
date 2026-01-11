#pragma once
#include <map>
#include <memory>
#include <atomic>
#include <mutex>
#include "../models/Theatre.h"
#include "../models/Screen.h"
#include "../models/Seat.h"
#include "../enums/SeatCategory.h"

// Merged TheatreController + TheatreService
class TheatreManager {
private:
    std::map<int, std::shared_ptr<Theatre>> theatres;
    std::map<int, std::shared_ptr<Screen>> screens;
    std::map<int, std::shared_ptr<Seat>> seats;
    
    std::atomic<int> theatreCounter;
    std::atomic<int> screenCounter;
    std::atomic<int> seatCounter;
    
    std::mutex theatreMutex;

public:
    TheatreManager();
    
    // Theatre operations
    int createTheatre(const std::string& name);
    std::shared_ptr<Theatre> getTheatre(int theatreId);
    
    // Screen operations
    int createScreen(const std::string& name, int theatreId);
    std::shared_ptr<Screen> getScreen(int screenId);
    
    // Seat operations
    int createSeat(int rowNo, SeatCategory category, int screenId);
    std::shared_ptr<Seat> getSeat(int seatId);
};

