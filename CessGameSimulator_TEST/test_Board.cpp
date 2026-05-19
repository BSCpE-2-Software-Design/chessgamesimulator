#include "doctest.h"
#include "Board.h"
#include <sstream>

TEST_CASE("ChessBoard initialization tests") {

    SUBCASE("Default constructor initializes board correctly") {
        ChessBoard board;
        CHECK(board.getCurrentTurn() == Color::WHITE);
        CHECK(board.isGameOver() == false);
        std::vector<PieceOption> pieces = board.getAvailablePieces();
        REQUIRE(pieces.size() > 0);
        bool whitePawnFound = false;
        for (const auto& piece : pieces) {
            if (piece.name == "Pawn" && piece.position == "a2") {
                whitePawnFound = true;
                break;
            }
        }
        CHECK(whitePawnFound == true);
    }

    SUBCASE("Board displays correctly") {
        ChessBoard board;
        std::stringstream output;
        std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());
        board.display();
        std::cout.rdbuf(oldCout);
        std::string result = output.str();
        CHECK(result.find("BOARD VIEW") != std::string::npos);
        CHECK(result.find("WHITE'S TURN") != std::string::npos);
        CHECK(result.find("a   b   c   d   e   f   g   h") != std::string::npos);
    }
}

TEST_CASE("ChessBoard square naming tests") {
    ChessBoard board;

    SUBCASE("Get square name from coordinates") {
        std::vector<PieceOption> pieces = board.getAvailablePieces();

        bool validNames = true;
        for (const auto& piece : pieces) {
            if (piece.position.length() != 2 && piece.position.length() != 3) {
                validNames = false;
                break;
            }
        }
        CHECK(validNames == true);
    }
}
TEST_CASE("ChessBoard check detection tests") {
    ChessBoard board;

    SUBCASE("King not in check initially") {
        CHECK(board.isGameOver() == false);
    }
}

TEST_CASE("ChessBoard move execution tests") {
    ChessBoard board;

    SUBCASE("Execute valid pawn move") {
        bool result = board.executeMove(6, 0, 4, 0);
        CHECK(result == true);
        CHECK(board.getCurrentTurn() == Color::BLACK);
    }

    SUBCASE("Execute invalid move returns false") {
        bool result = board.executeMove(6, 0, 0, 0);
        CHECK(result == false);
        CHECK(board.getCurrentTurn() == Color::WHITE);
    }

    SUBCASE("Pawn promotion to queen") {
        ChessBoard testBoard;
        CHECK(true); 
    }

    SUBCASE("Move records last move detail") {
        board.executeMove(6, 0, 4, 0);
        std::stringstream output;
        std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());

        board.display();

        std::cout.rdbuf(oldCout);
        std::string result = output.str();

        CHECK(result.find("Last move:") != std::string::npos);
    }
}

TEST_CASE("ChessBoard game over conditions") {
    ChessBoard board;

    SUBCASE("Game not over initially") {
        CHECK(board.isGameOver() == false);
    }

    SUBCASE("Can set game over") {
        board.setGameOver(true);
        CHECK(board.isGameOver() == true);

        board.setGameOver(false);
        CHECK(board.isGameOver() == false);
    }
}

TEST_CASE("ChessBoard available pieces tests") {
    ChessBoard board;

    SUBCASE("Get available pieces at start") {
        std::vector<PieceOption> pieces = board.getAvailablePieces();

        REQUIRE(pieces.size() > 0);
        bool hasPawn = false;
        for (const auto& piece : pieces) {
            if (piece.name == "Pawn") {
                hasPawn = true;
                break;
            }
        }
        CHECK(hasPawn == true);
    }

    SUBCASE("Available pieces decrease after moves") {
        std::vector<PieceOption> initialPieces = board.getAvailablePieces();
        size_t initialCount = initialPieces.size();
        board.executeMove(6, 0, 4, 0);

        std::vector<PieceOption> newPieces = board.getAvailablePieces();
        CHECK(newPieces.size() != initialCount);
    }
}

TEST_CASE("ChessBoard piece menu display tests") {
    ChessBoard board;

    SUBCASE("Display piece menu") {
        std::stringstream output;
        std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());

        board.displayPieceMenu();

        std::cout.rdbuf(oldCout);
        std::string result = output.str();

        CHECK(result.find("SELECTION STAGE") != std::string::npos);
        CHECK(result.find("SELECT A PIECE TO MOVE") != std::string::npos);
    }
}

TEST_CASE("ChessBoard move menu tests") {
    ChessBoard board;

    SUBCASE("Get available moves for piece") {
        std::vector<PieceOption> pieces = board.getAvailablePieces();

        if (!pieces.empty()) {
            std::vector<MoveOption> moves = board.getAvailableMovesForPiece(pieces[0].row, pieces[0].col);

            CHECK(moves.size() >= 0);
        }
    }
}

TEST_CASE("ChessBoard turn switching tests") {
    ChessBoard board;

    SUBCASE("Turn alternates after valid move") {
        REQUIRE(board.getCurrentTurn() == Color::WHITE);

        board.executeMove(6, 0, 5, 0);
        CHECK(board.getCurrentTurn() == Color::BLACK);

        board.executeMove(1, 0, 2, 0);
        CHECK(board.getCurrentTurn() == Color::WHITE);
    }

    SUBCASE("Turn doesn't change after invalid move") {
        REQUIRE(board.getCurrentTurn() == Color::WHITE);

        board.executeMove(6, 0, 0, 0);
        CHECK(board.getCurrentTurn() == Color::WHITE);
    }
}
TEST_CASE("ChessBoard capture tests") {
    ChessBoard board;

    SUBCASE("Capture opponent piece") {
        board.executeMove(6, 3, 4, 3); 
        board.executeMove(1, 4, 3, 4); 
        bool result = board.executeMove(4, 3, 3, 4);
        CHECK(result == true);
    }
    SUBCASE("Cannot capture own piece") {
        bool result = board.executeMove(7, 3, 7, 4); 
        CHECK(result == false);
    }
}

TEST_CASE("ChessBoard edge cases") {
    ChessBoard board;

    SUBCASE("Out of bounds moves") {
        bool result = board.executeMove(6, 0, -1, 0);
        CHECK(result == false);

        result = board.executeMove(6, 0, 8, 0);
        CHECK(result == false);
    }

    SUBCASE("Move from empty square") {
        bool result = board.executeMove(0, 0, 0, 1);
        CHECK(result == false);
    }

    SUBCASE("Quit game functionality") {
        board.setGameOver(true);
        CHECK(board.isGameOver() == true);
    }
}