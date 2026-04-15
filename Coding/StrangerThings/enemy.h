#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"
#include "gridmap.h"

class Player;

class Enemy : public Character {
public:
    Enemy(QString name, int hp, int atk, int def, Position pos);

    void     takeTurn(Player* player, GridMap* map);
    bool     isAdjacentTo(Position p) const;
    Position getNextMoveToward(Position target, GridMap* map) const;
};

#endif