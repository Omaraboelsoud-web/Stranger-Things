#ifndef DOOR_H
#define DOOR_H

#include "position.h"

class Door {
private:
    Position position;
    bool     locked;
    bool     isOpenState;
public:
    Door(Position p);

    Position getPosition() const;
    bool     isLocked()    const;
    bool     isOpen()      const;

    void unlock();
    void openDoor();
};

#endif