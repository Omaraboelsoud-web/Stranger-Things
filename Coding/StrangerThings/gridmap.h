#ifndef GRIDMAP_H
#define GRIDMAP_H

#include <QVector>
#include "tile.h"
#include "position.h"

class GridMap {
private:
    int width, height;
    QVector<QVector<Tile>> tiles;
public:
    GridMap(int w, int h);

    bool isInside(Position p) const;
    bool isWalkable(Position p) const;
    Tile  getTile(Position p) const;
    void  setTile(Position p, Tile t);

    int getWidth()  const;
    int getHeight() const;
};

#endif