#include "../../include/managers/TheatreManager.h"
#include <stdexcept>

TheatreManager::TheatreManager() 
    : theatreCounter(0), screenCounter(0), seatCounter(0) {}

int TheatreManager::createTheatre(const std::string& name) {
    std::lock_guard<std::mutex> lock(theatreMutex);
    
    int id = ++theatreCounter;
    auto theatre = std::make_shared<Theatre>(id, name);
    theatres[id] = theatre;
    return id;
}

std::shared_ptr<Theatre> TheatreManager::getTheatre(int theatreId) {
    auto it = theatres.find(theatreId);
    if (it == theatres.end()) {
        throw std::runtime_error("Theatre not found");
    }
    return it->second;
}

int TheatreManager::createScreen(const std::string& name, int theatreId) {
    std::lock_guard<std::mutex> lock(theatreMutex);
    
    auto theatre = getTheatre(theatreId);
    int id = ++screenCounter;
    auto screen = std::make_shared<Screen>(id, name, theatre);
    screens[id] = screen;
    theatre->addScreen(screen);
    return id;
}

std::shared_ptr<Screen> TheatreManager::getScreen(int screenId) {
    auto it = screens.find(screenId);
    if (it == screens.end()) {
        throw std::runtime_error("Screen not found");
    }
    return it->second;
}

int TheatreManager::createSeat(int rowNo, SeatCategory category, int screenId) {
    std::lock_guard<std::mutex> lock(theatreMutex);
    
    auto screen = getScreen(screenId);
    int id = ++seatCounter;
    auto seat = std::make_shared<Seat>(id, rowNo, category);
    seats[id] = seat;
    screen->addSeat(seat);
    return id;
}

std::shared_ptr<Seat> TheatreManager::getSeat(int seatId) {
    auto it = seats.find(seatId);
    if (it == seats.end()) {
        throw std::runtime_error("Seat not found");
    }
    return it->second;
}

