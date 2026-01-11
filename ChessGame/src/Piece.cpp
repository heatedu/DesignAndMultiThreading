#include "../include/Piece.h"
#include "../include/Board.h"

Piece::Piece(PieceColor c, PieceType t, std::unique_ptr<MovementStrategy> strategy)
    : color(c), type(t), movementStrategy(std::move(strategy)), hasMoved(false) {}

bool Piece::canMove(const Position& from, const Position& to, Board* board) {
    return movementStrategy->canMove(from, to, board);
}

std::vector<Position> Piece::getPossibleMoves(const Position& from, Board* board) {
    return movementStrategy->getPossibleMoves(from, board);
}

// King
King::King(PieceColor c) : Piece(c, PieceType::KING, std::make_unique<KingMovement>()) {}
char King::getSymbol() const { return color == PieceColor::WHITE ? 'K' : 'k'; }

// Queen
Queen::Queen(PieceColor c) : Piece(c, PieceType::QUEEN, std::make_unique<QueenMovement>()) {}
char Queen::getSymbol() const { return color == PieceColor::WHITE ? 'Q' : 'q'; }

// Rook
Rook::Rook(PieceColor c) : Piece(c, PieceType::ROOK, std::make_unique<RookMovement>()) {}
char Rook::getSymbol() const { return color == PieceColor::WHITE ? 'R' : 'r'; }

// Bishop
Bishop::Bishop(PieceColor c) : Piece(c, PieceType::BISHOP, std::make_unique<BishopMovement>()) {}
char Bishop::getSymbol() const { return color == PieceColor::WHITE ? 'B' : 'b'; }

// Knight
Knight::Knight(PieceColor c) : Piece(c, PieceType::KNIGHT, std::make_unique<KnightMovement>()) {}
char Knight::getSymbol() const { return color == PieceColor::WHITE ? 'N' : 'n'; }

// Pawn
Pawn::Pawn(PieceColor c) : Piece(c, PieceType::PAWN, std::make_unique<PawnMovement>()) {}
char Pawn::getSymbol() const { return color == PieceColor::WHITE ? 'P' : 'p'; }

