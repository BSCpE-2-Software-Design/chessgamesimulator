#pragma once
#include "Attribute.h"
#include <string>

class Piece {
public:
    PieceType type;
    Color color;
    bool hasMoved;

    Piece(PieceType t = PieceType::NONE, Color c = Color::NONE);

    char getSymbol() const;
    std::string getName() const;
    std::string getFullName() const;
};