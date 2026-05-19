#include "Characterp.h"
#include <iostream>

// Constructors
Character::Character() : m_name("Unknown"), m_color(Color::NONE) {
}

Character::Character(const std::string& playerName, Color playerColor)
    : m_name(playerName), m_color(playerColor) {
}

// Getter methods
std::string Character::getName() const {
    return m_name;
}

Color Character::getColor() const {
    return m_color;
}

// Setter methods
void Character::setName(const std::string& newName) {
    m_name = newName;
}

void Character::setColor(Color newColor) {
    m_color = newColor;
}

// Virtual methods
void Character::displayInfo() const {
    std::cout << "Name: " << m_name << "\n";
    std::cout << "Color: " << getColorString() << "\n";
}

std::string Character::getRole() const {
    return "Character";
}

// Utility methods
std::string Character::getColorString() const {
    switch (m_color) {
    case Color::WHITE: return "White";
    case Color::BLACK: return "Black";
    default: return "None";
    }
}