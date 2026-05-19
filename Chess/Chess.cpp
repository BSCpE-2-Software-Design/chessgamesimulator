#include "Board.h"
#include "Players.h"
#include <iostream>
#include <vector>
#include <memory>

int main() {
    // Create players using inheritance
    std::string player1Name, player2Name;

    std::cout << "\n";
    std::cout << "=============================================\n";
    std::cout << "|           CHESS SIMULATOR                 |\n";
    std::cout << "=============================================\n\n";

    std::cout << "Enter name for White player: ";
    std::getline(std::cin, player1Name);
    if (player1Name.empty()) {
        player1Name = "player1";
    }

    std::cout << "Enter name for Black player: ";
    std::getline(std::cin, player2Name);
    if (player2Name.empty()) {
        player2Name = "player2";
    }

    // Create Player objects (derived from Character)
    Player whitePlayer(player1Name, Color::WHITE);
    Player blackPlayer(player2Name, Color::BLACK);

    // Demonstrate polymorphism
    std::vector<Character*> characters;
    characters.push_back(&whitePlayer);
    characters.push_back(&blackPlayer);

    std::cout << "Press Enter To Continue!";
    std::cin.ignore();
    std::cout << "\033[2J\033[1;1H";
    std::cout << "\n=== Player Information ===\n";
    for (auto* character : characters) {
        std::cout << "\nRole: " << character->getRole() << "\n";
        character->displayInfo();
    }

    std::cout << "\nPress Enter to start the game...";
    std::cin.ignore();
    std::cout << "\033[2J\033[1;1H";

    bool playAgain = true;

    while (playAgain) {
        ChessBoard board;

        while (!board.isGameOver()) {
            board.display();
            std::cout << "Press Enter To Continue!";
            std::cin.ignore();
            std::cout << "\033[2J\033[1;1H";
            // Show current player's turn
            if (board.getCurrentTurn() == Color::WHITE) {
                std::cout << "\n" << whitePlayer.getName() << "'s turn (White)!\n";
            }
            else {
                std::cout << "\n" << blackPlayer.getName() << "'s turn (Black)!\n";
            }

            if (!board.selectPieceAndMove()) {
                if (board.isGameOver()) break;
                continue;
            }

            std::cout << "\n  Press Enter to continue...";
            std::cin.ignore();
            std::cin.clear();
            std::cout << "\033[2J\033[1;1H";
        }

        // Update stats based on game result
        // For demonstration, let's assume White won
        std::cout << "\n=== Game Results ===\n";
        std::cout << "White wins!\n";
        whitePlayer.addWin();
        blackPlayer.addLoss();

        // Display updated stats
        whitePlayer.displayInfo();
        blackPlayer.displayInfo();

        std::cout << "\nDo you want to play again? (y/n): ";
        std::string choice;
        std::getline(std::cin, choice);

        if (choice != "y" && choice != "Y") {
            playAgain = false;
        }
    }

    std::cout << "\n==========================================\n";
    std::cout << "      FINAL STATISTICS                    \n";
    std::cout << "==========================================\n";
    whitePlayer.displayInfo();
    blackPlayer.displayInfo();

    std::cout << "\nThanks for playing!\n\n";

    return 0;
}