#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Seat.h"

class Theatre;

class Screen {
private:
    int id;
    std::string name;
    std::shared_ptr<Theatre> theatre;
    std::vector<std::shared_ptr<Seat>> seats;

public:
    Screen(int screenId, const std::string& screenName, std::shared_ptr<Theatre> theatrePtr);
    
    void addSeat(std::shared_ptr<Seat> seat);
    int getScreenId() const;
    std::vector<std::shared_ptr<Seat>> getSeats() const;
    std::shared_ptr<Theatre> getTheatre() const;
};

