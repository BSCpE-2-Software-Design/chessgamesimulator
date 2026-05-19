#pragma once
#include "Characterp.h"

class Player : public Character {
private:
    int m_score;
    int m_gamesPlayed;
    int m_gamesWon;
    int m_gamesDrawn;

public:
    // Constructors
    Player();
    Player(const std::string& playerName, Color playerColor);

    // Getter methods
    int getScore() const;
    int getGamesPlayed() const;
    int getGamesWon() const;
    int getGamesDrawn() const;
    double getWinRate() const;

    // Setter methods
    void setScore(int newScore);

    // Game result methods
    void addWin();
    void addLoss();
    void addDraw();
    void resetStats();

    // Override virtual methods
    void displayInfo() const override;
    std::string getRole() const override;

    // Additional utility methods
    void updateScore(int points);
};