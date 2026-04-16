#ifndef LEVEL_H
#define LEVEL_H

#include "gridmap.h" //Includes my custom gridmap classes which I'll use so makes it easier for me rather than creating to many functions
#include "player.h" //Includes my custom player classes which I'll use so makes it easier for me rather than creating to many functions
#include "enemy.h" //Includes my custom enemy classes which I'll use so makes it easier for me rather than creating to many functions
#include "trap.h" //Includes my custom trap classes which I'll use so makes it easier for me rather than creating to many functions
#include "door.h" //Includes my custom door classes which I'll use so makes it easier for me rather than creating to many functions
#include "treasure.h" //Includes my custom treasure classes which I'll use so makes it easier for me rather than creating to many functions

class Level { // I'll declare all functions and attributes here
private:
    // Theyre pointers so the Level can store and manage them dynamically
    GridMap*  map; // Pointer to the game map
    Player*   player; // Pointer to the player object
    Enemy*    enemy; // Pointer to the enemy object
    Trap*     trap; // Pointer to the trap object
    Door*     door; // Pointer to the door object
    Treasure* treasure; // Pointer to the treasure object
    bool      completed; // Checks whether the level is finished, True is finished false is unfinished
public:
    Level(); // Default constructor to initialize level
    ~Level(); // Destructor to cleans memory and reduce memory leaks
    GridMap*  getMap()      const; // Returns pointer to the map
    Player*   getPlayer()   const; // Returns pointer to the player
    Enemy*    getEnemy()    const; // Returns pointer to the enemy
    Trap*     getTrap()     const; // Returns pointer to the trap
    Door*     getDoor()     const; // Returns pointer to the door
    Treasure* getTreasure() const; // Returns pointer to the treasure
    bool isCompleted()          const; // Returns whether the level is finished
    void setCompleted(bool val); // Sets the level completion status
    void onEnemyDefeated(); // Handles what happens when the enemy dies
};

#endif