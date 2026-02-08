#include "../../include/models/Theatre.h"

Theatre::Theatre(int theatreId, const std::string& theatreName)
    : id(theatreId), name(theatreName) {}

void Theatre::addScreen(std::shared_ptr<Screen> screen) {
    screens.push_back(screen);
}

int Theatre::getTheatreId() const {
    return id;
}

std::vector<std::shared_ptr<Screen>> Theatre::getScreens() const {
    return screens;
}

