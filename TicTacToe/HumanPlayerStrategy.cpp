#include<iostream>
#include "HumanPlayerStrategy.h"
#include "Board.h"

Position HumanPlayerStrategy::makeMove(Board& board) 
{
    while (true) {
        std::cout << playerName
            << ", enter your move (row [0-2] and column [0-2]): ";

        int row, col;

        // Input check
        if (!(std::cin >> row >> col)) {
            std::cin.clear();                      // reset error
            std::cin.ignore(1000, '\n');          // flush garbage
            std::cout << "Invalid input. Please enter valid numbers.\n";
            continue;
        }

        Position move(row, col);

        // Validate move
        if (board.isValidMove(move)) {
            return move;
        }

        std::cout << "Invalid move. Try again.\n";
    }
}
