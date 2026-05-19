#include "doctest.h"
#include "Characterp.h"

TEST_CASE("Character class tests") {

    SUBCASE("Default constructor") {
        Character defaultChar;

        CHECK(defaultChar.getName() == "Unknown");
        CHECK(defaultChar.getColor() == Color::NONE);
        CHECK(defaultChar.getRole() == "Character");
        CHECK(defaultChar.getColorString() == "None");
    }

    SUBCASE("Parameterized constructor") {
        SUBCASE("White character") {
            Character whiteChar("Alice", Color::WHITE);

            CHECK(whiteChar.getName() == "Alice");
            CHECK(whiteChar.getColor() == Color::WHITE);
            CHECK(whiteChar.getColorString() == "White");
            CHECK(whiteChar.getRole() == "Character");
        }

        SUBCASE("Black character") {
            Character blackChar("Bob", Color::BLACK);

            CHECK(blackChar.getName() == "Bob");
            CHECK(blackChar.getColor() == Color::BLACK);
            CHECK(blackChar.getColorString() == "Black");
        }
    }

    SUBCASE("Setter methods") {
        Character testChar;

        SUBCASE("Set name") {
            testChar.setName("TestPlayer");
            CHECK(testChar.getName() == "TestPlayer");

            testChar.setName("AnotherName");
            CHECK(testChar.getName() == "AnotherName");
        }

        SUBCASE("Set color") {
            testChar.setColor(Color::WHITE);
            CHECK(testChar.getColor() == Color::WHITE);
            CHECK(testChar.getColorString() == "White");

            testChar.setColor(Color::BLACK);
            CHECK(testChar.getColor() == Color::BLACK);
            CHECK(testChar.getColorString() == "Black");

            testChar.setColor(Color::NONE);
            CHECK(testChar.getColor() == Color::NONE);
            CHECK(testChar.getColorString() == "None");
        }

        SUBCASE("Multiple property updates") {
            testChar.setName("Updater");
            testChar.setColor(Color::WHITE);

            REQUIRE(testChar.getName() == "Updater");
            REQUIRE(testChar.getColor() == Color::WHITE);

            testChar.setName("Changed");
            testChar.setColor(Color::BLACK);

            CHECK(testChar.getName() == "Changed");
            CHECK(testChar.getColor() == Color::BLACK);
            CHECK(testChar.getColorString() == "Black");
        }
    }

    SUBCASE("Color string conversions") {
        Character whiteChar("Whitey", Color::WHITE);
        Character blackChar("Blacky", Color::BLACK);
        Character noneChar("Noner", Color::NONE);

        CHECK(whiteChar.getColorString() == "White");
        CHECK(blackChar.getColorString() == "Black");
        CHECK(noneChar.getColorString() == "None");
    }

    SUBCASE("Virtual method overrides") {
        Character baseChar("Base", Color::WHITE);

        CHECK(baseChar.getRole() == "Character");
        CHECK_NOTHROW(baseChar.displayInfo());
    }

    SUBCASE("Const correctness") {
        const Character constChar("ConstPlayer", Color::BLACK);

        CHECK(constChar.getName() == "ConstPlayer");
        CHECK(constChar.getColor() == Color::BLACK);
        CHECK(constChar.getColorString() == "Black");
        CHECK(constChar.getRole() == "Character");
        CHECK_NOTHROW(constChar.displayInfo());
    }
}

TEST_CASE("Edge cases for Character class") {

    SUBCASE("Empty name string") {
        Character emptyChar("", Color::WHITE);
        CHECK(emptyChar.getName().empty());
        CHECK(emptyChar.getName() == "");
    }

    SUBCASE("Very long name") {
        std::string longName(1000, 'A');
        Character longChar(longName, Color::BLACK);
        CHECK(longChar.getName() == longName);
    }

    SUBCASE("Multiple instances independent") {
        Character char1("Player1", Color::WHITE);
        Character char2("Player2", Color::BLACK);

        CHECK(char1.getName() != char2.getName());
        CHECK(char1.getColor() != char2.getColor());

        char1.setName("Changed1");
        CHECK(char1.getName() == "Changed1");
        CHECK(char2.getName() == "Player2");
    }
}