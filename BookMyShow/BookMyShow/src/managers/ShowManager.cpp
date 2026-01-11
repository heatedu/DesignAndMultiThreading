#include "../../include/managers/ShowManager.h"
#include <stdexcept>

ShowManager::ShowManager() : movieCounter(0), showCounter(0) {}

int ShowManager::createMovie(const std::string& name, int durationInMinutes) {
    std::lock_guard<std::mutex> lock(showMutex);
    
    int id = ++movieCounter;
    auto movie = std::make_shared<Movie>(id, name, durationInMinutes);
    movies[id] = movie;
    return id;
}

std::shared_ptr<Movie> ShowManager::getMovie(int movieId) {
    auto it = movies.find(movieId);
    if (it == movies.end()) {
        throw std::runtime_error("Movie not found");
    }
    return it->second;
}

int ShowManager::createShow(int movieId, std::shared_ptr<Screen> screen,
                            std::chrono::system_clock::time_point startTime,
                            int durationInMinutes) {
    std::lock_guard<std::mutex> lock(showMutex);
    
    auto movie = getMovie(movieId);
    int id = ++showCounter;
    auto show = std::make_shared<Show>(id, movie, screen, startTime, durationInMinutes);
    shows[id] = show;
    return id;
}

std::shared_ptr<Show> ShowManager::getShow(int showId) {
    auto it = shows.find(showId);
    if (it == shows.end()) {
        throw std::runtime_error("Show not found");
    }
    return it->second;
}

std::vector<std::shared_ptr<Show>> ShowManager::getShowsForScreen(std::shared_ptr<Screen> screen) {
    std::vector<std::shared_ptr<Show>> result;
    for (const auto& [id, show] : shows) {
        if (show->getScreen()->getScreenId() == screen->getScreenId()) {
            result.push_back(show);
        }
    }
    return result;
}

