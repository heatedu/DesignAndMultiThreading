#pragma once
#include "GameState.h"
#include "GameStates.h"

class GameContext {
    GameState* currentState;

public:
    GameContext() { currentState = new XTurnState(); }

    void setState(GameState* state) { currentState = state; }

    void next(Player* player, bool hasWon) 
    {
        currentState->next(*this, player, hasWon);
    }

    bool isGameOver() const {
        return currentState->isGameOver();
    }

    GameState* getState() const { return currentState; }
};
