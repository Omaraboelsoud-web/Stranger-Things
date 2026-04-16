#include "treasure.h" //Includes my custom treasure classes which I'll use so makes it easier for me rather than creating to many functions

Treasure::Treasure(Position p) : position(p), collected(false) {} // Defines the constructor of the Treasure class and nitializes position and sets collected to false using an initializer list
Position Treasure::getPosition()  const { return position; } // Returns the treasure’s position without modifying the object
bool     Treasure::isCollected()  const { return collected; } // Returns whether the treasure has been collected without modifying the object
void     Treasure::collect()            { collected = true; } // Sets the treasure state to collected