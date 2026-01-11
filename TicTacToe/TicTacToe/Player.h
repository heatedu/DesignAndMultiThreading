#pragma once
#include "Symbol.h"
#include "PlayerStrategy.h"

class Player {
private:
    Symbol symbol;
    PlayerStrategy* playerStrategy;

public:
    Player(Symbol symbol, PlayerStrategy* playerStrategy)
        : symbol(symbol), playerStrategy(playerStrategy) {
    }

    Symbol getSymbol() const {
        return symbol;
    }

    PlayerStrategy* getStrategy() const {
        return playerStrategy;
    }
};
