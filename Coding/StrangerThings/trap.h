#ifndef TRAP_H
#define TRAP_H

#include "position.h" //Includes my custom position classes which I'll use so makes it easier for me rather than creating to many functions

class Character; // Declares the Character class without including the full definitions

class Trap { // Declares the Trap class
private:
    Position position; // Stores the location of the trap on the map
    int      damage; // Stores the amount of damage the trap deals
    bool     triggered; // Checks whether the trap has already been activated

public:
    Trap(Position p, int dmg); // Parameterized constructor that passes position and damage

    Position getPosition()  const; // Declares a function that returns the trap's position without modifying the object
    bool     isTriggered()  const; // Checks if the trap has already been triggered
    void     trigger(Character* c); // Declares a function that activates the trap and interacts with a Character object through a pointer
    void     setTriggered(bool val); // Declares a function that manually changes the trap's triggered state
};

#endif