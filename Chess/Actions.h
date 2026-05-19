#pragma once
#pragma once
#include <iostream>
#include <string>

// Structure to represent a piece option
struct PieceOption {
    int row, col;
    std::string name;
    std::string position;
};

// Structure to represent a move option
struct MoveOption {
    int toRow, toCol;
    std::string description;
};