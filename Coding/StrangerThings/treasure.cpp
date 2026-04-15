#include "treasure.h"

Treasure::Treasure(Position p) : position(p), collected(false) {}

Position Treasure::getPosition()  const { return position; }
bool     Treasure::isCollected()  const { return collected; }
void     Treasure::collect()            { collected = true; }