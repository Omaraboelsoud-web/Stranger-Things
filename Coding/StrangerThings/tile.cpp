#include "tile.h"

Tile::Tile(TileType t, bool walk) : type(t), walkable(walk) {}

TileType Tile::getType() const  { return type; }
bool Tile::isWalkable() const   { return walkable; }
void Tile::setType(TileType t)  { type = t; }
void Tile::setWalkable(bool w)  { walkable = w; }