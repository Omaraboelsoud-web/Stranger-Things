#ifndef TRAP_H
#define TRAP_H

#include "position.h"

class Character;

class Trap {
private:
    Position position;
    int      damage;
    bool     triggered;
public:
    Trap(Position p, int dmg);

    Position getPosition()  const;
    bool     isTriggered()  const;
    void     trigger(Character* c);
};

#endif