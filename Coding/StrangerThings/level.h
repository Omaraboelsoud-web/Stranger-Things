#ifndef LEVEL_H
#define LEVEL_H

#include <QVector> // Qt container used to store multiple objects like enemies and traps
#include "gridmap.h" //Includes my custom gridmap classes which I'll use so makes it easier for me rather than creating to many functions
#include "player.h" //Includes my custom player classes which I'll use so makes it easier for me rather than creating to many functions
#include "enemy.h" //Includes my custom enemy classes which I'll use so makes it easier for me rather than creating to many functions
#include "trap.h" //Includes my custom trap classes which I'll use so makes it easier for me rather than creating to many functions
#include "door.h" //Includes my custom door classes which I'll use so makes it easier for me rather than creating to many functions
#include "treasure.h" //Includes my custom treasure classes which I'll use so makes it easier for me rather than creating to many functions

class Level { // I'll declare all functions and attributes here
private:
    // Theyre pointers so the Level can store and manage them dynamically
    GridMap*        map; // Pointer to the game map
    Player*         player; // Pointer to the player object
    QVector<Enemy*> enemies; // Stores multiple enemies in the level
    QVector<Trap*>  traps; // Stores multiple traps in the level
    Door*           door; // Pointer to the door object
    Treasure*       treasure; // Pointer to the treasure object
    bool            completed; // Checks whether the level is finished, True is finished false is unfinished
    int             levelNum; // Stores which level the player is currently on from 1-5

    void buildLevel1(); // Builds and sets up level 1
    void buildLevel2(); // Builds and sets up level 2
    void buildLevel3(); // Builds and sets up level 3
    void buildLevel4(); // Builds and sets up level 4
    void buildLevel5(); // Builds and sets up level 5

    void addBorderWalls(int W, int H);
    // Adds walls around the outside border of the map
    // W is the width and H is the height of the map

public:
    explicit Level(int levelNumber = 1, int playerHP = 10, int playerAP = 2);
    // explicit prevents unwanted automatic conversions
    // levelNumber selects which level layout should load
    // playerHP and playerAP allow player stats to carry between levels

    ~Level(); // Destructor to cleans memory and reduce memory leaks

    GridMap*         getMap()      const; // Returns pointer to the map
    Player*          getPlayer()   const; // Returns pointer to the player

    QVector<Enemy*>& getEnemies();
    // Returns all enemies in the level
    // Mutable reference lets the game modify enemies directly

    const QVector<Enemy*>& getEnemies() const;
    // Returns all enemies without allowing modifications

    QVector<Trap*>&  getTraps(); // Returns all traps in the level

    Door*            getDoor()     const; // Returns pointer to the door
    Treasure*        getTreasure() const; // Returns pointer to the treasure
    int              getLevelNum() const; // Returns the current level number

    bool allEnemiesDead() const;
    // Checks if every enemy in the level has been defeated

    bool isCompleted() const; // Returns whether the level is finished
    void setCompleted(bool val); // Sets the level completion status

    void onEnemyDefeated();
    // Handles events after enemies are defeated
    // Such as unlocking and opening the door

    Enemy* getEnemy() const;
    // Returns the first living enemy
    // If all enemies are dead it returns the first enemy for compatibility with old code

    Trap* getTrap() const;
    // Returns the first trap in the level for backward compatibility
};

#endif