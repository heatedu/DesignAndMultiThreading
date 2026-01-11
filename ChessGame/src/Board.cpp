#include "../include/Board.h"
#include "../include/PieceFactory.h"
#include <iostream>
#include <cmath>

Board* Board::instance = nullptr;

Board::Board() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            grid[i][j] = nullptr;
        }
    }
}

Board* Board::getInstance() {
    if (!instance) {
        instance = new Board();
    }
    return instance;
}

void Board::resetInstance() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

void Board::initialize() {
    // Black pieces (top)
    grid[0][0] = PieceFactory::createPiece(PieceType::ROOK, PieceColor::BLACK);
    grid[0][1] = PieceFactory::createPiece(PieceType::KNIGHT, PieceColor::BLACK);
    grid[0][2] = PieceFactory::createPiece(PieceType::BISHOP, PieceColor::BLACK);
    grid[0][3] = PieceFactory::createPiece(PieceType::QUEEN, PieceColor::BLACK);
    grid[0][4] = PieceFactory::createPiece(PieceType::KING, PieceColor::BLACK);
    grid[0][5] = PieceFactory::createPiece(PieceType::BISHOP, PieceColor::BLACK);
    grid[0][6] = PieceFactory::createPiece(PieceType::KNIGHT, PieceColor::BLACK);
    grid[0][7] = PieceFactory::createPiece(PieceType::ROOK, PieceColor::BLACK);
    
    for (int i = 0; i < 8; i++) {
        grid[1][i] = PieceFactory::createPiece(PieceType::PAWN, PieceColor::BLACK);
    }
    
    // White pieces (bottom)
    for (int i = 0; i < 8; i++) {
        grid[6][i] = PieceFactory::createPiece(PieceType::PAWN, PieceColor::WHITE);
    }
    
    grid[7][0] = PieceFactory::createPiece(PieceType::ROOK, PieceColor::WHITE);
    grid[7][1] = PieceFactory::createPiece(PieceType::KNIGHT, PieceColor::WHITE);
    grid[7][2] = PieceFactory::createPiece(PieceType::BISHOP, PieceColor::WHITE);
    grid[7][3] = PieceFactory::createPiece(PieceType::QUEEN, PieceColor::WHITE);
    grid[7][4] = PieceFactory::createPiece(PieceType::KING, PieceColor::WHITE);
    grid[7][5] = PieceFactory::createPiece(PieceType::BISHOP, PieceColor::WHITE);
    grid[7][6] = PieceFactory::createPiece(PieceType::KNIGHT, PieceColor::WHITE);
    grid[7][7] = PieceFactory::createPiece(PieceType::ROOK, PieceColor::WHITE);
}

Piece* Board::getPiece(const Position& pos) {
    if (!pos.isValid()) return nullptr;
    return grid[pos.row][pos.col].get();
}

bool Board::isEmpty(const Position& pos) {
    return getPiece(pos) == nullptr;
}

bool Board::movePiece(const Position& from, const Position& to) {
    if (!from.isValid() || !to.isValid()) return false;
    
    Piece* piece = getPiece(from);
    if (!piece) return false;
    
    if (!piece->canMove(from, to, this)) return false;
    
    // Make the move
    grid[to.row][to.col] = std::move(grid[from.row][from.col]);
    grid[to.row][to.col]->setHasMoved(true);
    
    return true;
}

void Board::display() const {
    std::cout << "\n  a b c d e f g h\n";
    for (int i = 0; i < 8; i++) {
        std::cout << 8 - i << " ";
        for (int j = 0; j < 8; j++) {
            if (grid[i][j]) {
                std::cout << grid[i][j]->getSymbol() << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << 8 - i << "\n";
    }
    std::cout << "  a b c d e f g h\n";
}

bool Board::isPathClear(const Position& from, const Position& to) {
    int rowDir = (to.row > from.row) ? 1 : (to.row < from.row) ? -1 : 0;
    int colDir = (to.col > from.col) ? 1 : (to.col < from.col) ? -1 : 0;
    
    int row = from.row + rowDir;
    int col = from.col + colDir;
    
    while (row != to.row || col != to.col) 
    {
        if (!isEmpty(Position(row, col))) 
            return false;
        row += rowDir;
        col += colDir;
    }
    
    return true;
}

Position Board::findKing(PieceColor color) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* piece = grid[i][j].get();
            if (piece && piece->getType() == PieceType::KING && piece->getColor() == color) {
                return Position(i, j);
            }
        }
    }
    return Position(-1, -1);
}

bool Board::isSquareUnderAttack(const Position& pos, PieceColor byColor) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* piece = grid[i][j].get();
            if (piece && piece->getColor() == byColor) {
                if (piece->canMove(Position(i, j), pos, this)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Board::isInCheck(PieceColor color) {
    Position kingPos = findKing(color);
    if (!kingPos.isValid()) return false;
    
    PieceColor enemyColor = (color == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
    return isSquareUnderAttack(kingPos, enemyColor);
}

bool Board::hasValidMoves(PieceColor color) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* piece = grid[i][j].get();
            if (piece && piece->getColor() == color) {
                auto moves = piece->getPossibleMoves(Position(i, j), this);
                if (!moves.empty()) return true;
            }
        }
    }
    return false;
}

