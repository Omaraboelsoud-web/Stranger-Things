#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <memory>
#include "level.h"
#include "turnmanager.h"
#include "gameview.h"

class Game : public QObject {
    Q_OBJECT

private:
    std::unique_ptr<Level>       level;
    std::unique_ptr<TurnManager> turnManager;
    GameView*                    view;

    bool gameOver;

    void runEnemyTurn();
    void checkWinCondition();
    void checkLoseCondition();

public:
    explicit Game(QObject* parent = nullptr);

    void startGame();
    void restartLevel();

public slots:
    void onMoveRequested(int dx, int dy);
    void onPsychicAttackRequested();
    void onEndTurnRequested();
};

#endif