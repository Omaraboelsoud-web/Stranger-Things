#ifndef DOOR_H
#define DOOR_H

#include "position.h" //Includes my custom position classes which I'll use so makes it easier for me rather than creating to many functions

class Door { // I'll declare all functions and attributes here
private: // Private because I want these to be accessed inside the class only
    Position position; // This stores where the door is located on te map
    bool     locked; // This checks if the door can be opened or is still locked. True if locked false if open.
    bool     isOpenState; //This checks whether the door is currently open or not. True if open false if locked
public: // Everything can be used throught the program
    Door(Position p); //Door constructor that takes the position p and sets where the door is on the map
    Position getPosition() const; // Returns the door's position
    bool     isLocked()    const; //Returns whether the door is locked
    bool     isOpen()      const; //Returns whether the door is open
    void unlock(); // This changes the door from locked to unlocked
    void openDoor(); // Changes the door to open so makes the isOpenState true
};

#endif