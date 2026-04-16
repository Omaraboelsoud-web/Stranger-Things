#include "game.h" //Includes my custom game classes which I'll use so makes it easier for me rather than creating to many functions
#include <QTimer> // This is used for delays like enemy's turn
#include <QApplication> // This is needed to start the QT applicatition its part of the system

Game::Game(QObject* parent) : QObject(parent), gameOver(false) // Inherits this from QObject then gameOver(false) means game starts being active
{
    level       = std::make_unique<Level>(); // This uses the memory to store map, player, enemy, and objects in the level
    turnManager = std::make_unique<TurnManager>(); // This uses the memory to control when the turn is over and who's turn it should be
    view        = new GameView(); // This nitiates the UI display
    // The connect button triggers an event and changes the screen this is what makes the game react
    connect(view, &GameView::moveRequested,         this, &Game::onMoveRequested); // This moves the character
    connect(view, &GameView::psychicAttackRequested, this, &Game::onPsychicAttackRequested); // This makes the character attack
    connect(view, &GameView::endTurnRequested,       this, &Game::onEndTurnRequested); // This ends the turn
}

void Game::startGame() { // This initialized and starts gameplay
    view->initLevel(level.get()); // This shows the level I'm on view-> uses the view function to display
    view->show(); // This makes the window visible on my screen
    turnManager->startPlayerTurn(level->getPlayer()); // This will make sure there is a correct flow in changing turns and reset player AP
    view->redraw(true); // Redraws the screen for the player
    view->showMessage("Defeat Papa, then reach the treasure!"); // Shows this message for the task that the player needs to complete
}

void Game::restartLevel() {
    gameOver = false; // Resets the game so the game can run again
    level    = std::make_unique<Level>(); // Uses the memory to create a complete new level with fresh everything but doesnt change anything
    view->initLevel(level.get()); //Rebuild the visual map using the new level data
    turnManager->startPlayerTurn(level->getPlayer()); // Start the player’s turn and reset their action points
    view->redraw(true); // Update the screen to show the new level for the player
    view->showMessage("Level restarted. Defeat Papa!"); // Show restart message to the player
}

void Game::runEnemyTurn() {
    Enemy* e = level->getEnemy(); // Pointer that points to enemy we use pointer cuz we want to modify the object and not take a copy of it
    Player* p = level->getPlayer(); // Pointer that points to player we use pointer cuz we want to modify the object and not take a copy of it
    if (e->isAlive()) { // if statement that only runs attack if enemy is alive
        e->takeTurn(p, level->getMap());
    } // Enemy attacks
    view->redraw(false); // Update screen false just to say that its the enemy's turn
    checkLoseCondition(); // Check if player died after enemy attack
    if (!gameOver) { // If statement to check if the game is still running
        QTimer::singleShot(400, this, [this]() { // This lets the game wait 400ms before switching turns to prevent errors
            turnManager->startPlayerTurn(level->getPlayer()); // This starts players turn again
            view->redraw(true); // This updates screen for player's turn
            view->showMessage(""); // This clears all messages
        });
    }
}
void Game::checkWinCondition() {
    if (level->getTreasure()->isCollected()) { // if statement to check if treasure has been collected
        level->setCompleted(true); // Mark level as completed if setCompleted is true
        gameOver = true; // This stops the game because gameOver is true
        view->redraw(true); // This updates screen again
        view->showMessage("★  LEVEL COMPLETE!  You escaped the lab!  ★"); // This displays the win message
    }
}
void Game::checkLoseCondition() {
    if (!level->getPlayer()->isAlive()) { // if statement to check if player is dead
        gameOver = true; // this makes game end as gameOver = true
        view->redraw(false); // Doesn't update as you lost
        view->showMessage("✖  ELEVEN HAS FALLEN.  Restarting in 2s..."); // This shows lose message
        QTimer::singleShot(2000, this, &Game::restartLevel); // This restards the level after 2 seconds I chose 2 seconds to give the compiler time to rest to not cause errors
    }
}
void Game::onMoveRequested(int dx, int dy) {
    if (gameOver) return; // if statement that stops if the game is over
    if (turnManager->getCurrentTurn() != PlayerTurn) return; // if statement that only allows movement in player turn and not papa's too
    Player* p = level->getPlayer(); // Pointer that points to player we use pointer cuz we want to modify the object and not take a copy of it
    Enemy*  e = level->getEnemy(); // Pointer that points to enemy we use pointer cuz we want to modify the object and not take a copy of it
    if (!p->canAct()) { // if statement to see if player has no action points
        view->showMessage("No AP left! Press Enter to end your turn."); // This message shows
        return;
    }
    Position dest(p->getPosition().x + dx, p->getPosition().y + dy); // This calculates new position based on keyboard input it just does the position + input
    if (e->isAlive() && dest.equals(e->getPosition())) { // This makes the game easier as it attacks if the player tries to move into enemy
        if (!p->useAP(1)) { view->showMessage("Not enough AP!"); return; } // if statement that if I cant use 1 AP this means I have no AP so it displays a message
        e->takeDamage(p->getAttack()); // This deaks damage to enemy
        view->redraw(true); // Update UI
        if (!e->isAlive()) { // if statement to check if enemy is dead
            level->onEnemyDefeated(); // it handles the death like unlocks the door
            view->showMessage("Papa defeated! The door is now open."); // Shows message when papa is killed
        } else {
            view->showMessage(QString("Hit Papa for %1 damage!").arg(p->getAttack())); // else statement if he is still alive then this only damages him while giving a message of damage done //QString used because it works with showMessage() and .arg shows the amount of damage done
        }
        if (!p->canAct()) { onEndTurnRequested(); return; } //This ends the turn if no AP is left
        return;
    }
    if (!level->getMap()->isWalkable(dest)) { // if statement to check if tile is walkable on
        if (level->getDoor()->isLocked() && dest.equals(level->getDoor()->getPosition())) // This if statement happens as long as door is locked
        {
            view->showMessage("The door is locked. Defeat Papa first!");
        }
        return;
    }
    if (!p->useAP(1)) { view->showMessage("Not enough AP!"); return; } // This just shows that there is not enough AP points for the action player is trying to do
    p->setPosition(dest); //This moves the player to a new destination
    Trap* trap = level->getTrap(); // This passes trap as a pointer and gets trap as an object because I dont want a copy
    if (!trap->isTriggered() && dest.equals(trap->getPosition())) {
        trap->trigger(p);
        view->showMessage(QString("TRAP! Eleven takes 2 damage! HP: %1").arg(p->getHealth())); // This ifstatement shows that if trap is triggered meaning player pressed on it then generate message and cause damage
    }
    Treasure* treasure = level->getTreasure(); // This passes treasure as a pointer and gets treasure as an object because I dont want a copy
    if (!treasure->isCollected() && dest.equals(treasure->getPosition())) { // if statement to see if player reaches treasure
        if (!level->getEnemy()->isAlive()) { // Makes player reach treasure only if enemy is killed
            treasure->collect(); // This collects treasure when I walked on it
            checkWinCondition(); // This shows that player won
            return;
        } else {
            view->showMessage("The treasure is protected while Papa lives!"); // else thats if condition is not met then papa is still alive and message is not changed
        }
    }
    view->redraw(true); // update screen
    checkLoseCondition(); // this checks if the player died
    if (!gameOver && !p->canAct()) {
        onEndTurnRequested();
    } // if statement that automatically ends turn if there are no AP left
}
void Game::onPsychicAttackRequested() {
    if (gameOver) return; // This just tells the compiler to do nothing if the game has ended
    if (turnManager->getCurrentTurn() != PlayerTurn) return; // if statement that only allows attack when its player's turn
    Player* p = level->getPlayer(); // Pointer that points to player we use pointer cuz we want to modify the object and not take a copy of it
    Enemy*  e = level->getEnemy(); // Pointer that points to enemy we use pointer cuz we want to modify the object and not take a copy of it
    if (!p->canAct()) {
        view->showMessage("No AP left! Press Enter to end your turn."); // if statement that checks if player has enough AP if not show this message
        return;
    }
    if (!e->isAlive()) {
        view->showMessage("No target for psychic attack.");
        return; // if statement that doesnt let player attack if enemy is dead
    }
    if (!p->canUsePsychicAttack(e->getPosition(), level->getMap())) {
        view->showMessage("Psychic attack blocked! Papa must be in line of sight (same row/col, range 3).");
        return; // this if statement checks that enemy is in correct line of sight
    }
    if (!p->useAP(1)) return;
    int dmg = p->getAttack() + 1; // psychic attack deals more damage than normal attacks
    e->takeDamage(dmg); // This does damage to enemies as it uses its function
    view->showMessage(QString("✦ PSYCHIC BLAST! Papa takes %1 damage!").arg(dmg));
    view->redraw(true); // Update screen
    if (!e->isAlive()) {
        level->onEnemyDefeated(); // This opens door for player when enemy is dead
        view->showMessage("✦ Papa defeated with psychic power! Door is open.");
    } // This checks if enemy is alive and handles changes then creates a message
    if (!p->canAct()) { onEndTurnRequested(); } // if statement  if theres no AP left end player's turn
}
void Game::onEndTurnRequested() {
    if (gameOver) return; // if statement that does nothing if game ended
    if (turnManager->getCurrentTurn() != PlayerTurn) return; // if statement that only allows ending turn if its player's turn
    turnManager->startEnemyTurn(); // Switch to enemy turn
    view->redraw(false); // Dont update as turn ended but update screen for enemy
    view->showMessage("Papa's turn..."); // Display message showing its enemy's turn
    QTimer::singleShot(300, this, [this]() { runEnemyTurn(); }); // wait 300ms then switch to enemy turn this makes the code run better
}