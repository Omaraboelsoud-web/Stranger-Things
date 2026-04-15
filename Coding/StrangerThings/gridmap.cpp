#include "gridmap.h"

GridMap::GridMap(int w, int h) : width(w), height(h) {
    tiles.resize(h, QVector<Tile>(w, Tile(Floor, true)));
}

bool GridMap::isInside(Position p) const {
    return p.x >= 0 && p.x < width && p.y >= 0 && p.y < height;
}

bool GridMap::isWalkable(Position p) const {
    if (!isInside(p)) return false;
    return tiles[p.y][p.x].isWalkable();
}

Tile GridMap::getTile(Position p) const {
    return tiles[p.y][p.x];
}

void GridMap::setTile(Position p, Tile t) {
    if (isInside(p)) tiles[p.y][p.x] = t;
}

int GridMap::getWidth()  const { return width; }
int GridMap::getHeight() const { return height; }