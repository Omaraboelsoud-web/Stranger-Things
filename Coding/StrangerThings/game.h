#ifndef GAME_H
#define GAME_H

#include <QObject> // Built in library that enables stuff like signals & slots, also event handling which will allow the user to interact with the game and make actions
#include <memory> // Built in library that is used for automatic memory management so the game can survive for long without crashing
#include "level.h" //Includes my custom level classes which I'll use so makes it easier for me rather than creating to many functions
#include "turnmanager.h" //Includes my custom turnmanager classes which I'll use so makes it easier for me rather than creating to many functions
#include "gameview.h" //Includes my custom gameview classes which I'll use so makes it easier for me rather than creating to many functions

class Game : public QObject { // Creates a class game then inheris QObjects so it can use signals and slots, // I'll declare all functions and attributes here
    Q_OBJECT // Declares that QObject is used

private:
    std::unique_ptr<Level>       level; // Manages memory and stores the current game level
    std::unique_ptr<TurnManager> turnManager; // Controls memory and handles between switching the turns from enemy to player
    GameView*                    view; // This is a pointer to UI system that shows how the game will talk to the screen and display images also change as the enemy and character take actions
    bool gameOver; // Checks whether the game has ended or no. True for end. False for game still running
    void runEnemyTurn(); // This will run all enemy actions during the enemy's turn
    void checkWinCondition(); // This checks if the player has met the game's win conditions or not yet
    void checkLoseCondition(); // This checks if the player has lost or yet

public:
    explicit Game(QObject* parent = nullptr); // explicit is a C++ function that prevents unwanted automatic conversions when calling constructors. Game is a constructor and QObject* is a pointer to a Qt object partent is the name and will own the game. When I do it = to nullptr it makes it a default value.
    void startGame(); // Starts the game
    void restartLevel(); // Restarts the current level

public slots: // These are the functions that use and respond to the signal events
    void onMoveRequested(int dx, int dy); // Handles player movements
    void onPsychicAttackRequested(); //Handles the special ability of the player
    void onEndTurnRequested(); // Ends the player's turn
};

#endif