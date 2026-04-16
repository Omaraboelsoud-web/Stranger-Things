#ifndef TILE_H
#define TILE_H

enum TileType { // enum used to name a list on constant values

    Floor,
    Wall,
    TrapTile,
    DoorTile,
    TreasureTile,
    ExitTile
};

class Tile {
private:
    TileType type; //This stores what kind of tile it is
    bool walkable; // checks if the tile is walkable
public:
    Tile(TileType t = Floor, bool walk = true); // Parameterized constructor that takes a type and bool this shows what player is on and is it walkable or nah
    TileType getType() const; // this returns the tile type
    bool isWalkable() const; // Returns whether the tile can be walked on
    void setType(TileType t); // Change tile type during gameplay
    void setWalkable(bool w); // this controls movement permission
};

#endif