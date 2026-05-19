#include "Board.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <limits>
#include <sstream>

ChessBoard::ChessBoard() : m_board(8, std::vector<Piece>(8)), m_currentTurn(Color::WHITE),
m_gameOver(false), m_isInCheck(false), m_checkColor(Color::NONE) {
    initializeBoard();
}

void ChessBoard::initializeBoard() {
    // Clear board
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            m_board[i][j] = Piece();
        }
    }

    // Place pawns
    for (int i = 0; i < 8; i++) {
        m_board[1][i] = Piece(PieceType::PAWN, Color::BLACK);
        m_board[6][i] = Piece(PieceType::PAWN, Color::WHITE);
    }

    // Place other pieces for black
    m_board[0][0] = Piece(PieceType::ROOK, Color::BLACK);
    m_board[0][1] = Piece(PieceType::KNIGHT, Color::BLACK);
    m_board[0][2] = Piece(PieceType::BISHOP, Color::BLACK);
    m_board[0][3] = Piece(PieceType::QUEEN, Color::BLACK);
    m_board[0][4] = Piece(PieceType::KING, Color::BLACK);
    m_board[0][5] = Piece(PieceType::BISHOP, Color::BLACK);
    m_board[0][6] = Piece(PieceType::KNIGHT, Color::BLACK);
    m_board[0][7] = Piece(PieceType::ROOK, Color::BLACK);

    // Place other pieces for white
    m_board[7][0] = Piece(PieceType::ROOK, Color::WHITE);
    m_board[7][1] = Piece(PieceType::KNIGHT, Color::WHITE);
    m_board[7][2] = Piece(PieceType::BISHOP, Color::WHITE);
    m_board[7][3] = Piece(PieceType::QUEEN, Color::WHITE);
    m_board[7][4] = Piece(PieceType::KING, Color::WHITE);
    m_board[7][5] = Piece(PieceType::BISHOP, Color::WHITE);
    m_board[7][6] = Piece(PieceType::KNIGHT, Color::WHITE);
    m_board[7][7] = Piece(PieceType::ROOK, Color::WHITE);
}

void ChessBoard::display() const {
    std::cout << "        **** BOARD VIEW ****       \n\n";
    std::cout << "\n    a   b   c   d   e   f   g   h\n";
    std::cout << "  +---+---+---+---+---+---+---+---+\n";
    for (int row = 0; row < 8; row++) {
        std::cout << 8 - row << " ";
        for (int col = 0; col < 8; col++) {
            std::cout << "| " << m_board[row][col].getSymbol() << " ";
        }
        std::cout << "| " << 8 - row << "\n";
        std::cout << "  +---+---+---+---+---+---+---+---+\n";
    }
    std::cout << "    a   b   c   d   e   f   g   h\n\n";

    std::cout << "====================================\n";
    std::cout << (m_currentTurn == Color::WHITE ? "WHITE'S" : "BLACK'S") << " TURN\n";

    if (m_isInCheck && m_checkColor == m_currentTurn) {
        std::cout << "  YOU ARE IN CHECK! \n";
    }

    if (!m_lastMoveDetail.empty()) {
        std::cout << "Last move: " << m_lastMoveDetail << "\n";
    }
    std::cout << "====================================\n\n";
}

std::string ChessBoard::getSquareName(int row, int col) const {
    char file = 'a' + col;
    int rank = 8 - row;
    return std::string(1, file) + std::to_string(rank);
}

std::pair<int, int> ChessBoard::findKing(Color color) const {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (m_board[row][col].type == PieceType::KING && m_board[row][col].color == color) {
                return { row, col };
            }
        }
    }
    return { -1, -1 };
}

bool ChessBoard::isSquareAttacked(int row, int col, Color defenderColor) const {
    Color attackerColor = (defenderColor == Color::WHITE) ? Color::BLACK : Color::WHITE;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            const Piece& piece = m_board[r][c];
            if (piece.color == attackerColor) {
                if (isValidMoveWithoutCheck(r, c, row, col, true)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool ChessBoard::isKingInCheck(Color color) const {
    auto [kingRow, kingCol] = findKing(color);
    return isSquareAttacked(kingRow, kingCol, color);
}

bool ChessBoard::hasAnyLegalMove(Color color) {
    for (int fromRow = 0; fromRow < 8; fromRow++) {
        for (int fromCol = 0; fromCol < 8; fromCol++) {
            const Piece& piece = m_board[fromRow][fromCol];
            if (piece.color == color) {
                for (int toRow = 0; toRow < 8; toRow++) {
                    for (int toCol = 0; toCol < 8; toCol++) {
                        if (isValidMove(fromRow, fromCol, toRow, toCol)) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool ChessBoard::wouldMoveCauseCheck(int fromRow, int fromCol, int toRow, int toCol) const {
    std::vector<std::vector<Piece>> tempBoard = m_board;
    Piece movingPiece = tempBoard[fromRow][fromCol];
    tempBoard[toRow][toCol] = movingPiece;
    tempBoard[fromRow][fromCol] = Piece();

    Color kingColor = movingPiece.color;
    int kingRow, kingCol;

    if (movingPiece.type == PieceType::KING) {
        kingRow = toRow;
        kingCol = toCol;
    }
    else {
        kingRow = -1;
        kingCol = -1;
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                if (tempBoard[r][c].type == PieceType::KING && tempBoard[r][c].color == kingColor) {
                    kingRow = r;
                    kingCol = c;
                    break;
                }
            }
        }
    }

    Color attackerColor = (kingColor == Color::WHITE) ? Color::BLACK : Color::WHITE;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            const Piece& piece = tempBoard[r][c];
            if (piece.color == attackerColor) {
                if (isValidMoveWithoutCheck(r, c, kingRow, kingCol, true, tempBoard)) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool ChessBoard::isValidMove(int fromRow, int fromCol, int toRow, int toCol) {
    if (!isValidMoveWithoutCheck(fromRow, fromCol, toRow, toCol, false)) {
        return false;
    }

    if (wouldMoveCauseCheck(fromRow, fromCol, toRow, toCol)) {
        return false;
    }

    return true;
}

bool ChessBoard::isValidMoveWithoutCheck(int fromRow, int fromCol, int toRow, int toCol,
    bool ignoreSelfCheck,
    const std::vector<std::vector<Piece>>& boardToUse) const {
    const auto& currentBoard = (boardToUse.empty()) ? m_board : boardToUse;

    if (fromRow < 0 || fromRow >= 8 || fromCol < 0 || fromCol >= 8 ||
        toRow < 0 || toRow >= 8 || toCol < 0 || toCol >= 8) {
        return false;
    }

    const Piece& piece = currentBoard[fromRow][fromCol];

    if (piece.type == PieceType::NONE) {
        return false;
    }

    if (!ignoreSelfCheck && piece.color != m_currentTurn) {
        return false;
    }

    if (currentBoard[toRow][toCol].color == piece.color) {
        return false;
    }

    int rowDiff = toRow - fromRow;
    int colDiff = toCol - fromCol;

    switch (piece.type) {
    case PieceType::PAWN: {
        int direction = (piece.color == Color::WHITE) ? -1 : 1;
        int startRow = (piece.color == Color::WHITE) ? 6 : 1;

        if (colDiff == 0 && rowDiff == direction && currentBoard[toRow][toCol].type == PieceType::NONE) {
            return true;
        }

        if (colDiff == 0 && rowDiff == 2 * direction && fromRow == startRow &&
            currentBoard[toRow][toCol].type == PieceType::NONE &&
            currentBoard[fromRow + direction][fromCol].type == PieceType::NONE) {
            return true;
        }

        if (std::abs(colDiff) == 1 && rowDiff == direction && currentBoard[toRow][toCol].type != PieceType::NONE) {
            return true;
        }

        return false;
    }

    case PieceType::KNIGHT: {
        return (std::abs(rowDiff) == 2 && std::abs(colDiff) == 1) ||
            (std::abs(rowDiff) == 1 && std::abs(colDiff) == 2);
    }

    case PieceType::BISHOP: {
        if (std::abs(rowDiff) != std::abs(colDiff)) return false;
        return isClearPath(fromRow, fromCol, toRow, toCol, currentBoard);
    }

    case PieceType::ROOK: {
        if (fromRow != toRow && fromCol != toCol) return false;
        return isClearPath(fromRow, fromCol, toRow, toCol, currentBoard);
    }

    case PieceType::QUEEN: {
        if (fromRow != toRow && fromCol != toCol && std::abs(rowDiff) != std::abs(colDiff)) return false;
        return isClearPath(fromRow, fromCol, toRow, toCol, currentBoard);
    }

    case PieceType::KING: {
        return std::max(std::abs(rowDiff), std::abs(colDiff)) == 1;
    }

    default:
        return false;
    }
}

bool ChessBoard::isClearPath(int fromRow, int fromCol, int toRow, int toCol,
    const std::vector<std::vector<Piece>>& boardToUse) const {
    int rowStep = (toRow > fromRow) ? 1 : (toRow < fromRow) ? -1 : 0;
    int colStep = (toCol > fromCol) ? 1 : (toCol < fromCol) ? -1 : 0;

    int row = fromRow + rowStep;
    int col = fromCol + colStep;

    while (row != toRow || col != toCol) {
        if (boardToUse[row][col].type != PieceType::NONE) {
            return false;
        }
        row += rowStep;
        col += colStep;
    }

    return true;
}

std::vector<PieceOption> ChessBoard::getAvailablePieces() {
    std::vector<PieceOption> pieces;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece& piece = m_board[row][col];

            if (piece.type == PieceType::NONE) continue;
            if (piece.color != m_currentTurn) continue;

            bool hasAnyMove = false;
            for (int toRow = 0; toRow < 8; toRow++) {
                for (int toCol = 0; toCol < 8; toCol++) {
                    if (isValidMove(row, col, toRow, toCol)) {
                        hasAnyMove = true;
                        break;
                    }
                }
                if (hasAnyMove) break;
            }

            if (hasAnyMove) {
                PieceOption p;
                p.row = row;
                p.col = col;
                p.name = piece.getName();
                p.position = getSquareName(row, col);
                pieces.push_back(p);
            }
        }
    }

    return pieces;
}

void ChessBoard::displayPieceMenu() {
    std::vector<PieceOption> pieces = getAvailablePieces();

    if (pieces.empty()) {
        std::cout << "\n  NO AVAILABLE MOVES! ";
        if (isKingInCheck(m_currentTurn)) {
            std::cout << "CHECKMATE! " << (m_currentTurn == Color::WHITE ? "Black" : "White") << " wins!\n";
        }
        else {
            std::cout << "STALEMATE! Game is a draw!\n";
        }
        m_gameOver = true;
        return;
    }
    std::cout << "# SELECTION STAGE #\n\n";
    std::cout << "|           SELECT A PIECE TO MOVE               |\n";

    for (size_t i = 0; i < pieces.size(); i++) {
        std::string option = "  " + std::to_string(i + 1) + ". " + pieces[i].name +
            " at " + pieces[i].position;

        char symbol = m_board[pieces[i].row][pieces[i].col].getSymbol();
        option += " [" + std::string(1, symbol) + "]";

        std::cout << "| " << option;
        int padding = 47 - option.length();
        for (int j = 0; j < padding; j++) std::cout << " ";
        std::cout << "|\n";
    }

    std::cout << "\n|  0. Quit game\n";
    std::cout << "|  d. Display board again\n";
}

std::vector<MoveOption> ChessBoard::getAvailableMovesForPiece(int fromRow, int fromCol) {
    std::vector<MoveOption> moves;
    int optionNumber = 1;

    for (int toRow = 0; toRow < 8; toRow++) {
        for (int toCol = 0; toCol < 8; toCol++) {
            if (isValidMove(fromRow, fromCol, toRow, toCol)) {
                MoveOption move;
                move.toRow = toRow;
                move.toCol = toCol;

                std::string toName = getSquareName(toRow, toCol);
                move.description = std::to_string(optionNumber++) + ". Move to " + toName;

                if (m_board[toRow][toCol].type != PieceType::NONE) {
                    move.description += " (capture " + m_board[toRow][toCol].getName() + ")";
                }

                moves.push_back(move);
            }
        }
    }

    return moves;
}

void ChessBoard::displayMoveMenuForPiece(int fromRow, int fromCol) {
    Piece& piece = m_board[fromRow][fromCol];
    std::vector<MoveOption> moves = getAvailableMovesForPiece(fromRow, fromCol);

    std::cout << "\n| MOVES FOR " << piece.getFullName() << " at " << getSquareName(fromRow, fromCol) << "                      |\n\n";

    for (const auto& move : moves) {
        std::cout << "|  " << move.description;
        int padding = 47 - move.description.length();
        for (int i = 0; i < padding; i++) std::cout << " ";
        std::cout << "|\n";
    }

    std::cout << "\n0. Go back to piece selection\n";
}

bool ChessBoard::executeMove(int fromRow, int fromCol, int toRow, int toCol) {
    if (!isValidMove(fromRow, fromCol, toRow, toCol)) {
        return false;
    }

    Piece movedPiece = m_board[fromRow][fromCol];
    m_board[toRow][toCol] = m_board[fromRow][fromCol];
    m_board[fromRow][fromCol] = Piece();
    m_board[toRow][toCol].hasMoved = true;

    std::string fromName = getSquareName(fromRow, fromCol);
    std::string toName = getSquareName(toRow, toCol);
    m_lastMoveDetail = movedPiece.getFullName() + " from " + fromName + " to " + toName;

    if (movedPiece.type == PieceType::PAWN) {
        if ((movedPiece.color == Color::WHITE && toRow == 0) ||
            (movedPiece.color == Color::BLACK && toRow == 7)) {
            m_board[toRow][toCol] = Piece(PieceType::QUEEN, movedPiece.color);
            m_lastMoveDetail += " (Promoted to Queen)";
            std::cout << "\n Pawn promoted to Queen!\n";
        }
    }

    m_currentTurn = (m_currentTurn == Color::WHITE) ? Color::BLACK : Color::WHITE;

    if (isKingInCheck(m_currentTurn)) {
        m_isInCheck = true;
        m_checkColor = m_currentTurn;
        std::cout << "\n  " << (m_currentTurn == Color::WHITE ? "White" : "Black") << " is in CHECK! \n";
    }
    else {
        m_isInCheck = false;
        m_checkColor = Color::NONE;
    }

    if (!hasAnyLegalMove(m_currentTurn)) {
        if (isKingInCheck(m_currentTurn)) {
            std::cout << "\n CHECKMATE! " << (m_currentTurn == Color::WHITE ? "Black" : "White") << " wins! \n";
            m_gameOver = true;
        }
        else {
            std::cout << "\n STALEMATE! Game is a draw! \n";
            m_gameOver = true;
        }
    }

    std::cout << "\n Move successful! " << m_lastMoveDetail << "\n";
    return true;
}

bool ChessBoard::selectPieceAndMove() {
    std::string input;

    while (true) {
        displayPieceMenu();

        std::vector<PieceOption> pieces = getAvailablePieces();
        if (pieces.empty()) {
            m_gameOver = true;
            return false;
        }

        std::cout << "\n Choose a piece (1-" << pieces.size() << ", 0 to quit, d to display): ";
        std::getline(std::cin, input);

        if (input.empty()) return false;

        if (input == "0" || input == "quit" || input == "exit") {
            m_gameOver = true;
            return false;
        }

        if (input == "d" || input == "display") {
            return false;
        }

        int choice;
        try {
            choice = std::stoi(input);
        }
        catch (...) {
            std::cout << " Invalid input! Please enter a number.\n";
            continue;
        }

        if (choice < 1 || choice > static_cast<int>(pieces.size())) {
            std::cout << " Invalid choice! Please select a number between 1 and " << pieces.size() << ".\n";
            continue;
        }

        PieceOption selectedPiece = pieces[choice - 1];

        while (true) {
            displayMoveMenuForPiece(selectedPiece.row, selectedPiece.col);

            std::vector<MoveOption> moves = getAvailableMovesForPiece(selectedPiece.row, selectedPiece.col);

            std::cout << "\n Choose a move (1-" << moves.size() << ", 0 to pick another piece): ";
            std::getline(std::cin, input);

            if (input.empty()) continue;

            if (input == "0") {
                std::cout << "\n  Going back to piece selection...\n";
                break;
            }

            int moveChoice;
            try {
                moveChoice = std::stoi(input);
            }
            catch (...) {
                std::cout << " Invalid input! Please enter a number.\n";
                continue;
            }

            if (moveChoice < 1 || moveChoice > static_cast<int>(moves.size())) {
                std::cout << " Invalid choice! Please select a number between 1 and " << moves.size() << ".\n";
                continue;
            }

            MoveOption selectedMove = moves[moveChoice - 1];
            if (executeMove(selectedPiece.row, selectedPiece.col, selectedMove.toRow, selectedMove.toCol)) {
                return true;
            }
            else {
                std::cout << " Invalid move! Please try again.\n";
            }
        }
    }
}

bool ChessBoard::isGameOver() const {
    return m_gameOver;
}

void ChessBoard::setGameOver(bool over) {
    m_gameOver = over;
}

Color ChessBoard::getCurrentTurn() const {
    return m_currentTurn;
}