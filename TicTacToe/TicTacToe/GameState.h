#pragma once

// Forward declarations
class GameContext;
class Player;

class GameState {
public:
    virtual void next(GameContext& context, Player* player, bool hasWon) = 0;
    virtual bool isGameOver() const = 0;
    virtual ~GameState() = default;
};

