#include "trap.h" //Includes my custom trap classes which I'll use so makes it easier for me rather than creating to many functions
#include "character.h" //Includes my custom character classes which I'll use so makes it easier for me rather than creating to many functions

Trap::Trap(Position p, int dmg) : position(p), damage(dmg), triggered(false) {} //Defines the constructor of the Trap class and initialize position, damage, and sets triggered to false using an initializer list
Position Trap::getPosition()  const { return position; } // Returns the trap’s position without modifying the object
bool     Trap::isTriggered()  const { return triggered; } // Returns whether the trap has already been activated
void Trap::trigger(Character* c) { // Defines a function that activates the trap and affects a Character using a pointer
    if (triggered) return; // Stops the function if the trap has already been triggered
    c->takeDamage(damage); //Applies damage to the character by calling its takeDamage function
    triggered = true; // Marks the trap as triggered so it cannot activate again
}