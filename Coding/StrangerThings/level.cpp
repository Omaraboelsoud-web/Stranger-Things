#include "level.h"//includes level class

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
// map layout:
// # = wall
// P = player
// T = trap
// E = enemy
// D = door
// $ = treasure
// . = empty floor

Level::Level() : completed(false) {// the constructor creates the level and places all game objects
    const int W = 12, H = 9; // the map width and height
    map = new GridMap(W, H);// creates grid map

    for (int x = 0; x < W; x++) {// add walls to the top and bottom borders
        map->setTile(Position(x, 0), Tile(Wall, false));
        map->setTile(Position(x, 8), Tile(Wall, false));
    }

    for (int y = 0; y < H; y++) {//add walls to the right and left borders
            map->setTile(Position(0,    y), Tile(Wall, false));
        map->setTile(Position(W-1,  y), Tile(Wall, false));
    }


    for (int y = 2; y <= 4; y++) map->setTile(Position(2, y), Tile(Wall, false)); // adds vertical wall is at x = 2 from y = 2 to y = 4

    for (int y = 2; y <= 4; y++) map->setTile(Position(9, y), Tile(Wall, false));// adds vertical wall is at x = 9 from y = 2 to y = 4

    map->setTile(Position(3, 4), Tile(Wall, false));  // adds extra walls in row 4
    map->setTile(Position(4, 4), Tile(Wall, false)); // already set

    for (int y = 2; y <= 6; y++) map->setTile(Position(6, y), Tile(Wall, false));//adds vertical wall at x = 6 from y = 2 to y = 6

    for (int x = 7; x <= 9; x++) map->setTile(Position(x, 2), Tile(Wall, false));// adds horizontal wall in row 2 from x = 7 to x = 9

    // adds extra walls at column 9
    map->setTile(Position(9, 3), Tile(Wall, false));
    map->setTile(Position(9, 4), Tile(Wall, false));

    map->setTile(Position(6, 4), Tile(Wall, false));// its the middle wall tile
    // create objects
    trap     = new Trap    (Position(8, 5), 2);// trap is placed at (8,5) and the damage it makes is 2
    door     = new Door    (Position(9, 7));// door placed at (9,7)
    treasure = new Treasure(Position(10, 7));// treasure placed at (10,7)


    map->setTile(door->getPosition(),     Tile(DoorTile,     false));// indicates closed door
    map->setTile(trap->getPosition(),     Tile(TrapTile,     true));// indicates walkable trap
    map->setTile(treasure->getPosition(), Tile(TreasureTile, true));// indicates walkable treasure

    // create player and enemy with name, stats, start position, and ability range
    player = new Player("Eleven", 10, 3, 1, Position(1, 1), 2);
    enemy  = new Enemy ("Papa",   8,  2, 0, Position(9, 6));
}
// destructor that frees allocated memory
Level::~Level() {
    delete map;
    delete player;
    delete enemy;
    delete trap;
    delete door;
    delete treasure;
}

GridMap*  Level::getMap()      const { return map; }// getter for map
Player*   Level::getPlayer()   const { return player; }// getter for player
Enemy*    Level::getEnemy()    const { return enemy; }// getter for enemy
Trap*     Level::getTrap()     const { return trap; }// getter for trap
Door*     Level::getDoor()     const { return door; }// getter for door
Treasure* Level::getTreasure() const { return treasure; }// getter for treasure

bool Level::isCompleted()           const { return completed; }// checks if level is completed
void Level::setCompleted(bool val)        { completed = val; }// indicates whether level is completed or not

// called when enemy is defeated
void Level::onEnemyDefeated() {
    door->unlock();// unlocks the door
    door->openDoor();// opens the door

    map->setTile(door->getPosition(), Tile(DoorTile, true));// makes door tile walkable
}