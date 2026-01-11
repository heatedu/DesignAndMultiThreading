#include "../include/Game.h"
#include <iostream>

Game::Game(const std::string& player1, const std::string& player2) 
    : state(GameState::ACTIVE), currentTurn(PieceColor::WHITE)
{
    board = Board::getInstance();
    board->initialize();
    whitePlayer = std::make_unique<Player>(player1, PieceColor::WHITE);
    blackPlayer = std::make_unique<Player>(player2, PieceColor::BLACK);
}

void Game::start() {
    std::cout << "\n=== Chess Game Started ===\n";
    std::cout << whitePlayer->getName() << " (White) vs " 
              << blackPlayer->getName() << " (Black)\n";
}

bool Game::makeMove(const Position& from, const Position& to) {
    Piece* piece = board->getPiece(from);
    
    if (!piece) {
        std::cout << "No piece at that position!\n";
        return false;
    }
    
    if (piece->getColor() != currentTurn) {
        std::cout << "Not your piece!\n";
        return false;
    }
    
    // Try the move
    if (!board->movePiece(from, to)) {
        std::cout << "Invalid move!\n";
        return false;
    }
    
    // Check if move puts own king in check (simplified - should undo move)
    if (board->isInCheck(currentTurn)) {
        std::cout << "Move leaves king in check!\n";
        // In production, you'd undo the move here
        return false;
    }
    
    return true;
}

void Game::switchTurn() {
    currentTurn = (currentTurn == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
}

void Game::updateGameState() {
    PieceColor enemyColor = (currentTurn == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
    
    if (board->isInCheck(currentTurn)) {
        if (!board->hasValidMoves(currentTurn)) {
            state = (currentTurn == PieceColor::WHITE) ? GameState::BLACK_WIN : GameState::WHITE_WIN;
            Player* winner = (currentTurn == PieceColor::WHITE) ? blackPlayer.get() : whitePlayer.get();
            std::cout << "\nCheckmate! " << winner->getName() << " wins!\n";
        } else {
            std::cout << "\nCheck!\n";
        }
    } else if (!board->hasValidMoves(currentTurn)) {
        state = GameState::STALEMATE;
        std::cout << "\nStalemate!\n";
    }
}

Player* Game::getCurrentPlayer() {
    return (currentTurn == PieceColor::WHITE) ? whitePlayer.get() : blackPlayer.get();
}

void Game::displayBoard() {
    board->display();
}

