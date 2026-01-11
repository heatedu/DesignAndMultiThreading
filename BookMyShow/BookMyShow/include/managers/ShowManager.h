#pragma once
#include <map>
#include <vector>
#include <memory>
#include <atomic>
#include <mutex>
#include <chrono>
#include "../models/Show.h"
#include "../models/Movie.h"
#include "../models/Screen.h"

// Merged MovieController + ShowController + MovieService + ShowService
class ShowManager {
private:
    std::map<int, std::shared_ptr<Movie>> movies;
    std::map<int, std::shared_ptr<Show>> shows;
    
    std::atomic<int> movieCounter;
    std::atomic<int> showCounter;
    
    std::mutex showMutex;

public:
    ShowManager();
    
    // Movie operations
    int createMovie(const std::string& name, int durationInMinutes);
    std::shared_ptr<Movie> getMovie(int movieId);
    
    // Show operations
    int createShow(int movieId, std::shared_ptr<Screen> screen, 
                   std::chrono::system_clock::time_point startTime, 
                   int durationInMinutes);
    std::shared_ptr<Show> getShow(int showId);
    std::vector<std::shared_ptr<Show>> getShowsForScreen(std::shared_ptr<Screen> screen);
};

