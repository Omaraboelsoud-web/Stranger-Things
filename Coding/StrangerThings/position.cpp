#include "position.h" //Includes my custom position classes which I'll use so makes it easier for me rather than creating to many functions
#include <cstdlib> //includes standard library functions ill use it for abs()

Position::Position(int x, int y) : x(x), y(y) {} // Defines the constructor of the Position class and initialize x and y

bool Position::equals(const Position& other) const { // Defines a function that checks if two positions are equal without modifying the object
    return x == other.x && y == other.y; //Returns true only if both x and y coordinates match
}
int Position::distanceTo(const Position& other) const { //Defines a function that calculates the distance between two positions without modifying the object
    return abs(x - other.x) + abs(y - other.y); // Calculates and returns the Manhattan distance between the two positions using absolute differences
}
// Manhattan distance is way of measuring distance on a grid where you can only move in straight lines (up, down, left, right) — not diagonally.