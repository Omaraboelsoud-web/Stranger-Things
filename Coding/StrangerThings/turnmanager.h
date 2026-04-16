#ifndef TURNMANAGER_H
#define TURNMANAGER_H

enum TurnType { PlayerTurn, EnemyTurn }; // Declares an enum TurnType with two possible values representing whose turn it is

class Player; // Declares  class without including its full definition

class TurnManager {
private:
    TurnType currentTurn; //Stores the current turn state using the TurnType enum
public:
    TurnManager(); // default constructor
    TurnType getCurrentTurn() const; //Declares a function that returns the current turn without modifying the object
    void startPlayerTurn(Player* player); // Declares a function that sets the turn to the player and interacts with a Player object through a pointer
    void startEnemyTurn(); // Function that sets the turn to enemy
    void endTurn(); // function that ends turns
};

#endif