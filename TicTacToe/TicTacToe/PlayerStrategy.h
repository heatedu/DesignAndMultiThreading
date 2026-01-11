#pragma once
#include "Position.h"

class Board;  // Forward declaration

class PlayerStrategy {
public:
    virtual Position makeMove(Board& board) = 0;
    virtual ~PlayerStrategy() = default;
};
