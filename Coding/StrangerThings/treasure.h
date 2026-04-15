#ifndef TREASURE_H
#define TREASURE_H

#include "position.h"

class Treasure {
private:
    Position position;
    bool     collected;
public:
    Treasure(Position p);

    Position getPosition()  const;
    bool     isCollected()  const;
    void     collect();
};

#endif