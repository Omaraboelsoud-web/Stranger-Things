#include "tile.h" //Includes my custom tile classes which I'll use so makes it easier for me rather than creating to many functions

Tile::Tile(TileType t, bool walk) : type(t), walkable(walk) {} // Defines the constructor of the Tile class and initialized type and walkable
TileType Tile::getType() const  { return type; } // Returns the tile’s type without modifying the object
bool Tile::isWalkable() const   { return walkable; } // Returns whether the tile can be walked on without modifying the object
void Tile::setType(TileType t)  { type = t; } //changes the tile’s type
void Tile::setWalkable(bool w)  { walkable = w; } // Sets whether the tile is walkable
