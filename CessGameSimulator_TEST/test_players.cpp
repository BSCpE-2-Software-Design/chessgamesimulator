#include "doctest.h"
#include "Players.h"
#include <sstream>
#include <iostream>

TEST_CASE("Player class tests") {

    SUBCASE("Default constructor") {
        Player defaultPlayer;

        CHECK(defaultPlayer.getName() == "Unknown");
        CHECK(defaultPlayer.getColor() == Color::NONE);
        CHECK(defaultPlayer.getRole() == "Player");
        CHECK(defaultPlayer.getScore() == 0);
        CHECK(defaultPlayer.getGamesPlayed() == 0);
        CHECK(defaultPlayer.getGamesWon() == 0);
        CHECK(defaultPlayer.getGamesDrawn() == 0);
        CHECK(defaultPlayer.getWinRate() == 0.0);
    }

    SUBCASE("Parameterized constructor") {
        SUBCASE("White player") {
            Player whitePlayer("Alice", Color::WHITE);

            CHECK(whitePlayer.getName() == "Alice");
            CHECK(whitePlayer.getColor() == Color::WHITE);
            CHECK(whitePlayer.getRole() == "Player");
            CHECK(whitePlayer.getScore() == 0);
            CHECK(whitePlayer.getGamesPlayed() == 0);
            CHECK(whitePlayer.getGamesWon() == 0);
            CHECK(whitePlayer.getGamesDrawn() == 0);
            CHECK(whitePlayer.getWinRate() == 0.0);
        }

        SUBCASE("Black player") {
            Player blackPlayer("Bob", Color::BLACK);

            CHECK(blackPlayer.getName() == "Bob");
            CHECK(blackPlayer.getColor() == Color::BLACK);
            CHECK(blackPlayer.getScore() == 0);
            CHECK(blackPlayer.getGamesPlayed() == 0);
        }
    }

    SUBCASE("Game result methods") {
        Player player("TestPlayer", Color::WHITE);

        SUBCASE("Add win") {
            player.addWin();

            CHECK(player.getGamesPlayed() == 1);
            CHECK(player.getGamesWon() == 1);
            CHECK(player.getGamesDrawn() == 0);
            CHECK(player.getScore() == 3);
            CHECK(player.getWinRate() == 100.0);
        }

        SUBCASE("Add loss") {
            player.addLoss();

            CHECK(player.getGamesPlayed() == 1);
            CHECK(player.getGamesWon() == 0);
            CHECK(player.getGamesDrawn() == 0);
            CHECK(player.getScore() == 0);
            CHECK(player.getWinRate() == 0.0);
        }

        SUBCASE("Add draw") {
            player.addDraw();

            CHECK(player.getGamesPlayed() == 1);
            CHECK(player.getGamesWon() == 0);
            CHECK(player.getGamesDrawn() == 1);
            CHECK(player.getScore() == 1);
            CHECK(player.getWinRate() == 0.0);
        }

        SUBCASE("Multiple games mixed results") {
            player.addWin();   
            player.addDraw();  
            player.addLoss();  

            REQUIRE(player.getGamesPlayed() == 3);
            REQUIRE(player.getScore() == 4);

            CHECK(player.getGamesWon() == 1);
            CHECK(player.getGamesDrawn() == 1);

            double expectedWinRate = (1.0 / 3.0) * 100.0;
            CHECK(player.getWinRate() == doctest::Approx(expectedWinRate));
        }
    }

    SUBCASE("Score management") {
        Player player("ScoreTest", Color::WHITE);

        SUBCASE("Set score directly") {
            player.setScore(15);
            REQUIRE(player.getScore() == 15);

            player.setScore(-5);
            CHECK(player.getScore() == -5);
        }

        SUBCASE("Update score with points") {
            player.updateScore(10);
            REQUIRE(player.getScore() == 10);

            player.updateScore(-3);
            CHECK(player.getScore() == 7);

            player.updateScore(0);
            CHECK(player.getScore() == 7);
        }

        SUBCASE("Score accumulates with games") {
            player.addWin();  
            player.addDraw();  
            player.addWin();   

            REQUIRE(player.getScore() == 7);
            REQUIRE(player.getGamesPlayed() == 3);
            CHECK(player.getGamesWon() == 2);
            CHECK(player.getGamesDrawn() == 1);
        }
    }

    SUBCASE("Reset statistics") {
        Player player("ResetTest", Color::BLACK);

        
        player.addWin();
        player.addDraw();
        player.addLoss();

        REQUIRE(player.getGamesPlayed() == 3);
        REQUIRE(player.getScore() == 4);
        REQUIRE(player.getGamesWon() == 1);
        REQUIRE(player.getGamesDrawn() == 1);

        player.resetStats();

        CHECK(player.getScore() == 0);
        CHECK(player.getGamesPlayed() == 0);
        CHECK(player.getGamesWon() == 0);
        CHECK(player.getGamesDrawn() == 0);
        CHECK(player.getWinRate() == 0.0);

       
        REQUIRE(player.getName() == "ResetTest");
        CHECK(player.getColor() == Color::BLACK);
    }

    SUBCASE("Win rate calculations") {
        Player player("WinRateTest", Color::WHITE);

        SUBCASE("Zero games played") {
            REQUIRE(player.getGamesPlayed() == 0);
            CHECK(player.getWinRate() == 0.0);
        }

        SUBCASE("100% win rate") {
            player.addWin();
            player.addWin();
            player.addWin();

            REQUIRE(player.getGamesPlayed() == 3);
            REQUIRE(player.getGamesWon() == 3);
            CHECK(player.getWinRate() == 100.0);
        }

        SUBCASE("0% win rate with draws and losses") {
            player.addDraw();
            player.addLoss();
            player.addDraw();

            REQUIRE(player.getGamesPlayed() == 3);
            REQUIRE(player.getGamesWon() == 0);
            CHECK(player.getWinRate() == 0.0);
        }

        SUBCASE("Mixed win rate") {
            player.addWin();   
            player.addDraw();  
            player.addLoss();  
            player.addWin();  

            REQUIRE(player.getGamesPlayed() == 4);
            REQUIRE(player.getGamesWon() == 2);
            CHECK(player.getWinRate() == doctest::Approx(50.0));
        }
    }

    SUBCASE("Display information") {
        Player player("DisplayTest", Color::WHITE);
        player.addWin();
        player.addDraw();

        REQUIRE(player.getGamesPlayed() == 2);
        REQUIRE(player.getScore() == 4);

        std::stringstream output;
        std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());

        player.displayInfo();

        std::cout.rdbuf(oldCout);
        std::string result = output.str();

        CHECK(result.find("Name: DisplayTest") != std::string::npos);
        CHECK(result.find("Color: White") != std::string::npos);
        CHECK(result.find("Player Statistics") != std::string::npos);
        CHECK(result.find("Score: 4") != std::string::npos);
        CHECK(result.find("Games Played: 2") != std::string::npos);
        CHECK(result.find("Games Won: 1") != std::string::npos);
        CHECK(result.find("Games Drawn: 1") != std::string::npos);
        CHECK(result.find("Games Lost: 0") != std::string::npos);
        CHECK(result.find("Win Rate: 50.00%") != std::string::npos);
    }

    SUBCASE("Virtual function overrides") {
        Player player("VirtualTest", Color::BLACK);

        REQUIRE(player.getRole() == "Player");
        Character* basePtr = &player;
        CHECK(basePtr->getRole() == "Player");
    }

    SUBCASE("Inheritance from Character") {
        Player player("InheritTest", Color::WHITE);

        player.setName("NewName");
        REQUIRE(player.getName() == "NewName");

        player.setColor(Color::BLACK);
        CHECK(player.getColor() == Color::BLACK);
        CHECK(player.getColorString() == "Black");
    }

    SUBCASE("Games lost calculation") {
        Player player("LossTest", Color::WHITE);

        player.addWin();   
        player.addDraw(); 
        player.addLoss();  
        player.addLoss(); 
        player.addWin();   

        REQUIRE(player.getGamesPlayed() == 5);
        REQUIRE(player.getGamesWon() == 2);
        REQUIRE(player.getGamesDrawn() == 1);

        int gamesLost = player.getGamesPlayed() - player.getGamesWon() - player.getGamesDrawn();
        CHECK(gamesLost == 2);
    }
}

TEST_CASE("Player edge cases") {

    SUBCASE("Negative score handling") {
        Player player;
        player.setScore(-10);
        REQUIRE(player.getScore() == -10);

        player.updateScore(-5);
        CHECK(player.getScore() == -15);
    }

    SUBCASE("Large number of games") {
        Player player;

        for (int i = 0; i < 1000; i++) {
            player.addWin();
        }

        REQUIRE(player.getGamesPlayed() == 1000);
        REQUIRE(player.getGamesWon() == 1000);
        CHECK(player.getScore() == 3000);
        CHECK(player.getWinRate() == 100.0);
    }

    SUBCASE("Zero division prevention") {
        Player player;
        REQUIRE(player.getGamesPlayed() == 0);
        CHECK_NOTHROW(player.getWinRate());
        CHECK(player.getWinRate() == 0.0);

        player.addLoss();
        REQUIRE(player.getGamesPlayed() == 1);
        REQUIRE(player.getGamesWon() == 0);
        CHECK(player.getWinRate() == 0.0);
    }

    SUBCASE("Multiple resets") {
        Player player("Reset", Color::WHITE);

        player.addWin();
        REQUIRE(player.getScore() == 3);
        REQUIRE(player.getGamesPlayed() == 1);

        player.resetStats();
        CHECK(player.getScore() == 0);
        CHECK(player.getGamesPlayed() == 0);

        player.addDraw();
        REQUIRE(player.getScore() == 1);
        REQUIRE(player.getGamesPlayed() == 1);

        player.resetStats();
        CHECK(player.getScore() == 0);
        CHECK(player.getGamesPlayed() == 0);
    }
}