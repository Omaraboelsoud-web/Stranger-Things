#ifndef TILE_H
#define TILE_H

enum TileType {
    Floor,
    Wall,
    TrapTile,
    DoorTile,
    TreasureTile,
    ExitTile
};

class Tile {
private:
    TileType type;
    bool walkable;
public:
    Tile(TileType t = Floor, bool walk = true);
    TileType getType() const;
    bool isWalkable() const;
    void setType(TileType t);
    void setWalkable(bool w);
};

#endif