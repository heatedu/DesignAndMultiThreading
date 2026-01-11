#pragma once
#include <vector>
#include "Symbol.h"
#include "Position.h"

class GameContext;  // Forward declaration
class Player;       // Forward declaration

class Board {
    int rows, cols;
    std::vector<std::vector<Symbol>> grid;

public:
    Board(int r, int c);

    // Checks if a given position is within the bounds of the board
    bool isValidMove(const Position& pos);

    // Allows players to make their moves
    void makeMove(const Position& pos, Symbol sym);

    // Determines the current state of the game by checking for
    // Rows, Columns and Diagonals for winning conditions
    void checkAndUpdateGameStateAfterEachMove(GameContext& context, Player* currentPlayer);

    // Prints the current board state
    void printBoard();

private:
    // Helper method to check if a line is a winning line
    bool isWinningLine(const std::vector<Symbol>& line);
};
