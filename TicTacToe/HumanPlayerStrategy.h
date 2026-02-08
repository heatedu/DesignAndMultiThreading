#pragma once
#include <iostream>
#include "PlayerStrategy.h"
#include "Position.h"

class HumanPlayerStrategy : public PlayerStrategy {
private:
    std::string playerName;

public:
    // Constructor
    HumanPlayerStrategy(const std::string& name)
        : playerName(name) {
    }

    // Implementation of makeMove()
    Position makeMove(Board& board) override;
};
