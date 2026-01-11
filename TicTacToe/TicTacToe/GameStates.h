#pragma once
#include "GameState.h"
#include "Symbol.h"

// Forward declarations
class GameContext;
class Player;

// Concrete State: XTurnState
class XTurnState : public GameState {
public:
    void next(GameContext& context, Player* player, bool hasWon) override;
    bool isGameOver() const override { return false; }
};

// Concrete State: OTurnState
class OTurnState : public GameState {
public:
    void next(GameContext& context, Player* player, bool hasWon) override;
    bool isGameOver() const override { return false; }
};

// Concrete State: XWonState
class XWonState : public GameState {
public:
    void next(GameContext&, Player*, bool) override {
        // Game over, no next state
    }
    bool isGameOver() const override { return true; }
};

// Concrete State: OWonState
class OWonState : public GameState {
public:
    void next(GameContext&, Player*, bool) override {
        // Game over, no next state
    }
    bool isGameOver() const override { return true; }
};

// Concrete State: DrawState
class DrawState : public GameState {
public:
    void next(GameContext&, Player*, bool) override {
        // Game over, no next state
    }
    bool isGameOver() const override { return true; }
};

