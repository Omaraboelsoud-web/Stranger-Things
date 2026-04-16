#include "turnmanager.h"//includes turnmanager class
#include "player.h"// it allows it to access player functions

// constructor that starts with player's turn
TurnManager::TurnManager() : currentTurn(PlayerTurn) {}

TurnType TurnManager::getCurrentTurn() const { return currentTurn; }// it returns whose turn it is

// starts player's turn
void TurnManager::startPlayerTurn(Player* player) {
    currentTurn = PlayerTurn;// sets the turn to player
    player->resetAP();// reset player's action points
}
// starts enemy's turn
void TurnManager::startEnemyTurn() {
    currentTurn = EnemyTurn;// set turn to enemy
}
// it switches turn between player and enemy
void TurnManager::endTurn() {
    currentTurn = (currentTurn == PlayerTurn) ? EnemyTurn : PlayerTurn;//if its the players turn then it switches to the enemy and if its the enemys turn it switches to the player
}