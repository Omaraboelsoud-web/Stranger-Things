#ifndef TREASURE_H
#define TREASURE_H

#include "position.h" //Includes my custom position classes which I'll use so makes it easier for me rather than creating to many functions

class Treasure {
private:
    Position position; //Stores the location of the treasure on the map
    bool     collected; // checks whether treasure has been collected
public:
    Treasure(Position p); //Declares a constructor that initializes the treasure’s position

    Position getPosition()  const; // Declares a function that returns the treasure’s position without modifying the object
    bool     isCollected()  const; // checks if treasure has been collected
    void     collect(); // Declares a function that marks the treasure as collected
};

#endif