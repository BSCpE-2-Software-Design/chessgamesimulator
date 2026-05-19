#pragma once
#include <string>
#include "Attribute.h"
class Character {
protected:
    std::string m_name;
    Color m_color;

public:
    // Constructors
    Character();
    Character(const std::string& playerName, Color playerColor);
    virtual ~Character() = default;

    // Getter methods
    std::string getName() const;
    Color getColor() const;

    // Setter methods
    void setName(const std::string& newName);
    void setColor(Color newColor);

    // Virtual methods for polymorphism
    virtual void displayInfo() const;
    virtual std::string getRole() const;

    // Utility methods
    std::string getColorString() const;
};