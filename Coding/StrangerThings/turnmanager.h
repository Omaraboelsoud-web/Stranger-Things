#ifndef TURNMANAGER_H
#define TURNMANAGER_H

enum TurnType { PlayerTurn, EnemyTurn };

class Player;

class TurnManager {
private:
    TurnType currentTurn;
public:
    TurnManager();

    TurnType getCurrentTurn() const;

    void startPlayerTurn(Player* player);
    void startEnemyTurn();
    void endTurn();
};

#endif