#pragma once
#include <memory>
#include <string>
#include "Position.h"
#include "MovementStrategy.h"

enum class PieceColor { WHITE, BLACK };
enum class PieceType { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN };

class Piece {
protected:
    PieceColor color;
    PieceType type;
    std::unique_ptr<MovementStrategy> movementStrategy;
    bool hasMoved;

public:
    Piece(PieceColor c, PieceType t, std::unique_ptr<MovementStrategy> strategy);
    virtual ~Piece() = default;

    PieceColor getColor() const { return color; }
    PieceType getType() const { return type; }
    bool getHasMoved() const { return hasMoved; }
    void setHasMoved(bool moved) { hasMoved = moved; }

    bool canMove(const Position& from, const Position& to, Board* board);
    std::vector<Position> getPossibleMoves(const Position& from, Board* board);
    
    virtual char getSymbol() const = 0;
};

class King : public Piece {
public:
    King(PieceColor c);
    char getSymbol() const override;
};

class Queen : public Piece {
public:
    Queen(PieceColor c);
    char getSymbol() const override;
};

class Rook : public Piece {
public:
    Rook(PieceColor c);
    char getSymbol() const override;
};

class Bishop : public Piece {
public:
    Bishop(PieceColor c);
    char getSymbol() const override;
};

class Knight : public Piece {
public:
    Knight(PieceColor c);
    char getSymbol() const override;
};

class Pawn : public Piece {
public:
    Pawn(PieceColor c);
    char getSymbol() const override;
};

