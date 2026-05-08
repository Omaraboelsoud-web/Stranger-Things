#include "level.h"

// ============================================================
//  Helper
// ============================================================
void Level::addBorderWalls(int W, int H) { // Defines a helper function that adds walls around the border of the map
    for (int x = 0; x < W; x++) { // Loops over every column in the grid
        map->setTile(Position(x, 0),   Tile(Wall, false)); // Places a wall tile on the top border row
        map->setTile(Position(x, H-1), Tile(Wall, false)); // Places a wall tile on the bottom border row
    }
    for (int y = 0; y < H; y++) { // Loops over every row in the grid
        map->setTile(Position(0,   y), Tile(Wall, false)); // Places a wall tile on the left border column
        map->setTile(Position(W-1, y), Tile(Wall, false)); // Places a wall tile on the right border column
    }
}

// ============================================================
//  Level 1 -- "The Lab" (original layout, one enemy, one trap)
//
//  y=0  # # # # # # # # # # # #
//  y=1  # P . . . . . . . . . #
//  y=2  # . # . . . # . # # # #
//  y=3  # . # . . . # . . . # #
//  y=4  # . # # # . # . . . # #
//  y=5  # . . . . . . . T . . #
//  y=6  # . . . . . # . . . E #
//  y=7  # . . . . . . . . D $ #
//  y=8  # # # # # # # # # # # #
//       0 1 2 3 4 5 6 7 8 9 10 11
// ============================================================
void Level::buildLevel1() { // Defines the function that builds level 1 "The Lab"
    const int W = 12, H = 9; // Sets the width and height of the level grid
    map = new GridMap(W, H); // Creates a new grid map with the specified dimensions
    addBorderWalls(W, H); // Calls the helper to place walls around the entire border

    for (int y = 2; y <= 4; y++) map->setTile(Position(2, y), Tile(Wall, false)); // Places a vertical wall segment at x=2 from y=2 to y=4
    for (int y = 2; y <= 4; y++) map->setTile(Position(9, y), Tile(Wall, false)); // Places a vertical wall segment at x=9 from y=2 to y=4
    map->setTile(Position(3, 4), Tile(Wall, false)); // Places a single wall tile at (3,4) to extend the bottom corridor blocker
    map->setTile(Position(4, 4), Tile(Wall, false)); // Places a single wall tile at (4,4) to complete the horizontal wall segment
    for (int y = 2; y <= 6; y++) map->setTile(Position(6, y), Tile(Wall, false)); // Places a long vertical wall at x=6 splitting the map
    for (int x = 7; x <= 9; x++) map->setTile(Position(x, 2), Tile(Wall, false)); // Places a horizontal wall at y=2 from x=7 to x=9
    map->setTile(Position(9, 3), Tile(Wall, false)); // Extends the right-side wall down to y=3
    map->setTile(Position(9, 4), Tile(Wall, false)); // Extends the right-side wall down to y=4
    map->setTile(Position(6, 4), Tile(Wall, false)); // Places an extra wall tile at (6,4) to close a gap in the center divider

    Trap* t = new Trap(Position(8, 5), 2); // Creates a trap at (8,5) with damage value 2
    traps.append(t); // Adds the trap to the level's trap list
    door     = new Door    (Position(9, 7)); // Creates the exit door at position (9,7)
    treasure = new Treasure(Position(10, 7)); // Places the treasure reward at position (10,7)

    map->setTile(door->getPosition(),     Tile(DoorTile,     false)); // Marks the door's position on the map as a door tile
    map->setTile(t->getPosition(),        Tile(TrapTile,     true)); // Marks the trap's position on the map as a visible trap tile
    map->setTile(treasure->getPosition(), Tile(TreasureTile, true)); // Marks the treasure's position on the map as a visible treasure tile

    player = new Player("Eleven", 10, 3, 1, Position(1, 1), 2); // Spawns the player Eleven with 10 HP, 3 ATK, 1 DEF at position (1,1)
    enemies.append(new Enemy("Papa", 8, 2, 0, Position(9, 6))); // Spawns enemy Papa with 8 HP, 2 ATK, 0 DEF at position (9,6)
}

// ============================================================
//  Level 2 -- "The Upside Down Tunnels"
//  Wider map (14x9), two enemies, two traps, narrower corridors.
//
//  y=0  # # # # # # # # # # # # # #
//  y=1  # P . . # . . . . . . . . #
//  y=2  # . . . # . # # # . . . . #
//  y=3  # . # . . . . . # . . . . #
//  y=4  # . # . . . # . # . T . . #
//  y=5  # . # # # . # . . . . . . #
//  y=6  # . . . . . # . . E1. . . #
//  y=7  # . . T . . . . . . E2. D #
//  y=8  # # # # # # # # # # # # $ #     <- treasure outside door column
//       Actually put $ at (12,7) and D at (11,7)
//  y=8  # # # # # # # # # # # # # #
//       0 1 2 3 4 5 6 7 8 9 0 1 2 3
// ============================================================
void Level::buildLevel2() { // Defines the function that builds level 2 "The Upside Down Tunnels"
    const int W = 14, H = 9; // Sets the width and height of the level grid, wider than level 1
    map = new GridMap(W, H); // Creates a new grid map with the specified dimensions
    addBorderWalls(W, H); // Calls the helper to place walls around the entire border

    // vertical wall x=4 y=1..2
    map->setTile(Position(4, 1), Tile(Wall, false)); // Places the top tile of the vertical wall at x=4
    map->setTile(Position(4, 2), Tile(Wall, false)); // Places the bottom tile of the vertical wall at x=4

    // vertical wall x=2 y=3..5
    for (int y = 3; y <= 5; y++) map->setTile(Position(2, y), Tile(Wall, false)); // Places a vertical wall segment at x=2 from y=3 to y=5

    // horizontal wall y=2 x=6..8
    for (int x = 6; x <= 8; x++) map->setTile(Position(x, 2), Tile(Wall, false)); // Places a horizontal wall segment at y=2 from x=6 to x=8

    // vertical wall x=8 y=3..4
    map->setTile(Position(8, 3), Tile(Wall, false)); // Places the top tile of the vertical wall at x=8
    map->setTile(Position(8, 4), Tile(Wall, false)); // Places the bottom tile of the vertical wall at x=8

    // wall cluster y=5 x=3..4
    map->setTile(Position(3, 5), Tile(Wall, false)); // Places a wall tile at (3,5) to narrow the lower corridor
    map->setTile(Position(4, 5), Tile(Wall, false)); // Places a wall tile at (4,5) completing the corridor blocker

    // vertical wall x=6 y=4..6
    for (int y = 4; y <= 6; y++) map->setTile(Position(6, y), Tile(Wall, false)); // Places a vertical wall segment at x=6 from y=4 to y=6

    // traps
    Trap* t1 = new Trap(Position(10, 4), 2); // Creates first trap at (10,4) with damage value 2
    Trap* t2 = new Trap(Position(3,  7), 2); // Creates second trap at (3,7) with damage value 2
    traps.append(t1); // Adds first trap to the level's trap list
    traps.append(t2); // Adds second trap to the level's trap list

    door     = new Door    (Position(11, 7)); // Creates the exit door at position (11,7)
    treasure = new Treasure(Position(12, 7)); // Places the treasure reward at position (12,7)

    map->setTile(t1->getPosition(),       Tile(TrapTile,     true)); // Marks first trap's position on the map as a visible trap tile
    map->setTile(t2->getPosition(),       Tile(TrapTile,     true)); // Marks second trap's position on the map as a visible trap tile
    map->setTile(door->getPosition(),     Tile(DoorTile,     false)); // Marks the door's position on the map as a door tile
    map->setTile(treasure->getPosition(), Tile(TreasureTile, true)); // Marks the treasure's position on the map as a visible treasure tile

    // Player carries over HP from previous level (passed in constructor)
    player = new Player("Eleven", 10, 3, 1, Position(1, 1), 2); // Spawns the player Eleven at (1,1) with the same base stats as level 1

    // Two enemies -- Demogorgon scout and a lab soldier
    enemies.append(new Enemy("Demogorgon Scout", 7, 2, 0, Position(9,  6))); // Spawns Demogorgon Scout with 7 HP, 2 ATK, 0 DEF at (9,6)
    enemies.append(new Enemy("Lab Soldier",      6, 3, 1, Position(10, 7))); // Spawns Lab Soldier with 6 HP, 3 ATK, 1 DEF at (10,7)
}

// ============================================================
//  Level 3 -- "The Mind Flayer's Lair"
//  Same 14x10 grid, three enemies, three traps, maze-like.
//
//  y=0  # # # # # # # # # # # # # #
//  y=1  # P . . . . . . . . . . . #
//  y=2  # # # # . . # # # # . . . #
//  y=3  # . . . . . . . . # . . . #
//  y=4  # . # # # # # . . # . T . #
//  y=5  # . # . . . . . . . . . . #
//  y=6  # . # . # # # # . . E1. . #
//  y=7  # . . . # . . . . . . E2. #
//  y=8  # T . . # . T . . . . . D #   <- D at (11,8), $ at (12,8)
//  y=9  # # # # # # # # # # # # # #
//       Wait, H=10 so y=9 is bottom wall. Put $ at (12,8) and D at (11,8).
// ============================================================
void Level::buildLevel3() { // Defines the function that builds level 3 "The Mind Flayer's Lair"
    const int W = 14, H = 10; // Sets the width and height of the level grid, taller than level 2
    map = new GridMap(W, H); // Creates a new grid map with the specified dimensions
    addBorderWalls(W, H); // Calls the helper to place walls around the entire border

    // horizontal wall y=2 x=1..3
    for (int x = 1; x <= 3; x++) map->setTile(Position(x, 2), Tile(Wall, false)); // Places a horizontal wall segment at y=2 from x=1 to x=3
    // horizontal wall y=2 x=6..9
    for (int x = 6; x <= 9; x++) map->setTile(Position(x, 2), Tile(Wall, false)); // Places a second horizontal wall segment at y=2 from x=6 to x=9

    // vertical wall x=9 y=3..4
    map->setTile(Position(9, 3), Tile(Wall, false)); // Places the top tile of the vertical wall at x=9
    map->setTile(Position(9, 4), Tile(Wall, false)); // Places the bottom tile of the vertical wall at x=9

    // horizontal wall y=4 x=2..6
    for (int x = 2; x <= 6; x++) map->setTile(Position(x, 4), Tile(Wall, false)); // Places a horizontal wall segment at y=4 from x=2 to x=6

    // vertical wall x=2 y=5..7
    for (int y = 5; y <= 7; y++) map->setTile(Position(2, y), Tile(Wall, false)); // Places a vertical wall segment at x=2 from y=5 to y=7

    // horizontal wall y=6 x=4..7
    for (int x = 4; x <= 7; x++) map->setTile(Position(x, 6), Tile(Wall, false)); // Places a horizontal wall segment at y=6 from x=4 to x=7

    // vertical wall x=4 y=7..8
    map->setTile(Position(4, 7), Tile(Wall, false)); // Places the top tile of the vertical wall at x=4
    map->setTile(Position(4, 8), Tile(Wall, false)); // Places the bottom tile of the vertical wall at x=4

    // traps
    Trap* t1 = new Trap(Position(11, 4), 2); // Creates first trap at (11,4) with damage value 2
    Trap* t2 = new Trap(Position(1,  8), 2); // Creates second trap at (1,8) with damage value 2
    Trap* t3 = new Trap(Position(6,  8), 3); // Creates third trap at (6,8) with damage value 3, more dangerous
    traps.append(t1); // Adds first trap to the level's trap list
    traps.append(t2); // Adds second trap to the level's trap list
    traps.append(t3); // Adds third trap to the level's trap list

    door     = new Door    (Position(11, 8)); // Creates the exit door at position (11,8)
    treasure = new Treasure(Position(12, 8)); // Places the treasure reward at position (12,8)

    map->setTile(t1->getPosition(),       Tile(TrapTile,     true)); // Marks first trap's position on the map as a visible trap tile
    map->setTile(t2->getPosition(),       Tile(TrapTile,     true)); // Marks second trap's position on the map as a visible trap tile
    map->setTile(t3->getPosition(),       Tile(TrapTile,     true)); // Marks third trap's position on the map as a visible trap tile
    map->setTile(door->getPosition(),     Tile(DoorTile,     false)); // Marks the door's position on the map as a door tile
    map->setTile(treasure->getPosition(), Tile(TreasureTile, true)); // Marks the treasure's position on the map as a visible treasure tile

    player = new Player("Eleven", 10, 3, 1, Position(1, 1), 2); // Spawns the player Eleven at (1,1) with the same base stats

    enemies.append(new Enemy("Demogorgon",      10, 3, 0, Position(10, 6))); // Spawns Demogorgon with 10 HP, 3 ATK, 0 DEF at (10,6)
    enemies.append(new Enemy("Shadow Creature",  8, 2, 1, Position(11, 7))); // Spawns Shadow Creature with 8 HP, 2 ATK, 1 DEF at (11,7)
    enemies.append(new Enemy("Flayed Soldier",   7, 2, 0, Position(5,  5))); // Spawns Flayed Soldier with 7 HP, 2 ATK, 0 DEF at (5,5)
}

// ============================================================
//  Level 4 -- "The Starcourt Basement"
//  16x10, four enemies, four traps, tight corridors.
//
//  y=0  # # # # # # # # # # # # # # # #
//  y=1  # P . . . # . . . . . . . . . #
//  y=2  # . . . . # . # # # # . . . . #
//  y=3  # . # # . . . . . . # . . . . #
//  y=4  # . # . . . # # . . # . T . . #
//  y=5  # . # . # . . . . . . . . . . #
//  y=6  # . . . # # # # . . E . . . . #
//  y=7  # . . . # . . . . . . E . . . #
//  y=8  # T . T . . . . . . . . E . D #
//       Put D at (13,8) and $ at (14,8)
//  y=9  # # # # # # # # # # # # # # # #
// ============================================================
void Level::buildLevel4() { // Defines the function that builds level 4 "The Starcourt Basement"
    const int W = 16, H = 10; // Sets the width and height of the level grid, wider than previous levels
    map = new GridMap(W, H); // Creates a new grid map with the specified dimensions
    addBorderWalls(W, H); // Calls the helper to place walls around the entire border

    // vertical wall x=5 y=1..2
    map->setTile(Position(5, 1), Tile(Wall, false)); // Places the top tile of the vertical wall at x=5
    map->setTile(Position(5, 2), Tile(Wall, false)); // Places the bottom tile of the vertical wall at x=5

    // horizontal wall y=2 x=7..10
    for (int x = 7; x <= 10; x++) map->setTile(Position(x, 2), Tile(Wall, false)); // Places a horizontal wall segment at y=2 from x=7 to x=10

    // vertical wall x=10 y=3..4
    map->setTile(Position(10, 3), Tile(Wall, false)); // Places the top tile of the vertical wall at x=10
    map->setTile(Position(10, 4), Tile(Wall, false)); // Places the bottom tile of the vertical wall at x=10

    // wall cluster x=2 y=3..5
    for (int y = 3; y <= 5; y++) map->setTile(Position(2, y), Tile(Wall, false)); // Places a vertical wall segment at x=2 from y=3 to y=5

    // horizontal wall y=4 x=6..7
    map->setTile(Position(6, 4), Tile(Wall, false)); // Places a wall tile at (6,4) to start a narrow corridor blocker
    map->setTile(Position(7, 4), Tile(Wall, false)); // Places a wall tile at (7,4) completing the corridor blocker

    // vertical wall x=4 y=5..6
    map->setTile(Position(4, 5), Tile(Wall, false)); // Places the top tile of the vertical wall at x=4
    map->setTile(Position(4, 6), Tile(Wall, false)); // Places the bottom tile of the vertical wall at x=4

    // horizontal wall y=6 x=5..7
    map->setTile(Position(5, 6), Tile(Wall, false)); // Places a wall tile at (5,6) to begin the lower horizontal barrier
    map->setTile(Position(6, 6), Tile(Wall, false)); // Places a wall tile at (6,6) continuing the horizontal barrier
    map->setTile(Position(7, 6), Tile(Wall, false)); // Places a wall tile at (7,6) completing the horizontal barrier

    // vertical wall x=4 y=7..8
    map->setTile(Position(4, 7), Tile(Wall, false)); // Places the top tile of the lower vertical wall at x=4
    map->setTile(Position(4, 8), Tile(Wall, false)); // Places the bottom tile of the lower vertical wall at x=4

    // traps
    Trap* t1 = new Trap(Position(12, 4), 3); // Creates first trap at (12,4) with damage value 3
    Trap* t2 = new Trap(Position(1,  8), 2); // Creates second trap at (1,8) with damage value 2
    Trap* t3 = new Trap(Position(3,  8), 2); // Creates third trap at (3,8) with damage value 2
    Trap* t4 = new Trap(Position(8,  6), 3); // Creates fourth trap at (8,6) with damage value 3, more dangerous
    traps.append(t1); // Adds first trap to the level's trap list
    traps.append(t2); // Adds second trap to the level's trap list
    traps.append(t3); // Adds third trap to the level's trap list
    traps.append(t4); // Adds fourth trap to the level's trap list

    door     = new Door    (Position(13, 8)); // Creates the exit door at position (13,8)
    treasure = new Treasure(Position(14, 8)); // Places the treasure reward at position (14,8)

    map->setTile(t1->getPosition(),       Tile(TrapTile,     true)); // Marks first trap's position on the map as a visible trap tile
    map->setTile(t2->getPosition(),       Tile(TrapTile,     true)); // Marks second trap's position on the map as a visible trap tile
    map->setTile(t3->getPosition(),       Tile(TrapTile,     true)); // Marks third trap's position on the map as a visible trap tile
    map->setTile(t4->getPosition(),       Tile(TrapTile,     true)); // Marks fourth trap's position on the map as a visible trap tile
    map->setTile(door->getPosition(),     Tile(DoorTile,     false)); // Marks the door's position on the map as a door tile
    map->setTile(treasure->getPosition(), Tile(TreasureTile, true)); // Marks the treasure's position on the map as a visible treasure tile

    player = new Player("Eleven", 10, 3, 1, Position(1, 1), 2); // Spawns the player Eleven at (1,1) with the same base stats

    enemies.append(new Enemy("Flayed Tom",   10, 3, 0, Position(10, 6))); // Spawns Flayed Tom with 10 HP, 3 ATK, 0 DEF at (10,6)
    enemies.append(new Enemy("Flayed Bruce", 10, 3, 1, Position(11, 7))); // Spawns Flayed Bruce with 10 HP, 3 ATK, 1 DEF at (11,7)
    enemies.append(new Enemy("Mind Servant",  8, 2, 0, Position(5,  3))); // Spawns Mind Servant with 8 HP, 2 ATK, 0 DEF at (5,3)
    enemies.append(new Enemy("Lab Guard",     7, 2, 1, Position(12, 8))); // Spawns Lab Guard with 7 HP, 2 ATK, 1 DEF at (12,8)
}

// ============================================================
//  Level 5 -- "The Gate Room" (Final Boss)
//  18x11, five enemies (boss + four guards), four traps.
//  Boss: The Mind Flayer -- high HP, high ATK, high DEF.
//
//  y=0   # # # # # # # # # # # # # # # # # #
//  y=1   # P . . . . . . . . . . . . . . . #
//  y=2   # . # # # . . # # # # # . . . . . #
//  y=3   # . # . . . . . . . . # . . . . . #
//  y=4   # . # . # # # . . . . # . T . . . #
//  y=5   # . . . # . . . . . . . . . . . . #
//  y=6   # . . . # . # # # # . . . . E . . #
//  y=7   # . . . . . # . . . . . . . . E . #
//  y=8   # T . . . T # . BOSS . . . . . . . #
//  y=9   # . . . . . . . . . . . . . D . $ #
//  y=10  # # # # # # # # # # # # # # # # # #
//        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7
// ============================================================
void Level::buildLevel5() { // Defines the function that builds level 5 "The Gate Room", the final boss level
    const int W = 18, H = 11; // Sets the width and height of the level grid, the largest map in the game
    map = new GridMap(W, H); // Creates a new grid map with the specified dimensions
    addBorderWalls(W, H); // Calls the helper to place walls around the entire border

    // vertical wall x=2 y=2..4
    for (int y = 2; y <= 4; y++) map->setTile(Position(2, y), Tile(Wall, false)); // Places a vertical wall segment at x=2 from y=2 to y=4

    // horizontal wall y=2 x=3..4
    map->setTile(Position(3, 2), Tile(Wall, false)); // Places a wall tile at (3,2) to begin the top corridor blocker
    map->setTile(Position(4, 2), Tile(Wall, false)); // Places a wall tile at (4,2) completing the top corridor blocker

    // horizontal wall y=2 x=7..11
    for (int x = 7; x <= 11; x++) map->setTile(Position(x, 2), Tile(Wall, false)); // Places a long horizontal wall segment at y=2 from x=7 to x=11

    // vertical wall x=11 y=3..4
    map->setTile(Position(11, 3), Tile(Wall, false)); // Places the top tile of the vertical wall at x=11
    map->setTile(Position(11, 4), Tile(Wall, false)); // Places the bottom tile of the vertical wall at x=11

    // vertical wall x=4 y=4..6
    for (int y = 4; y <= 6; y++) map->setTile(Position(4, y), Tile(Wall, false)); // Places a vertical wall segment at x=4 from y=4 to y=6

    // horizontal wall y=4 x=5..6
    map->setTile(Position(5, 4), Tile(Wall, false)); // Places a wall tile at (5,4) to begin the mid corridor blocker
    map->setTile(Position(6, 4), Tile(Wall, false)); // Places a wall tile at (6,4) completing the mid corridor blocker

    // vertical wall x=6 y=6..8
    for (int y = 6; y <= 8; y++) map->setTile(Position(6, y), Tile(Wall, false)); // Places a vertical wall segment at x=6 from y=6 to y=8 separating the boss arena

    // horizontal wall y=6 x=7..9
    map->setTile(Position(7, 6), Tile(Wall, false)); // Places a wall tile at (7,6) to begin the boss arena top wall
    map->setTile(Position(8, 6), Tile(Wall, false)); // Places a wall tile at (8,6) continuing the boss arena top wall
    map->setTile(Position(9, 6), Tile(Wall, false)); // Places a wall tile at (9,6) completing the boss arena top wall

    // traps
    Trap* t1 = new Trap(Position(1,  8), 3); // Creates first trap at (1,8) with damage value 3
    Trap* t2 = new Trap(Position(5,  8), 3); // Creates second trap at (5,8) with damage value 3
    Trap* t3 = new Trap(Position(13, 4), 3); // Creates third trap at (13,4) with damage value 3
    Trap* t4 = new Trap(Position(10, 5), 2); // Creates fourth trap at (10,5) with damage value 2
    traps.append(t1); // Adds first trap to the level's trap list
    traps.append(t2); // Adds second trap to the level's trap list
    traps.append(t3); // Adds third trap to the level's trap list
    traps.append(t4); // Adds fourth trap to the level's trap list

    door     = new Door    (Position(14, 9)); // Creates the exit door at position (14,9)
    treasure = new Treasure(Position(16, 9)); // Places the treasure reward at position (16,9)

    map->setTile(t1->getPosition(),       Tile(TrapTile,     true)); // Marks first trap's position on the map as a visible trap tile
    map->setTile(t2->getPosition(),       Tile(TrapTile,     true)); // Marks second trap's position on the map as a visible trap tile
    map->setTile(t3->getPosition(),       Tile(TrapTile,     true)); // Marks third trap's position on the map as a visible trap tile
    map->setTile(t4->getPosition(),       Tile(TrapTile,     true)); // Marks fourth trap's position on the map as a visible trap tile
    map->setTile(door->getPosition(),     Tile(DoorTile,     false)); // Marks the door's position on the map as a door tile
    map->setTile(treasure->getPosition(), Tile(TreasureTile, true)); // Marks the treasure's position on the map as a visible treasure tile

    player = new Player("Eleven", 10, 3, 1, Position(1, 1), 2); // Spawns the player Eleven at (1,1) with the same base stats

    // BOSS: The Mind Flayer -- very tough
    enemies.append(new Enemy("The Mind Flayer", 20, 4, 2, Position(9,  8))); // Spawns the Mind Flayer boss with 20 HP, 4 ATK, 2 DEF at (9,8)
    // Four guards protecting the boss
    enemies.append(new Enemy("Flayed Guard 1",   8, 3, 1, Position(14, 6))); // Spawns Flayed Guard 1 with 8 HP, 3 ATK, 1 DEF at (14,6)
    enemies.append(new Enemy("Flayed Guard 2",   8, 3, 1, Position(15, 7))); // Spawns Flayed Guard 2 with 8 HP, 3 ATK, 1 DEF at (15,7)
    enemies.append(new Enemy("Demodog",          7, 2, 0, Position(3,  5))); // Spawns Demodog with 7 HP, 2 ATK, 0 DEF at (3,5)
    enemies.append(new Enemy("Demodog 2",        7, 2, 0, Position(12, 3))); // Spawns Demodog 2 with 7 HP, 2 ATK, 0 DEF at (12,3)
}

// ============================================================
//  Constructor
// ============================================================
Level::Level(int levelNumber, int /*playerHP*/, int /*playerAP*/)
    : map(nullptr), player(nullptr), door(nullptr), treasure(nullptr),
    completed(false), levelNum(levelNumber) // Initializes all pointers to nullptr and sets completed to false and levelNum to the given level number
{
    switch (levelNumber) { // Checks which level number was passed in to decide which build function to call
    case 2:  buildLevel2(); break; // Builds level 2 if levelNumber is 2
    case 3:  buildLevel3(); break; // Builds level 3 if levelNumber is 3
    case 4:  buildLevel4(); break; // Builds level 4 if levelNumber is 4
    case 5:  buildLevel5(); break; // Builds level 5 if levelNumber is 5
    default: buildLevel1(); break; // Defaults to building level 1 for any other value
    }
}

Level::~Level() { // Defines the destructor that cleans up all dynamically allocated memory
    delete map; // Frees the memory allocated for the grid map
    delete player; // Frees the memory allocated for the player
    for (Enemy* e : enemies) delete e; // Loops over all enemies and frees each one's memory
    for (Trap*  t : traps)   delete t; // Loops over all traps and frees each one's memory
    delete door; // Frees the memory allocated for the door
    delete treasure; // Frees the memory allocated for the treasure
}

// ============================================================
//  Getters
// ============================================================
GridMap*  Level::getMap()      const { return map; } // Returns a pointer to the level's grid map
Player*   Level::getPlayer()   const { return player; } // Returns a pointer to the player object
Door*     Level::getDoor()     const { return door; } // Returns a pointer to the door object
Treasure* Level::getTreasure() const { return treasure; } // Returns a pointer to the treasure object
int       Level::getLevelNum() const { return levelNum; } // Returns the current level number

QVector<Enemy*>& Level::getEnemies()             { return enemies; } // Returns a reference to the list of enemies, allowing modification
const QVector<Enemy*>& Level::getEnemies() const { return enemies; } // Returns a const reference to the list of enemies, read only
QVector<Trap*>&  Level::getTraps()               { return traps; } // Returns a reference to the list of traps, allowing modification

// Legacy single-enemy getter: first living, else first
Enemy* Level::getEnemy() const { // Defines a legacy getter that returns the first living enemy or the first enemy if all are dead
    for (Enemy* e : enemies) // Loops through all enemies in the list
        if (e->isAlive()) return e; // Returns the first enemy that is still alive
    return enemies.isEmpty() ? nullptr : enemies.first(); // Returns nullptr if the list is empty, otherwise returns the first enemy
}

// Legacy single-trap getter
Trap* Level::getTrap() const { // Defines a legacy getter that returns the first trap in the list
    return traps.isEmpty() ? nullptr : traps.first(); // Returns nullptr if no traps exist, otherwise returns the first trap
}

bool Level::allEnemiesDead() const { // Defines a function that checks if every enemy in the level has been defeated
    for (Enemy* e : enemies) // Loops through all enemies in the list
        if (e->isAlive()) return false; // Returns false immediately if any enemy is still alive
    return true; // Returns true only if no living enemies were found
}

bool Level::isCompleted()           const { return completed; } // Returns whether the level has been marked as completed
void Level::setCompleted(bool val)        { completed = val; } // Sets the level's completed state to the given value

void Level::onEnemyDefeated() { // Defines a function called whenever an enemy is defeated to check if the door should open
    // Only open the door when ALL enemies are dead
    if (!allEnemiesDead()) return; // Exits early if there are still living enemies remaining
    door->unlock(); // Unlocks the door now that all enemies have been defeated
    door->openDoor(); // Opens the door to allow the player to proceed
    map->setTile(door->getPosition(), Tile(DoorTile, true)); // Updates the map tile at the door's position to show it as open
}