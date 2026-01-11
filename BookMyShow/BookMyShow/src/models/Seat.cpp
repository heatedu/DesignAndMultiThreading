#include "../../include/models/Seat.h"

Seat::Seat(int id, int rowNum, SeatCategory category)
    : seatId(id), row(rowNum), seatCategory(category) {}

int Seat::getSeatId() const {
    return seatId;
}

int Seat::getRow() const {
    return row;
}

SeatCategory Seat::getSeatCategory() const {
    return seatCategory;
}

bool Seat::operator==(const Seat& other) const {
    return seatId == other.seatId;
}

