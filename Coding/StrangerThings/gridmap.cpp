#include "gridmap.h" //Includes my custom gridmap classes which I'll use so makes it easier for me rather than creating to many functions

GridMap::GridMap(int w, int h) : width(w), height(h) { // Defines the constructor and initializes width and height using an initializer list
    tiles.resize(h, QVector<Tile>(w, Tile(Floor, true)));
} // Resizes the outer vector to h rows and initializes each row with w tiles of type Floor and walkable
bool GridMap::isInside(Position p) const { //Defines a function that checks if a position is within map boundaries without modifying the object
    return p.x >= 0 && p.x < width && p.y >= 0 && p.y < height;
} // Returns true if the position’s coordinates are within valid range
bool GridMap::isWalkable(Position p) const { // Defines a function that checks if a position can be walked on without modifying the object
    if (!isInside(p)) return false; // Returns false immediately if the position is outside the map
    return tiles[p.y][p.x].isWalkable();
} // Returns the walkable state of the tile at the given position
Tile GridMap::getTile(Position p) const { // Defines a function that returns the tile at a given position without modifying the object
    return tiles[p.y][p.x];
} // Returns the tile stored at the specified coordinates
void GridMap::setTile(Position p, Tile t) { // Defines a function that updates a tile at a given position
    if (isInside(p)) tiles[p.y][p.x] = t; // Assigns the new tile to the position only if it is inside the map
}
int GridMap::getWidth()  const { return width; } // Returns the width of the map without modifying the object
int GridMap::getHeight() const { return height; } // Returns the height of the map without modifying the object