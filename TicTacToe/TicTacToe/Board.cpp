#include <iostream>
#include <algorithm>
#include "Board.h"
#include "GameContext.h"
#include "Player.h"
#include "GameStates.h"

// Constructor
Board::Board(int r, int c) : rows(r), cols(c) {
    grid.assign(r, std::vector<Symbol>(c, Symbol::EMPTY));
}

// Checks if a given position is within the bounds of the board
bool Board::isValidMove(const Position& pos) {
    return pos.row >= 0 && pos.row < rows &&
           pos.col >= 0 && pos.col < cols &&
           grid[pos.row][pos.col] == Symbol::EMPTY;
}

// Allows players to make their moves
void Board::makeMove(const Position& pos, Symbol sym) {
    grid[pos.row][pos.col] = sym;
}

// Determines the current state of the game by checking for
// Rows, Columns and Diagonals for winning conditions
void Board::checkAndUpdateGameStateAfterEachMove(GameContext& context, Player* currentPlayer) { //iske andarr hasWon bhi dena pdega
    // Check rows
    for (int i = 0; i < rows; i++) {
        if (grid[i][0] != Symbol::EMPTY && isWinningLine(grid[i])) {
            context.next(currentPlayer, true);
            return;
        }
    }

    // Check columns
    for (int j = 0; j < cols; j++) {
        std::vector<Symbol> column;
        for (int i = 0; i < rows; i++) {
            column.push_back(grid[i][j]);
        }
        if (column[0] != Symbol::EMPTY && isWinningLine(column)) {
            context.next(currentPlayer, true);
            return;
        }
    }

    // Check diagonals
    int minDim = std::min(rows, cols);
    std::vector<Symbol> diagonal1;
    std::vector<Symbol> diagonal2;
    
    for (int i = 0; i < minDim; i++) {
        diagonal1.push_back(grid[i][i]);
        diagonal2.push_back(grid[i][cols - 1 - i]);
    }

    if (diagonal1[0] != Symbol::EMPTY && isWinningLine(diagonal1)) {
        context.next(currentPlayer, true);
        return;
    }

    if (diagonal2[0] != Symbol::EMPTY && isWinningLine(diagonal2)) {
        context.next(currentPlayer, true);
        return;
    }

    // Check for draw - if board is full and no winner
    bool isFull = true;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == Symbol::EMPTY) {
                isFull = false;
                break;
            }
        }
        if (!isFull) break;
    }

    if (isFull) {
        // Board is full, it's a draw
        context.setState(new DrawState());
        return;
    }

    // No win detected, continue game
    context.next(currentPlayer, false);
}

// Helper method to check if a line is a winning line
bool Board::isWinningLine(const std::vector<Symbol>& line) {
    Symbol first = line[0];
    for (const Symbol& s : line) {
        if (s != first) {
            return false;
        }
    }
    return true;
}

// Prints the current board state
void Board::printBoard() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Symbol symbol = grid[i][j];
            switch (symbol) {
                case Symbol::X:
                    std::cout << " X ";
                    break;
                case Symbol::O:
                    std::cout << " O ";
                    break;
                case Symbol::EMPTY:
                default:
                    std::cout << " . ";
            }
            if (j < cols - 1) {
                std::cout << "|";
            }
        }
        std::cout << "\n";
        if (i < rows - 1) {
            std::cout << "---+---+---\n";
        }
    }
    std::cout << "\n";
}
