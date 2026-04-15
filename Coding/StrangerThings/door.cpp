#include "door.h"

Door::Door(Position p) : position(p), locked(true), isOpenState(false) {}

Position Door::getPosition() const { return position; }
bool     Door::isLocked()    const { return locked; }
bool     Door::isOpen()      const { return isOpenState; }

void Door::unlock()   { locked = false; }
void Door::openDoor() { isOpenState = true; }