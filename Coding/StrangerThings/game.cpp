#include "game.h"
#include <QTimer>
#include <QApplication>

Game::Game(QObject* parent)
    : QObject(parent), gameOver(false)
{
    level       = std::make_unique<Level>();
    turnManager = std::make_unique<TurnManager>();
    view        = new GameView();

    connect(view, &GameView::moveRequested,         this, &Game::onMoveRequested);
    connect(view, &GameView::psychicAttackRequested, this, &Game::onPsychicAttackRequested);
    connect(view, &GameView::endTurnRequested,       this, &Game::onEndTurnRequested);
}

void Game::startGame() {
    view->initLevel(level.get());
    view->show();
    turnManager->startPlayerTurn(level->getPlayer());
    view->redraw(true);
    view->showMessage("Defeat Papa, then reach the treasure!");
}

void Game::restartLevel() {
    gameOver = false;
    level    = std::make_unique<Level>();
    view->initLevel(level.get());
    turnManager->startPlayerTurn(level->getPlayer());
    view->redraw(true);
    view->showMessage("Level restarted. Defeat Papa!");
}



void Game::runEnemyTurn() {
    Enemy* e = level->getEnemy();
    Player* p = level->getPlayer();

    if (e->isAlive()) {
        e->takeTurn(p, level->getMap());
    }

    view->redraw(false);
    checkLoseCondition();

    if (!gameOver) {

        QTimer::singleShot(400, this, [this]() {
            turnManager->startPlayerTurn(level->getPlayer());
            view->redraw(true);
            view->showMessage("");
        });
    }
}

void Game::checkWinCondition() {
    if (level->getTreasure()->isCollected()) {
        level->setCompleted(true);
        gameOver = true;
        view->redraw(true);
        view->showMessage("★  LEVEL COMPLETE!  You escaped the lab!  ★");
    }
}

void Game::checkLoseCondition() {
    if (!level->getPlayer()->isAlive()) {
        gameOver = true;
        view->redraw(false);
        view->showMessage("✖  ELEVEN HAS FALLEN.  Restarting in 2s...");
        QTimer::singleShot(2000, this, &Game::restartLevel);
    }
}



void Game::onMoveRequested(int dx, int dy) {
    if (gameOver) return;
    if (turnManager->getCurrentTurn() != PlayerTurn) return;

    Player* p = level->getPlayer();
    Enemy*  e = level->getEnemy();

    if (!p->canAct()) {
        view->showMessage("No AP left! Press Enter to end your turn.");
        return;
    }

    Position dest(p->getPosition().x + dx, p->getPosition().y + dy);


    if (e->isAlive() && dest.equals(e->getPosition())) {
        if (!p->useAP(1)) { view->showMessage("Not enough AP!"); return; }
        e->takeDamage(p->getAttack());
        view->redraw(true);

        if (!e->isAlive()) {
            level->onEnemyDefeated();
            view->showMessage("Papa defeated! The door is now open.");
        } else {
            view->showMessage(QString("Hit Papa for %1 damage!").arg(p->getAttack()));
        }


        if (!p->canAct()) { onEndTurnRequested(); return; }
        return;
    }


    if (!level->getMap()->isWalkable(dest)) {
        if (level->getDoor()->isLocked() &&
            dest.equals(level->getDoor()->getPosition()))
        {
            view->showMessage("The door is locked. Defeat Papa first!");
        }
        return;
    }


    if (!p->useAP(1)) { view->showMessage("Not enough AP!"); return; }
    p->setPosition(dest);


    Trap* trap = level->getTrap();
    if (!trap->isTriggered() && dest.equals(trap->getPosition())) {
        trap->trigger(p);
        view->showMessage(QString("TRAP! Eleven takes 2 damage! HP: %1").arg(p->getHealth()));
    }


    Treasure* treasure = level->getTreasure();
    if (!treasure->isCollected() && dest.equals(treasure->getPosition())) {
        if (!level->getEnemy()->isAlive()) {
            treasure->collect();
            checkWinCondition();
            return;
        } else {
            view->showMessage("The treasure is protected while Papa lives!");
        }
    }

    view->redraw(true);
    checkLoseCondition();

    if (!gameOver && !p->canAct()) {
        onEndTurnRequested();
    }
}

void Game::onPsychicAttackRequested() {
    if (gameOver) return;
    if (turnManager->getCurrentTurn() != PlayerTurn) return;

    Player* p = level->getPlayer();
    Enemy*  e = level->getEnemy();

    if (!p->canAct()) {
        view->showMessage("No AP left! Press Enter to end your turn.");
        return;
    }

    if (!e->isAlive()) {
        view->showMessage("No target for psychic attack.");
        return;
    }

    if (!p->canUsePsychicAttack(e->getPosition(), level->getMap())) {
        view->showMessage("Psychic attack blocked! Papa must be in line of sight (same row/col, range 3).");
        return;
    }

    if (!p->useAP(1)) return;


    int dmg = p->getAttack() + 1;
    e->takeDamage(dmg);
    view->showMessage(QString("✦ PSYCHIC BLAST! Papa takes %1 damage!").arg(dmg));
    view->redraw(true);

    if (!e->isAlive()) {
        level->onEnemyDefeated();
        view->showMessage("✦ Papa defeated with psychic power! Door is open.");
    }

    if (!p->canAct()) { onEndTurnRequested(); }
}

void Game::onEndTurnRequested() {
    if (gameOver) return;
    if (turnManager->getCurrentTurn() != PlayerTurn) return;

    turnManager->startEnemyTurn();
    view->redraw(false);
    view->showMessage("Papa's turn...");

    QTimer::singleShot(300, this, [this]() { runEnemyTurn(); });
}