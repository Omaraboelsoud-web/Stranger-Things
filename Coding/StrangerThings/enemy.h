#ifndef ENEMY_H
#define ENEMY_H

#include <QVector>              // used for storing lists (like multiple enemies or paths)
#include "character.h"          // base class that Enemy inherits common stats/behavior from
#include "gridmap.h"            // gives access to the game map so enemy can move intelligently
#include "tile.h"               // defines tile types (wall, floor, etc.)

class Player;                   // forward declaration so we can use Player pointer without full include
class Enemy : public Character { // Enemy is a type of Character (inherits HP, ATK, DEF, position, etc.)
public:
    Enemy(QString name, int hp, int atk, int def, Position pos); // constructor: creates an enemy with name, stats, and starting position
    void takeTurn(Player* player, GridMap* map, const QVector<Enemy*>& allEnemies);
    // main AI function: controls what enemy does during its turn
    // uses player position + map + other enemies for decisions (move/attack/etc.)
    bool isAdjacentTo(Position p) const; // checks if a given position is next to the enemy (used for melee attacks)
    Position getNextMoveToward(Position target, GridMap* map, const QVector<Enemy*>& allEnemies) const; // AI path decision: calculates best next step toward a target (usually the player)
    // avoids walls, traps, and other enemies using map data
};
#endif