#pragma once
#include <memory>
#include "Board.h"
#include "Player.h"

enum class GameState {
    ACTIVE,
    SAVED,
    BLACK_WIN,
    WHITE_WIN,
    STALEMATE
};

class Game {
private:
    Board* board;
    std::unique_ptr<Player> whitePlayer;
    std::unique_ptr<Player> blackPlayer;
    GameState state;
    PieceColor currentTurn;

public:
    Game(const std::string& player1, const std::string& player2);
    
    void start();
    bool makeMove(const Position& from, const Position& to);
    void switchTurn();
    void updateGameState();
    
    GameState getState() const { return state; }
    PieceColor getCurrentTurn() const { return currentTurn; }
    Player* getCurrentPlayer();
    
    void displayBoard();
};

