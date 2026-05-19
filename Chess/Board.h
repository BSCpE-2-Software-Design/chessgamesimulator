#pragma once
#include "Piece.h"
#include "Actions.h"
#include "Attribute.h"
#include <vector>
#include <string>
#include <utility>

class ChessBoard {
private:
    std::vector<std::vector<Piece>> m_board;
    Color m_currentTurn;
    bool m_gameOver;
    std::string m_lastMoveDetail;
    bool m_isInCheck;
    Color m_checkColor;

public:
    ChessBoard();

    void initializeBoard();
    void display() const;
    std::string getSquareName(int row, int col) const;
    std::pair<int, int> findKing(Color color) const;
    bool isSquareAttacked(int row, int col, Color defenderColor) const;
    bool isKingInCheck(Color color) const;
    bool hasAnyLegalMove(Color color);
    bool wouldMoveCauseCheck(int fromRow, int fromCol, int toRow, int toCol) const;
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol);
    bool isValidMoveWithoutCheck(int fromRow, int fromCol, int toRow, int toCol,
        bool ignoreSelfCheck = false,
        const std::vector<std::vector<Piece>>& boardToUse = std::vector<std::vector<Piece>>()) const;
    bool isClearPath(int fromRow, int fromCol, int toRow, int toCol,
        const std::vector<std::vector<Piece>>& boardToUse) const;
    std::vector<PieceOption> getAvailablePieces();
    void displayPieceMenu();
    std::vector<MoveOption> getAvailableMovesForPiece(int fromRow, int fromCol);
    void displayMoveMenuForPiece(int fromRow, int fromCol);
    bool executeMove(int fromRow, int fromCol, int toRow, int toCol);
    bool selectPieceAndMove();
    bool isGameOver() const;
    void setGameOver(bool over);
    Color getCurrentTurn() const;
};