#pragma once
#include <memory>
#include <vector>
#include "Piece.h"
#include "Position.h"

// Singleton Pattern for Board
class Board {
private:
    static Board* instance;
    std::unique_ptr<Piece> grid[8][8];

    Board();  // Private constructor
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;

public:
    static Board* getInstance();
    static void resetInstance();

    void initialize();
    Piece* getPiece(const Position& pos);
    bool isEmpty(const Position& pos);
    bool movePiece(const Position& from, const Position& to);
    void display() const;

    bool isPathClear(const Position& from, const Position& to);
    Position findKing(PieceColor color);
    bool isSquareUnderAttack(const Position& pos, PieceColor byColor);
    bool isInCheck(PieceColor color);
    bool hasValidMoves(PieceColor color);
};

