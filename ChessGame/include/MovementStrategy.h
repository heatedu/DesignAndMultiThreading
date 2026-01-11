#pragma once
#include <vector>
#include "Position.h"

class Board;

// Strategy Pattern for Piece Movements
class MovementStrategy {
public:
    virtual ~MovementStrategy() = default;
    virtual bool canMove(const Position& from, const Position& to, Board* board) = 0;
    virtual std::vector<Position> getPossibleMoves(const Position& from, Board* board) = 0;
};

class KingMovement : public MovementStrategy {
public:
    bool canMove(const Position& from, const Position& to, Board* board) override;
    std::vector<Position> getPossibleMoves(const Position& from, Board* board) override;
};

class QueenMovement : public MovementStrategy {
public:
    bool canMove(const Position& from, const Position& to, Board* board) override;
    std::vector<Position> getPossibleMoves(const Position& from, Board* board) override;
};

class RookMovement : public MovementStrategy {
public:
    bool canMove(const Position& from, const Position& to, Board* board) override;
    std::vector<Position> getPossibleMoves(const Position& from, Board* board) override;
};

class BishopMovement : public MovementStrategy {
public:
    bool canMove(const Position& from, const Position& to, Board* board) override;
    std::vector<Position> getPossibleMoves(const Position& from, Board* board) override;
};

class KnightMovement : public MovementStrategy {
public:
    bool canMove(const Position& from, const Position& to, Board* board) override;
    std::vector<Position> getPossibleMoves(const Position& from, Board* board) override;
};

class PawnMovement : public MovementStrategy {
public:
    bool canMove(const Position& from, const Position& to, Board* board) override;
    std::vector<Position> getPossibleMoves(const Position& from, Board* board) override;
};

