#include "door.h" //Includes my custom door classes which I'll use so makes it easier for me rather than creating to many functions

Door::Door(Position p) : position(p), locked(true), isOpenState(false) {}
// This initializes the Door constructors objects
// Takes the objects from the parameters and intialized them to the objects declared in the door.h
Position Door::getPosition() const { return position; } // Returns where the door
bool     Door::isLocked()    const { return locked; } // Returns if the door is locked or no
bool     Door::isOpen()      const { return isOpenState; } // Returns whether the door is currently open or nah
void Door::unlock()   { locked = false; } //Changes the door to unlocked by making locked = false which then gives open
void Door::openDoor() { isOpenState = true; } // Changes the door state to open by making the isOpenState true
// I need both open door and unlock door because one unlocks it with key and the other opens and goes to the next level