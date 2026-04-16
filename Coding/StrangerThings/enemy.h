#ifndef ENEMY_H
#define ENEMY_H

#include "character.h" //Includes my custom character classes which I'll use so makes it easier for me rather than creating to many functions
#include "gridmap.h" //Includes my custom gridmap classes which I'll use so makes it easier for me rather than creating to many functions

class Player; // This makes sure that player class exists

class Enemy : public Character { // Class Enemy inherrits from class Character, // I'll declare all functions and attributes here
public: // Everything will be accessed throughout the whole program
    Enemy(QString name, int hp, int atk, int def, Position pos); // enemy Constructor with name, health, attack power, defense, and position
    void     takeTurn(Player* player, GridMap* map); // This will control what the enemy does in his turn Player* player makes the enemy attack and GridMap* map makes the enemy move
    bool     isAdjacentTo(Position p) const; //Check if the player is in the attack range, return true if it is and return false if it is not.
    Position getNextMoveToward(Position target, GridMap* map) const; // This will make the AI enemy calculate the next best step to take towards the character. Will use the players location and the gridmap to calculate and see this is like the AI thinking of the enemy
};

#endif