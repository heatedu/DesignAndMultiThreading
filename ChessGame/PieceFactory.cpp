#include "../include/PieceFactory.h"

std::unique_ptr<Piece> PieceFactory::createPiece(PieceType type, PieceColor color) {
    switch (type) {
        case PieceType::KING:   return std::make_unique<King>(color); //here it will assign king strategy to this piece as it will call king's constructor and in the constructor strategy will be assigned and passed to piece and in piece constructor movement strategy will be updated.
        case PieceType::QUEEN:  return std::make_unique<Queen>(color);
        case PieceType::ROOK:   return std::make_unique<Rook>(color);
        case PieceType::BISHOP: return std::make_unique<Bishop>(color);
        case PieceType::KNIGHT: return std::make_unique<Knight>(color);
        case PieceType::PAWN:   return std::make_unique<Pawn>(color);
        default: return nullptr;
    }
}

