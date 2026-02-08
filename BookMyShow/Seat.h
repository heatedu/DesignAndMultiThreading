#pragma once
#include "../enums/SeatCategory.h"

class Seat {
private:
    int seatId;
    int row;
    SeatCategory seatCategory;

public:
    Seat(int id, int rowNum, SeatCategory category);
    
    int getSeatId() const;
    int getRow() const;
    SeatCategory getSeatCategory() const;
    
    bool operator==(const Seat& other) const;
};

