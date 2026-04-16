#ifndef TRAP_H
#define TRAP_H

#include "position.h" //Includes my custom position classes which I'll use so makes it easier for me rather than creating to many functions

class Character; // declres the character class without including definitions

class Trap { // Declares the Trap class
private:
    Position position; // Stores the location of the trap on the map
    int      damage; // declares the amount of damage
    bool     triggered; // checks whether trap has been pressed on
public:
    Trap(Position p, int dmg); // Parameterized constructor that passed position and damage

    Position getPosition()  const; // Declares a function that returns the trap’s position without modifying the object
    bool     isTriggered()  const; // Checks if trap had been triggered
    void     trigger(Character* c); // Declares a function that activates the trap and interacts with a Character object through a pointer
};

#endif