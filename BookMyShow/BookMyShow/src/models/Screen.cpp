#include "../../include/models/Screen.h"

Screen::Screen(int screenId, const std::string& screenName, std::shared_ptr<Theatre> theatrePtr)
    : id(screenId), name(screenName), theatre(theatrePtr) {}

void Screen::addSeat(std::shared_ptr<Seat> seat) {
    seats.push_back(seat);
}

int Screen::getScreenId() const {
    return id;
}

std::vector<std::shared_ptr<Seat>> Screen::getSeats() const {
    return seats;
}

std::shared_ptr<Theatre> Screen::getTheatre() const {
    return theatre;
}

