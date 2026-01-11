#include "../../include/models/Show.h"

Show::Show(int showId, std::shared_ptr<Movie> moviePtr, std::shared_ptr<Screen> screenPtr,
           std::chrono::system_clock::time_point start, int duration)
    : id(showId), movie(moviePtr), screen(screenPtr), startTime(start), durationInMinutes(duration) {}

int Show::getId() const {
    return id;
}

std::shared_ptr<Movie> Show::getMovie() const {
    return movie;
}

std::shared_ptr<Screen> Show::getScreen() const {
    return screen;
}

std::chrono::system_clock::time_point Show::getStartTime() const {
    return startTime;
}

int Show::getDurationInMinutes() const {
    return durationInMinutes;
}

bool Show::operator==(const Show& other) const {
    return id == other.id;
}

