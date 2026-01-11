#include "../include/Game.h"
#include <iostream>
#include <string>

Position parsePosition(const std::string& pos) {
    if (pos.length() != 2) return Position(-1, -1);
    
    int col = pos[0] - 'a';
    int row = 8 - (pos[1] - '0');
    
    return Position(row, col);
}

int main() {
    std::cout << "=== Chess Game - Design Patterns Demo ===\n";
    std::cout << "Patterns Used:\n";
    std::cout << "1. Strategy Pattern - Piece Movement\n";
    std::cout << "2. Singleton Pattern - Board\n";
    std::cout << "3. Factory Pattern - Piece Creation\n";
    std::cout << "4. Enum - Game State Management\n\n";
    
    Game game("Alice", "Bob");
    game.start();
    
    while (game.getState() == GameState::ACTIVE) 
    {
        game.displayBoard();
        
        std::cout << "\n" << game.getCurrentPlayer()->getName() 
                  << "'s turn (" << (game.getCurrentTurn() == PieceColor::WHITE ? "White" : "Black") << ")\n";
        std::cout << "Enter move (e.g., 'e2 e4') or 'quit': ";
        
        std::string input;
        std::getline(std::cin, input);
        
        if (input == "quit") {
            std::cout << "Game saved!\n";
            break;
        }
        
        size_t space = input.find(' ');
        if (space == std::string::npos) {
            std::cout << "Invalid input! Use format: e2 e4\n";
            continue;
        }
        
        std::string fromStr = input.substr(0, space);
        std::string toStr = input.substr(space + 1);
        
        Position from = parsePosition(fromStr);
        Position to = parsePosition(toStr);
        
        if (!from.isValid() || !to.isValid()) {
            std::cout << "Invalid positions!\n";
            continue;
        }
        
        if (game.makeMove(from, to)) {
            game.switchTurn();
            game.updateGameState();
        }
    }
    
    Board::resetInstance();
    std::cout << "\nGame Over!\n";
    
    return 0;
}

