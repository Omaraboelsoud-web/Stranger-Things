#ifndef POSITION_H
#define POSITION_H

class Position {
public:
    int x, y;
    Position(int x = 0, int y = 0);
    bool equals(const Position& other) const;
    int distanceTo(const Position& other) const;
};

#endif