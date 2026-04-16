#include "level.h"

/*

y=0   # # # # # # # # # # # #
y=1   # P . . . . . . . . . #
y=2   # . # . . . # . # # # #
y=3   # . # . . . # . . . # #
y=4   # . # # # . # . . . # #
y=5   # . . . . . . . T . . #
y=6   # . . . . . # . . . E #
y=7   # . . . . . . . . D $ #
y=8   # # # # # # # # # # # #
      0 1 2 3 4 5 6 7 8 9 10 11

*/

Level::Level() : completed(false) {
    const int W = 12, H = 9;
    map = new GridMap(W, H);

    for (int x = 0; x < W; x++) {
        map->setTile(Position(x, 0), Tile(Wall, false));
        map->setTile(Position(x, 8), Tile(Wall, false));
    }

    for (int y = 0; y < H; y++) {
        map->setTile(Position(0,    y), Tile(Wall, false));
        map->setTile(Position(W-1,  y), Tile(Wall, false));
    }


    for (int y = 2; y <= 4; y++) map->setTile(Position(2, y), Tile(Wall, false));

    for (int y = 2; y <= 4; y++) map->setTile(Position(9, y), Tile(Wall, false));

    map->setTile(Position(3, 4), Tile(Wall, false));
    map->setTile(Position(4, 4), Tile(Wall, false)); // already set

    for (int y = 2; y <= 6; y++) map->setTile(Position(6, y), Tile(Wall, false));

    for (int x = 7; x <= 9; x++) map->setTile(Position(x, 2), Tile(Wall, false));

    map->setTile(Position(9, 3), Tile(Wall, false));
    map->setTile(Position(9, 4), Tile(Wall, false));

    map->setTile(Position(6, 4), Tile(Wall, false));

    trap     = new Trap    (Position(8, 5), 2);
    door     = new Door    (Position(9, 7));
    treasure = new Treasure(Position(10, 7));


    map->setTile(door->getPosition(),     Tile(DoorTile,     false));
    map->setTile(trap->getPosition(),     Tile(TrapTile,     true));
    map->setTile(treasure->getPosition(), Tile(TreasureTile, true));


    player = new Player("Eleven", 10, 3, 1, Position(1, 1), 2);
    enemy  = new Enemy ("Papa",   8,  2, 0, Position(9, 6));
}

Level::~Level() {
    delete map;
    delete player;
    delete enemy;
    delete trap;
    delete door;
    delete treasure;
}

GridMap*  Level::getMap()      const { return map; }
Player*   Level::getPlayer()   const { return player; }
Enemy*    Level::getEnemy()    const { return enemy; }
Trap*     Level::getTrap()     const { return trap; }
Door*     Level::getDoor()     const { return door; }
Treasure* Level::getTreasure() const { return treasure; }

bool Level::isCompleted()           const { return completed; }
void Level::setCompleted(bool val)        { completed = val; }

void Level::onEnemyDefeated() {
    door->unlock();
    door->openDoor();

    map->setTile(door->getPosition(), Tile(DoorTile, true));
}