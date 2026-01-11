#pragma once
#include <memory>
#include "Piece.h"

// Factory Pattern for Piece Creation
class PieceFactory {
public:
    static std::unique_ptr<Piece> createPiece(PieceType type, PieceColor color);
};

