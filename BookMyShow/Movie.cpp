#include "../../include/models/Movie.h"

Movie::Movie(int id, const std::string& name, int duration)
    : movieId(id), movieName(name), movieDurationInMinutes(duration) {}

int Movie::getMovieId() const {
    return movieId;
}

std::string Movie::getMovieName() const {
    return movieName;
}

int Movie::getMovieDuration() const {
    return movieDurationInMinutes;
}

