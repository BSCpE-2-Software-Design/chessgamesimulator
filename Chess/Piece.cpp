#include "Piece.h"
#include <cctype>

Piece::Piece(PieceType t, Color c) : type(t), color(c), hasMoved(false) {
}

char Piece::getSymbol() const {
    if (color == Color::NONE) return ' ';

    char symbol;
    switch (type) {
    case PieceType::KING:   symbol = 'K'; break;
    case PieceType::QUEEN:  symbol = 'Q'; break;
    case PieceType::ROOK:   symbol = 'R'; break;
    case PieceType::BISHOP: symbol = 'B'; break;
    case PieceType::KNIGHT: symbol = 'N'; break;
    case PieceType::PAWN:   symbol = 'P'; break;
    default: return ' ';
    }
    return (color == Color::WHITE) ? symbol : tolower(symbol);
}

std::string Piece::getName() const {
    switch (type) {
    case PieceType::KING:   return "King";
    case PieceType::QUEEN:  return "Queen";
    case PieceType::ROOK:   return "Rook";
    case PieceType::BISHOP: return "Bishop";
    case PieceType::KNIGHT: return "Knight";
    case PieceType::PAWN:   return "Pawn";
    default: return "None";
    }
}

std::string Piece::getFullName() const {
    std::string colorName = (color == Color::WHITE) ? "White" : "Black";
    return colorName + " " + getName();
}