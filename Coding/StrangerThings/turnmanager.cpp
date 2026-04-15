#include "turnmanager.h"
#include "player.h"

TurnManager::TurnManager() : currentTurn(PlayerTurn) {}

TurnType TurnManager::getCurrentTurn() const { return currentTurn; }

void TurnManager::startPlayerTurn(Player* player) {
    currentTurn = PlayerTurn;
    player->resetAP();
}

void TurnManager::startEnemyTurn() {
    currentTurn = EnemyTurn;
}

void TurnManager::endTurn() {
    currentTurn = (currentTurn == PlayerTurn) ? EnemyTurn : PlayerTurn;
}