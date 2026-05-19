#include "doctest.h"
#include "Piece.h"

TEST_CASE("Piece class tests") {

    SUBCASE("Constructor initialization") {
        SUBCASE("White king") {
            Piece whiteKing(PieceType::KING, Color::WHITE);

            CHECK(whiteKing.getSymbol() == 'K');
            CHECK(whiteKing.getName() == "King");
            CHECK(whiteKing.getFullName() == "White King");
        }

        SUBCASE("Black queen") {
            Piece blackQueen(PieceType::QUEEN, Color::BLACK);

            CHECK(blackQueen.getSymbol() == 'q');
            CHECK(blackQueen.getName() == "Queen");
            CHECK(blackQueen.getFullName() == "Black Queen");
        }

        SUBCASE("White pawn") {
            Piece whitePawn(PieceType::PAWN, Color::WHITE);

            CHECK(whitePawn.getSymbol() == 'P');
            CHECK(whitePawn.getName() == "Pawn");
            CHECK(whitePawn.getFullName() == "White Pawn");
        }

        SUBCASE("Black knight") {
            Piece blackKnight(PieceType::KNIGHT, Color::BLACK);

            CHECK(blackKnight.getSymbol() == 'n');
            CHECK(blackKnight.getName() == "Knight");
            CHECK(blackKnight.getFullName() == "Black Knight");
        }
    }

    SUBCASE("All piece types and colors") {
        SUBCASE("White pieces") {
            Piece whiteKing(PieceType::KING, Color::WHITE);
            Piece whiteQueen(PieceType::QUEEN, Color::WHITE);
            Piece whiteRook(PieceType::ROOK, Color::WHITE);
            Piece whiteBishop(PieceType::BISHOP, Color::WHITE);
            Piece whiteKnight(PieceType::KNIGHT, Color::WHITE);
            Piece whitePawn(PieceType::PAWN, Color::WHITE);
            CHECK(whiteKing.getSymbol() == 'K');
            CHECK(whiteQueen.getSymbol() == 'Q');
            CHECK(whiteRook.getSymbol() == 'R');
            CHECK(whiteBishop.getSymbol() == 'B');
            CHECK(whiteKnight.getSymbol() == 'N');
            CHECK(whitePawn.getSymbol() == 'P');
            CHECK(whiteKing.getName() == "King");
            CHECK(whiteQueen.getName() == "Queen");
            CHECK(whiteRook.getName() == "Rook");
            CHECK(whiteBishop.getName() == "Bishop");
            CHECK(whiteKnight.getName() == "Knight");
            CHECK(whitePawn.getName() == "Pawn");
        }

        SUBCASE("Black pieces") {
            Piece blackKing(PieceType::KING, Color::BLACK);
            Piece blackQueen(PieceType::QUEEN, Color::BLACK);
            Piece blackRook(PieceType::ROOK, Color::BLACK);
            Piece blackBishop(PieceType::BISHOP, Color::BLACK);
            Piece blackKnight(PieceType::KNIGHT, Color::BLACK);
            Piece blackPawn(PieceType::PAWN, Color::BLACK);

            CHECK(blackKing.getSymbol() == 'k');
            CHECK(blackQueen.getSymbol() == 'q');
            CHECK(blackRook.getSymbol() == 'r');
            CHECK(blackBishop.getSymbol() == 'b');
            CHECK(blackKnight.getSymbol() == 'n');
            CHECK(blackPawn.getSymbol() == 'p');
        }
    }
}

TEST_CASE("Piece move tracking") {

    SUBCASE("Initial hasMoved state") {
        Piece freshPiece(PieceType::PAWN, Color::WHITE);
        CHECK(freshPiece.hasMoved == false);
    }

    SUBCASE("Setting hasMoved") {
        Piece piece(PieceType::KING, Color::BLACK);
        REQUIRE(piece.hasMoved == false);

        piece.hasMoved = true;
        CHECK(piece.hasMoved == true);

        piece.hasMoved = false;
        CHECK(piece.hasMoved == false);
    }

    SUBCASE("Multiple pieces independent tracking") {
        Piece piece1(PieceType::ROOK, Color::WHITE);
        Piece piece2(PieceType::ROOK, Color::WHITE);

        piece1.hasMoved = true;

        CHECK(piece1.hasMoved == true);
        CHECK(piece2.hasMoved == false);
    }
}