#include "../include/MovementStrategy.h"
#include "../include/Board.h"
#include "../include/Piece.h"
#include <cmath>

// King Movement
bool KingMovement::canMove(const Position& from, const Position& to, Board* board) {
    int rowDiff = abs(to.row - from.row);
    int colDiff = abs(to.col - from.col);
    
    if (rowDiff > 1 || colDiff > 1) return false;
    if (from == to) return false;
    
    Piece* destPiece = board->getPiece(to);
    Piece* srcPiece = board->getPiece(from);
    
    if (destPiece && destPiece->getColor() == srcPiece->getColor()) return false;
    
    return true;
}

std::vector<Position> KingMovement::getPossibleMoves(const Position& from, Board* board) {
    std::vector<Position> moves;
    int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
    
    for (auto& dir : dirs) {
        Position to(from.row + dir[0], from.col + dir[1]);
        if (to.isValid() && canMove(from, to, board)) {
            moves.push_back(to);
        }
    }
    return moves;
}

// Queen Movement (Rook + Bishop)
bool QueenMovement::canMove(const Position& from, const Position& to, Board* board) {
    if (from == to) return false;
    
    int rowDiff = abs(to.row - from.row);
    int colDiff = abs(to.col - from.col);
    
    bool isDiagonal = (rowDiff == colDiff);
    bool isStraight = (from.row == to.row || from.col == to.col);
    
    if (!isDiagonal && !isStraight) return false;
    
    Piece* destPiece = board->getPiece(to);
    Piece* srcPiece = board->getPiece(from);
    
    if (destPiece && destPiece->getColor() == srcPiece->getColor()) return false;
    
    return board->isPathClear(from, to);
}

std::vector<Position> QueenMovement::getPossibleMoves(const Position& from, Board* board) {
    std::vector<Position> moves;
    int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
    
    for (auto& dir : dirs) {
        for (int i = 1; i < 8; i++) {
            Position to(from.row + dir[0]*i, from.col + dir[1]*i);
            if (!to.isValid()) break;
            if (canMove(from, to, board)) {
                moves.push_back(to);
                if (board->getPiece(to)) break;
            } else break;
        }
    }
    return moves;
}

// Rook Movement
bool RookMovement::canMove(const Position& from, const Position& to, Board* board) {
    if (from == to) return false;
    if (from.row != to.row && from.col != to.col) return false;
    
    Piece* destPiece = board->getPiece(to);
    Piece* srcPiece = board->getPiece(from);
    
    if (destPiece && destPiece->getColor() == srcPiece->getColor()) return false;
    
    return board->isPathClear(from, to);
}

std::vector<Position> RookMovement::getPossibleMoves(const Position& from, Board* board) {
    std::vector<Position> moves;
    int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    
    for (auto& dir : dirs) {
        for (int i = 1; i < 8; i++) {
            Position to(from.row + dir[0]*i, from.col + dir[1]*i);
            if (!to.isValid()) break;
            if (canMove(from, to, board)) {
                moves.push_back(to);
                if (board->getPiece(to)) break;
            } else break;
        }
    }
    return moves;
}

// Bishop Movement
bool BishopMovement::canMove(const Position& from, const Position& to, Board* board) {
    if (from == to) return false;
    
    int rowDiff = abs(to.row - from.row);
    int colDiff = abs(to.col - from.col);
    
    if (rowDiff != colDiff) return false;
    
    Piece* destPiece = board->getPiece(to);
    Piece* srcPiece = board->getPiece(from);
    
    if (destPiece && destPiece->getColor() == srcPiece->getColor()) return false;
    
    return board->isPathClear(from, to);
}

std::vector<Position> BishopMovement::getPossibleMoves(const Position& from, Board* board) {
    std::vector<Position> moves;
    int dirs[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
    
    for (auto& dir : dirs) {
        for (int i = 1; i < 8; i++) {
            Position to(from.row + dir[0]*i, from.col + dir[1]*i);
            if (!to.isValid()) break;
            if (canMove(from, to, board)) {
                moves.push_back(to);
                if (board->getPiece(to)) break;
            } else break;
        }
    }
    return moves;
}

// Knight Movement
bool KnightMovement::canMove(const Position& from, const Position& to, Board* board) {
    int rowDiff = abs(to.row - from.row);
    int colDiff = abs(to.col - from.col);
    
    if (!((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2))) return false;
    
    Piece* destPiece = board->getPiece(to);
    Piece* srcPiece = board->getPiece(from);
    
    if (destPiece && destPiece->getColor() == srcPiece->getColor()) return false;
    
    return true;
}

std::vector<Position> KnightMovement::getPossibleMoves(const Position& from, Board* board) {
    std::vector<Position> moves;
    int offsets[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
    
    for (auto& offset : offsets) {
        Position to(from.row + offset[0], from.col + offset[1]);
        if (to.isValid() && canMove(from, to, board)) {
            moves.push_back(to);
        }
    }
    return moves;
}

// Pawn Movement
bool PawnMovement::canMove(const Position& from, const Position& to, Board* board) {
    Piece* srcPiece = board->getPiece(from);
    Piece* destPiece = board->getPiece(to);
    
    int direction = (srcPiece->getColor() == PieceColor::WHITE) ? -1 : 1;
    int startRow = (srcPiece->getColor() == PieceColor::WHITE) ? 6 : 1;
    
    // Forward move
    if (from.col == to.col) {
        if (to.row == from.row + direction && !destPiece) return true;
        if (from.row == startRow && to.row == from.row + 2*direction && 
            !destPiece && board->isEmpty(Position(from.row + direction, from.col))) {
            return true;
        }
    }
    
    // Diagonal capture
    if (abs(to.col - from.col) == 1 && to.row == from.row + direction) {
        if (destPiece && destPiece->getColor() != srcPiece->getColor()) return true;
    }
    
    return false;
}

std::vector<Position> PawnMovement::getPossibleMoves(const Position& from, Board* board) {
    std::vector<Position> moves;
    Piece* srcPiece = board->getPiece(from);
    
    int direction = (srcPiece->getColor() == PieceColor::WHITE) ? -1 : 1;
    int startRow = (srcPiece->getColor() == PieceColor::WHITE) ? 6 : 1;
    
    // Forward
    Position oneStep(from.row + direction, from.col);
    if (oneStep.isValid() && canMove(from, oneStep, board)) {
        moves.push_back(oneStep);
    }
    
    // Two steps
    if (from.row == startRow) {
        Position twoStep(from.row + 2*direction, from.col);
        if (twoStep.isValid() && canMove(from, twoStep, board)) {
            moves.push_back(twoStep);
        }
    }
    
    // Captures
    Position captureLeft(from.row + direction, from.col - 1);
    if (captureLeft.isValid() && canMove(from, captureLeft, board)) {
        moves.push_back(captureLeft);
    }
    
    Position captureRight(from.row + direction, from.col + 1);
    if (captureRight.isValid() && canMove(from, captureRight, board)) {
        moves.push_back(captureRight);
    }
    
    return moves;
}

