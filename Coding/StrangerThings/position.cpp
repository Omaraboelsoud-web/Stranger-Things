#include "position.h"
#include <cstdlib>

Position::Position(int x, int y) : x(x), y(y) {}

bool Position::equals(const Position& other) const {
    return x == other.x && y == other.y;
}

int Position::distanceTo(const Position& other) const {
    return abs(x - other.x) + abs(y - other.y);
}