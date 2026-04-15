#include "trap.h"
#include "character.h"

Trap::Trap(Position p, int dmg) : position(p), damage(dmg), triggered(false) {}

Position Trap::getPosition()  const { return position; }
bool     Trap::isTriggered()  const { return triggered; }

void Trap::trigger(Character* c) {
    if (triggered) return;
    c->takeDamage(damage);
    triggered = true;
}