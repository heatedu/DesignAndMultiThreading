#pragma once
#include <string>
#include "Piece.h"

class Player {
private:
    std::string name;
    PieceColor color;

public:
    Player(const std::string& n, PieceColor c);
    
    std::string getName() const { return name; }
    PieceColor getColor() const { return color; }
};

