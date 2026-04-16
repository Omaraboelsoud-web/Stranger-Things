#ifndef GRIDMAP_H
#define GRIDMAP_H

#include <QVector> // It's Qt's dynamic array and it works well with nested structures
#include "tile.h" //Includes my custom tile classes which I'll use so makes it easier for me rather than creating to many functions
#include "position.h" //Includes my custom position classes which I'll use so makes it easier for me rather than creating to many functions

class GridMap { // I'll declare all functions and attributes here
private:
    int width, height; // declares width, number of colums, and height, number of rows.
    QVector<QVector<Tile>> tiles; // This means a 2D dynamic array which represents the full map
public:
    GridMap(int w, int h); // This is a constructor that creates the grid of the map width = w height = h
    bool isInside(Position p) const; // Checks if the position is inside the map
    bool isWalkable(Position p) const; // Checks if the tile can be walked on
    Tile  getTile(Position p) const; //This returns the tile that is stored at a specific position
    void  setTile(Position p, Tile t); //This replaces the tile with an object that it will be using
    int getWidth()  const; // This returns number of colums
    int getHeight() const; // This returns number of rows
};

#endif