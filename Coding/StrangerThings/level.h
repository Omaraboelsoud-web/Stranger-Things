#ifndef LEVEL_H
#define LEVEL_H

#include "gridmap.h"
#include "player.h"
#include "enemy.h"
#include "trap.h"
#include "door.h"
#include "treasure.h"

class Level {
private:
    GridMap*  map;
    Player*   player;
    Enemy*    enemy;
    Trap*     trap;
    Door*     door;
    Treasure* treasure;
    bool      completed;
public:
    Level();
    ~Level();

    GridMap*  getMap()      const;
    Player*   getPlayer()   const;
    Enemy*    getEnemy()    const;
    Trap*     getTrap()     const;
    Door*     getDoor()     const;
    Treasure* getTreasure() const;

    bool isCompleted()          const;
    void setCompleted(bool val);


    void onEnemyDefeated();
};

#endif