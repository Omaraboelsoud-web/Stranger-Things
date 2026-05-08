#include "game.h" // Includes my custom game class which handles all the core game logic
#include <QTimer> // This is used for delays like the enemy's turn waiting before it runs
#include <QApplication> // This is needed to start the Qt application, its part of the system
#include <QFile> // This is used for creating and reading save files on disk
#include <QDir> // This is used for creating folders if they dont exist yet
#include <QTextStream> // This is used to read and write text inside files line by line
#include <QStandardPaths> // This gives a safe location on the computer to store save files

Game::Game(QObject* parent)
    : QObject(parent), gameOver(false), currentLevelNum(1) // Inherits from QObject, gameOver starts as false meaning game is active, and currentLevelNum starts at 1
{
    level       = std::make_unique<Level>(currentLevelNum); // Allocates memory and loads the first level including map, player, enemies, and objects
    turnManager = std::make_unique<TurnManager>(); // Allocates memory and creates the turn manager that controls who acts when
    view        = new GameView(); // Creates the visual game window that shows everything on screen

    // Each connect call links a signal from the view to a slot in game so the game reacts to player input
    connect(view, &GameView::moveRequested,          this, &Game::onMoveRequested); // Triggers movement when player presses a direction key
    connect(view, &GameView::psychicAttackRequested,  this, &Game::onPsychicAttackRequested); // Triggers psychic attack when player presses Q
    connect(view, &GameView::endTurnRequested,        this, &Game::onEndTurnRequested); // Triggers end of turn when player presses Enter or Space
    connect(view, &GameView::saveRequested,           this, &Game::onSaveRequested); // Triggers save when player presses F5
    connect(view, &GameView::loadRequested,           this, &Game::onLoadRequested); // Triggers load when player presses F9
}

void Game::startGame() { // Initializes and starts the game from the beginning
    view->initLevel(level.get()); // Passes the current level to the view so it can build and display the map
    view->show(); // Makes the game window visible on screen
    turnManager->startPlayerTurn(level->getPlayer()); // Begins the player's first turn and resets their action points
    view->redraw(true); // Draws the screen for the first time with the player going first

    view->showMessage( // Displays a different intro message depending on which level the player is starting
        QString("Level %1 -- %2").arg(currentLevelNum).arg(
            currentLevelNum == 1 ? "Escape the Lab!" : // Level 1 intro message
                currentLevelNum == 2 ? "Navigate the Upside Down!" : // Level 2 intro message
                currentLevelNum == 3 ? "Face the Mind Flayer's Lair!" : // Level 3 intro message
                currentLevelNum == 4 ? "Fight through Starcourt Basement!" : // Level 4 intro message
                "FINAL LEVEL: Defeat the Mind Flayer!" // Final level intro message
            )
        );
}

void Game::restartLevel() { // Resets and rebuilds the current level from scratch after the player dies
    gameOver = false; // Resets gameOver to false so the game loop can run again
    level    = std::make_unique<Level>(currentLevelNum); // Creates a completely fresh copy of the current level
    view->initLevel(level.get()); // Rebuilds the visual map using the new level data
    turnManager->startPlayerTurn(level->getPlayer()); // Starts the player's turn and resets their action points
    view->redraw(true); // Updates the screen to show the freshly restarted level
    view->showMessage(QString("Level %1 restarted.").arg(currentLevelNum)); // Shows a restart message with the current level number
}

void Game::advanceLevel() { // Moves the player forward to the next level after completing the current one
    if (currentLevelNum >= 5) { // Checks if the player just finished the final level
        gameOver = true; // Ends the game since all levels are complete
        view->redraw(true); // Updates the screen one last time
        view->showMessage("*** YOU CLOSED THE GATE! Hawkins is saved! ***"); // Shows the final victory message
        return;
    }

    currentLevelNum++; // Increments to the next level number
    gameOver = false; // Resets gameOver so the next level can run normally

    level    = std::make_unique<Level>(currentLevelNum); // Loads the next level into memory
    view->initLevel(level.get()); // Initializes the visual display for the new level
    turnManager->startPlayerTurn(level->getPlayer()); // Starts the player's turn on the new level
    view->redraw(true); // Updates the screen to show the new level

    view->showMessage( // Shows a different intro message for each new level the player advances to
        QString("Level %1 -- %2").arg(currentLevelNum).arg(
            currentLevelNum == 2 ? "Upside Down Tunnels!" : // Level 2 advance message
                currentLevelNum == 3 ? "The Mind Flayer's Lair!" : // Level 3 advance message
                currentLevelNum == 4 ? "Starcourt Basement!" : // Level 4 advance message
                "FINAL LEVEL: Defeat the Mind Flayer!" // Final level advance message
            )
        );
}

void Game::runEnemyTurn() { // Runs all enemy actions one by one during the enemy's turn
    Player* p = level->getPlayer(); // Gets a pointer to the player so enemies can target and damage them

    for (Enemy* e : level->getEnemies()) { // Loops through every enemy in the current level
        if (e->isAlive()) { // Only lets the enemy act if it is still alive
            e->takeTurn(p, level->getMap(), level->getEnemies()); // Enemy either attacks the player or moves toward them
        }

        if (!p->isAlive()) break; // Stops the loop immediately if the player dies mid-turn
    }

    view->redraw(false); // Updates the screen to show the result of the enemy's actions, false means it's the enemy's turn
    checkLoseCondition(); // Checks if the player died after all enemies have acted

    if (!gameOver) { // Only continues if the game is still running after the enemy turn
        QTimer::singleShot(400, this, [this]() { // Waits 400ms before switching turns to prevent visual glitches
            turnManager->startPlayerTurn(level->getPlayer()); // Starts the player's turn again and resets their AP
            view->redraw(true); // Updates the screen showing it is now the player's turn
            view->showMessage(""); // Clears any leftover messages from the enemy turn
        });
    }
}

void Game::checkWinCondition() { // Checks if the player has met the win condition by collecting the treasure
    if (level->getTreasure()->isCollected()) { // Checks if the treasure has been picked up
        level->setCompleted(true); // Marks the current level as completed
        gameOver = true; // Stops the current level's game loop
        view->redraw(true); // Updates the screen to reflect the win state

        if (currentLevelNum < 5) { // Checks if there are more levels to go
            view->showMessage(
                QString("** LEVEL %1 COMPLETE! Advancing...").arg(currentLevelNum) // Shows the level complete message with the level number
                );

            QTimer::singleShot(2000, this, &Game::advanceLevel); // Waits 2 seconds then loads the next level
        } else {
            view->showMessage("*** YOU CLOSED THE GATE! Hawkins is saved! ***"); // Shows the final win message if all levels are done
        }
    }
}

void Game::checkLoseCondition() { // Checks if the player has died and handles the game over state
    if (!level->getPlayer()->isAlive()) { // Checks if the player's HP has dropped to zero or below
        gameOver = true; // Sets gameOver to true to stop further player actions
        view->redraw(false); // Updates the screen to reflect the player's death
        view->showMessage("ELEVEN HAS FALLEN. Restarting in 2s..."); // Shows the death message on the HUD

        QTimer::singleShot(2000, this, &Game::restartLevel); // Waits 2 seconds then restarts the level giving the player a moment to see the message
    }
}

void Game::onMoveRequested(int dx, int dy) { // Handles all player movement and collision detection when a direction key is pressed
    if (gameOver) return; // Does nothing if the game has already ended
    if (turnManager->getCurrentTurn() != PlayerTurn) return; // Only allows movement during the player's turn

    Player* p = level->getPlayer(); // Gets a pointer to the player so we can modify their position and stats

    if (!p->canAct()) { // Checks if the player has any AP left to spend
        view->showMessage("No AP left! Press Enter to end your turn."); // Shows a message if the player is out of AP
        return;
    }

    Position dest(p->getPosition().x + dx, p->getPosition().y + dy); // Calculates the destination tile by adding the input direction to the current position

    for (Enemy* e : level->getEnemies()) { // Loops through all enemies to check if the player is walking into one
        if (e->isAlive() && dest.equals(e->getPosition())) { // Checks if the destination tile has a living enemy on it

            if (!p->useAP(1)) { view->showMessage("Not enough AP!"); return; } // Tries to spend 1 AP for the attack and stops if there is not enough

            e->takeDamage(p->getAttack()); // Deals the player's attack damage to the enemy
            view->redraw(true); // Updates the screen to show the damage

            if (!e->isAlive()) { // Checks if the enemy just died from the attack
                level->onEnemyDefeated(); // Handles the enemy death like potentially unlocking the door

                if (level->allEnemiesDead()) { // Checks if every enemy in the level is now dead
                    view->showMessage("All enemies defeated! The door is now open.");
                } else {
                    int remaining = 0; // Counter that will store how many enemies are still alive

                    for (Enemy* en : level->getEnemies()) // Loops through all enemies to count the living ones
                        if (en->isAlive()) remaining++; // Increments the counter for each living enemy

                    view->showMessage(
                        QString("Enemy down! %1 enemies remaining.").arg(remaining) // Shows how many enemies are left
                        );
                }

            } else {
                view->showMessage(
                    QString("Hit %1 for %2 damage!").arg(e->getName()).arg(p->getAttack()) // Shows the enemy name and how much damage was dealt
                    );
            }

            if (!p->canAct()) { onEndTurnRequested(); return; } // Automatically ends the turn if the player used their last AP on this attack
            return;
        }
    }

    if (!level->getMap()->isWalkable(dest)) { // Checks if the destination tile is blocked like a wall or locked door

        if (level->getDoor()->isLocked() && dest.equals(level->getDoor()->getPosition())) { // Checks specifically if the player tried to walk into the locked door
            view->showMessage("The door is locked. Defeat all enemies first!");
        }

        return;
    }

    if (!p->useAP(1)) { view->showMessage("Not enough AP!"); return; } // Spends 1 AP to move and stops if there is not enough

    p->setPosition(dest); // Moves the player to the new destination tile

    for (Trap* trap : level->getTraps()) { // Loops through all traps to check if the player stepped on one
        if (!trap->isTriggered() && dest.equals(trap->getPosition())) { // Checks if the trap hasnt been triggered yet and the player is on it
            trap->trigger(p); // Activates the trap and applies damage to the player

            view->showMessage(
                QString("TRAP! Eleven takes damage! HP: %1").arg(p->getHealth()) // Shows the player's remaining HP after trap damage
                );
        }
    }

    Treasure* treasure = level->getTreasure(); // Gets a pointer to the treasure object so we can check and collect it

    if (!treasure->isCollected() && dest.equals(treasure->getPosition())) { // Checks if the player walked onto the uncollected treasure tile

        if (level->allEnemiesDead()) { // Only allows collecting the treasure if all enemies are dead
            treasure->collect(); // Collects the treasure and marks it as taken
            checkWinCondition(); // Checks if collecting the treasure triggers the win
            return;
        } else {
            view->showMessage("The treasure is protected while enemies live!"); // Shows a message if enemies are still alive
        }
    }

    view->redraw(true); // Updates the screen after the player moves
    checkLoseCondition(); // Checks if the player died from a trap or other damage

    if (!gameOver && !p->canAct()) {
        onEndTurnRequested(); // Automatically ends the turn if the player has no AP left after moving
    }
}

void Game::onPsychicAttackRequested() { // Handles the player's special psychic attack ability triggered by pressing Q
    if (gameOver) return; // Does nothing if the game has already ended
    if (turnManager->getCurrentTurn() != PlayerTurn) return; // Only allows the attack during the player's turn

    Player* p = level->getPlayer(); // Gets a pointer to the player so we can check AP and deal damage

    if (!p->canAct()) {
        view->showMessage("No AP left! Press Enter to end your turn."); // Shows a message if the player has no AP left to use
        return;
    }

    Enemy* target = nullptr; // Creates an empty enemy pointer that will be filled once a valid target is found

    for (Enemy* e : level->getEnemies()) { // Loops through all enemies to find one in range
        if (e->isAlive() && p->canUsePsychicAttack(e->getPosition(), level->getMap())) { // Checks if the enemy is alive and in line of sight within range 3
            target = e; // Saves the first valid enemy as the target
            break; // Stops searching after finding the first valid target
        }
    }

    if (!target) {
        view->showMessage("Psychic attack blocked! Enemy must be in line of sight (same row/col, range 3).");
        return; // Stops if no valid target was found in range
    }

    if (!p->useAP(1)) return; // Tries to spend 1 AP and stops silently if there is not enough

    int dmg = p->getAttack() + 1; // Psychic attack deals 1 extra damage compared to a normal melee attack

    target->takeDamage(dmg); // Applies the psychic damage to the target enemy

    view->showMessage(
        QString("PSYCHIC BLAST! %1 takes %2 damage!").arg(target->getName()).arg(dmg) // Shows the target name and how much psychic damage was dealt
        );

    view->redraw(true); // Updates the screen to show the damage result

    if (!target->isAlive()) { // Checks if the psychic attack killed the enemy

        level->onEnemyDefeated(); // Handles the enemy death like potentially unlocking the door

        if (level->allEnemiesDead()) {
            view->showMessage("All enemies defeated with psychic power! Door is open.");
        } else {

            int remaining = 0; // Counter that stores how many enemies are still alive

            for (Enemy* en : level->getEnemies()) // Loops through all enemies to count the living ones
                if (en->isAlive()) remaining++; // Increments counter for each living enemy

            view->showMessage(
                QString("Enemy defeated with psychic power! %1 remaining.").arg(remaining) // Shows remaining enemy count
                );
        }
    }

    if (!p->canAct()) { onEndTurnRequested(); } // Automatically ends the turn if the player has no AP left
}

void Game::onEndTurnRequested() { // Handles ending the player's turn and handing control over to the enemies
    if (gameOver) return; // Does nothing if the game has already ended
    if (turnManager->getCurrentTurn() != PlayerTurn) return; // Only allows ending the turn during the player's turn

    turnManager->startEnemyTurn(); // Switches the turn state to the enemy's turn
    view->redraw(false); // Updates the screen to reflect that it is now the enemy's turn
    view->showMessage("Enemy turn..."); // Displays a message telling the player the enemy is now acting

    QTimer::singleShot(300, this, [this]() { runEnemyTurn(); }); // Waits 300ms before running enemy actions so the screen update has time to render
}

QString Game::saveFilePath() const { // Returns the full file path where the save file will be stored
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
           + "/savegame.txt"; // Appends the save file name to the app's writable storage location
}

void Game::saveGame() { // Writes all current game state to a text file so it can be loaded later

    QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)); // Creates the save folder if it doesnt already exist

    QFile file(saveFilePath()); // Creates a file object pointing to the save file path

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) { // Tries to open the file for writing and stops if it fails
        view->showMessage("Save FAILED -- could not write file.");
        return;
    }

    QTextStream out(&file); // Creates a text stream so we can write lines of text into the file easily
    Player* p = level->getPlayer(); // Gets a pointer to the player to read their current stats

    out << "level="  << currentLevelNum    << "\n"; // Saves the current level number
    out << "px="     << p->getPosition().x  << "\n"; // Saves the player's x position on the grid
    out << "py="     << p->getPosition().y  << "\n"; // Saves the player's y position on the grid
    out << "php="    << p->getHealth()      << "\n"; // Saves the player's current HP
    out << "pap="    << p->getAP()          << "\n"; // Saves the player's current AP

    const QVector<Enemy*>& enemies = level->getEnemies(); // Gets a reference to all enemies in the level

    out << "enemyCount=" << enemies.size() << "\n"; // Saves the total number of enemies in this level

    for (int i = 0; i < enemies.size(); i++) { // Loops through each enemy and saves their state

        Enemy* e = enemies[i]; // Gets the current enemy from the list

        out << "e" << i << "_alive=" << (e->isAlive() ? 1 : 0) << "\n"; // Saves whether the enemy is alive as 1 or dead as 0
        out << "e" << i << "_x="     << e->getPosition().x      << "\n"; // Saves the enemy's x position
        out << "e" << i << "_y="     << e->getPosition().y      << "\n"; // Saves the enemy's y position
        out << "e" << i << "_hp="    << e->getHealth()           << "\n"; // Saves the enemy's current HP
    }

    const QVector<Trap*>& traps = level->getTraps(); // Gets a reference to all traps in the level

    out << "trapCount=" << traps.size() << "\n"; // Saves the total number of traps in this level

    for (int i = 0; i < traps.size(); i++) { // Loops through each trap and saves whether it was already triggered
        out << "t" << i << "_triggered=" << (traps[i]->isTriggered() ? 1 : 0) << "\n"; // Saves 1 if triggered and 0 if not
    }

    out << "doorOpen="          << (level->getDoor()->isOpen()          ? 1 : 0) << "\n"; // Saves whether the door is open
    out << "treasureCollected=" << (level->getTreasure()->isCollected() ? 1 : 0) << "\n"; // Saves whether the treasure was collected

    file.close(); // Closes the file after all data has been written
    view->showMessage("Game saved! (F9 to load)"); // Shows a success message confirming the save worked
}

void Game::loadGame() { // Reads the save file and restores the game to its previously saved state
    QFile file(saveFilePath()); // Creates a file object pointing to the save file path

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) { // Tries to open the file for reading and stops if it doesnt exist
        view->showMessage("No save file found.");
        return;
    }

    QTextStream in(&file); // Creates a text stream so we can read the file line by line
    QMap<QString, QString> data; // Creates a map to store all key-value pairs read from the save file

    while (!in.atEnd()) { // Keeps reading until the end of the file
        QString line = in.readLine().trimmed(); // Reads one line and removes any extra whitespace
        int sep = line.indexOf('='); // Finds the equals sign that separates the key from the value
        if (sep > 0) { // Only processes lines that have a valid equals sign
            data[line.left(sep)] = line.mid(sep + 1); // Splits the line into key and value and stores both in the map
        }
    }
    file.close(); // Closes the file after reading all the data

    currentLevelNum = data.value("level", "1").toInt(); // Restores the saved level number defaulting to 1 if missing
    gameOver = false; // Makes sure the game is active after loading

    level = std::make_unique<Level>(currentLevelNum); // Rebuilds the level from scratch using the saved level number

    Player* p = level->getPlayer(); // Gets a pointer to the player in the freshly built level
    p->setPosition(Position(data.value("px","1").toInt(),
                            data.value("py","1").toInt())); // Restores the player's saved position on the grid

    int savedHP = data.value("php","10").toInt(); // Reads the saved HP value from the file
    int curHP   = p->getHealth(); // Gets the player's current full HP after the level was rebuilt
    if (savedHP < curHP) p->takeDamage(curHP - savedHP); // Reduces HP to match the saved value if it was lower

    QVector<Enemy*>& enemies = level->getEnemies(); // Gets a reference to all enemies in the rebuilt level
    int enemyCount = data.value("enemyCount","0").toInt(); // Reads how many enemies were saved

    for (int i = 0; i < qMin(enemyCount, enemies.size()); i++) { // Loops through each saved enemy up to the level's enemy count
        Enemy* e = enemies[i]; // Gets the current enemy from the list
        bool alive = data.value(QString("e%1_alive").arg(i), "1").toInt() == 1; // Reads whether this enemy was alive when saved
        int ex  = data.value(QString("e%1_x").arg(i),  "0").toInt(); // Reads the saved x position
        int ey  = data.value(QString("e%1_y").arg(i),  "0").toInt(); // Reads the saved y position
        int ehp = data.value(QString("e%1_hp").arg(i), "8").toInt(); // Reads the saved HP
        e->setPosition(Position(ex, ey)); // Restores the enemy's position
        int ecurHP = e->getHealth(); // Gets the enemy's current full HP after the level was rebuilt
        if (ehp < ecurHP) e->takeDamage(ecurHP - ehp); // Reduces HP to match the saved value if it was lower
        if (!alive && e->isAlive()) e->takeDamage(9999); // Kills the enemy if it was dead when saved
    }

    QVector<Trap*>& traps = level->getTraps(); // Gets a reference to all traps in the rebuilt level
    int trapCount = data.value("trapCount","0").toInt(); // Reads how many traps were saved

    for (int i = 0; i < qMin(trapCount, traps.size()); i++) { // Loops through each saved trap up to the level's trap count
        bool triggered = data.value(QString("t%1_triggered").arg(i), "0").toInt() == 1; // Reads whether this trap was already triggered
        traps[i]->setTriggered(triggered); // Restores the trap's triggered state directly
    }

    bool doorOpen = data.value("doorOpen","0").toInt() == 1; // Reads whether the door was open when saved
    if (doorOpen) { // Only runs this block if the door was open
        level->getDoor()->unlock(); // Unlocks the door to match the saved state
        level->getDoor()->openDoor(); // Opens the door visually
        level->getMap()->setTile(level->getDoor()->getPosition(), Tile(DoorTile, true)); // Updates the map tile to show the door as open
    }

    bool treasureCollected = data.value("treasureCollected","0").toInt() == 1; // Reads whether the treasure was collected when saved
    if (treasureCollected) level->getTreasure()->collect(); // Marks the treasure as collected if it was taken before saving

    view->initLevel(level.get()); // Initializes the visual display for the restored level
    turnManager->startPlayerTurn(level->getPlayer()); // Starts the player's turn after the load is complete
    view->redraw(true); // Updates the screen to show the restored game state
    view->showMessage(QString("Game loaded! Level %1").arg(currentLevelNum)); // Shows a success message confirming the load worked
}

void Game::onSaveRequested() { // This slot is triggered when the player presses F5 and calls the save function
    saveGame(); // Calls saveGame to write all current game data to the save file
}

void Game::onLoadRequested() { // This slot is triggered when the player presses F9 and calls the load function
    loadGame(); // Calls loadGame to read the save file and restore the game state
}