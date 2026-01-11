#pragma once
#include <memory>
#include <chrono>
#include "Movie.h"
#include "Screen.h"

class Show {
private:
    int id;
    std::shared_ptr<Movie> movie;
    std::shared_ptr<Screen> screen;
    std::chrono::system_clock::time_point startTime;
    int durationInMinutes;

public:
    Show(int showId, std::shared_ptr<Movie> moviePtr, std::shared_ptr<Screen> screenPtr,
         std::chrono::system_clock::time_point start, int duration);
    
    int getId() const;
    std::shared_ptr<Movie> getMovie() const;
    std::shared_ptr<Screen> getScreen() const;
    std::chrono::system_clock::time_point getStartTime() const;
    int getDurationInMinutes() const;
    
    bool operator==(const Show& other) const;
};

