#include "level.h"

/*
  Level 1 map (12 wide x 9 tall).  W=wall, .=floor, T=trap, D=door, $=treasure
  Eleven starts at (1,1), Papa starts at (9,6).

    Row 0:  W W W W W W W W W W W W
    Row 1:  W . . . . . . . . . . W
    Row 2:  W . W . W . W W W W . W
    Row 3:  W . W . . . . . . W . W
    Row 4:  W . W W . . W . . W . W
    Row 5:  W . . . . . W . T . . W
    Row 6:  W . . . . . . . . E . W
    Row 7:  W . . . . . . . . D $ W
    Row 8:  W W W W W W W W W W W W

  E = enemy start (treated as floor)
  T = trap tile (walkable, but hurts)
  D = door (locked wall until Papa dies)
  $ = treasure
*/

Level::Level() : completed(false) {
    const int W = 12, H = 9;
    map = new GridMap(W, H);

    // ---- Build walls ----
    // Row 0 and 8: full walls
    for (int x = 0; x < W; x++) {
        map->setTile(Position(x, 0), Tile(Wall, false));
        map->setTile(Position(x, 8), Tile(Wall, false));
    }
    // Column 0 and 11: full walls
    for (int y = 0; y < H; y++) {
        map->setTile(Position(0,    y), Tile(Wall, false));
        map->setTile(Position(W-1,  y), Tile(Wall, false));
    }

    // Interior walls
    // col 2 rows 2-4
    for (int y = 2; y <= 4; y++) map->setTile(Position(2, y), Tile(Wall, false));
    // col 9 rows 2-4
    for (int y = 2; y <= 4; y++) map->setTile(Position(9, y), Tile(Wall, false));
    // row 4 cols 2-4
    map->setTile(Position(3, 4), Tile(Wall, false));
    map->setTile(Position(4, 4), Tile(Wall, false)); // already set
    // col 6 rows 2-6
    for (int y = 2; y <= 6; y++) map->setTile(Position(6, y), Tile(Wall, false));
    // row 2 cols 7-9
    for (int x = 7; x <= 9; x++) map->setTile(Position(x, 2), Tile(Wall, false));
    // col 9 rows 3-4
    map->setTile(Position(9, 3), Tile(Wall, false));
    map->setTile(Position(9, 4), Tile(Wall, false));
    // row 4 col 6 already done; col 7 row 4
    map->setTile(Position(6, 4), Tile(Wall, false));

    // ---- Objects ----
    trap     = new Trap    (Position(8, 5), 2);
    door     = new Door    (Position(9, 7));
    treasure = new Treasure(Position(10, 7));

    // Door tile is a wall until unlocked
    map->setTile(door->getPosition(),     Tile(DoorTile,     false));
    map->setTile(trap->getPosition(),     Tile(TrapTile,     true));
    map->setTile(treasure->getPosition(), Tile(TreasureTile, true));

    // ---- Characters ----
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
    // Make the door tile walkable so the player can pass through
    map->setTile(door->getPosition(), Tile(DoorTile, true));
}