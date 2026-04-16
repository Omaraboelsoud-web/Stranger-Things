#ifndef POSITION_H
#define POSITION_H

class Position {
public:
    int x, y; // declares both x and y for x-axis and y-axis
    Position(int x = 0, int y = 0); // Creates a parameteriozed constructor with x and y as its parameters
    bool equals(const Position& other) const; // checks if two positions are the same
    int distanceTo(const Position& other) const; // returns distance between two points
};

#endif