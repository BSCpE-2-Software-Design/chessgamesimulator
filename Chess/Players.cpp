#include "Players.h"
#include <iostream>
#include <iomanip>

// Default constructor
Player::Player()
    : Character(), m_score(0), m_gamesPlayed(0), m_gamesWon(0), m_gamesDrawn(0) {
}

// Parameterized constructor
Player::Player(const std::string& playerName, Color playerColor)
    : Character(playerName, playerColor), m_score(0), m_gamesPlayed(0), m_gamesWon(0), m_gamesDrawn(0) {
}

// Getter methods
int Player::getScore() const {
    return m_score;
}

int Player::getGamesPlayed() const {
    return m_gamesPlayed;
}

int Player::getGamesWon() const {
    return m_gamesWon;
}

int Player::getGamesDrawn() const {
    return m_gamesDrawn;
}

double Player::getWinRate() const {
    if (m_gamesPlayed == 0) {
        return 0.0;
    }
    return (static_cast<double>(m_gamesWon) / m_gamesPlayed) * 100.0;
}

// Setter methods
void Player::setScore(int newScore) {
    m_score = newScore;
}

// Game result methods
void Player::addWin() {
    m_gamesPlayed++;
    m_gamesWon++;
    m_score += 3;  // 3 points for a win (typical chess scoring)
}

void Player::addLoss() {
    m_gamesPlayed++;
    m_score += 0;  // 0 points for a loss
}

void Player::addDraw() {
    m_gamesPlayed++;
    m_gamesDrawn++;
    m_score += 1;  // 1 point for a draw
}

void Player::resetStats() {
    m_score = 0;
    m_gamesPlayed = 0;
    m_gamesWon = 0;
    m_gamesDrawn = 0;
}

// Override virtual methods
void Player::displayInfo() const {
    // Call base class displayInfo first
    Character::displayInfo();

    // Display player-specific information
    std::cout << "================================:" << std::endl;
    std::cout << "Player Statistics:" << std::endl;
    std::cout << "================================:" << std::endl;
    std::cout << "  Score: " << m_score << std::endl;
    std::cout << "  Games Played: " << m_gamesPlayed << std::endl;
    std::cout << "  Games Won: " << m_gamesWon << std::endl;
    std::cout << "  Games Drawn: " << m_gamesDrawn << std::endl;
    std::cout << "  Games Lost: " << (m_gamesPlayed - m_gamesWon - m_gamesDrawn) << std::endl;
    std::cout << "  Win Rate: " << std::fixed << std::setprecision(2) << getWinRate() << "%" << std::endl;
    std::cout << "================================:";
}

std::string Player::getRole() const {
    return "Player";
}

// Additional utility methods
void Player::updateScore(int points) {
    m_score += points;
}