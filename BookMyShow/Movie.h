#pragma once
#include <string>

class Movie {
private:
    int movieId;
    std::string movieName;
    int movieDurationInMinutes;

public:
    Movie(int id, const std::string& name, int duration);
    
    int getMovieId() const;
    std::string getMovieName() const;
    int getMovieDuration() const;
};

