#include "hud.h" //Includes my custom hud classes which I'll use so makes it easier for me rather than creating to many functions
#include <QBrush> // Includes the QBrush class used for filling shapes with color
#include <QPen> // Includes the QPen class used for drawing outlines

HUD::HUD(QGraphicsScene* s, int mapHeightPx)
    : scene(s), hudY(mapHeightPx) // Defines the constructor and initializes scene and hudY using an initializer list
{
    // Background panel
    hudBg = scene->addRect( //Adds a rectangle to the scene and assigns it to hudBg
        0, hudY, 720, 102, // Sets the position and size of the rectangle
        QPen(Qt::NoPen), // Specifies that the rectangle has no border
        QBrush(QColor(20, 20, 30)) // Fills the rectangle with a dark color
        );
    hudBg->setZValue(10); // Sets the drawing order so the HUD appears above other elements

    // Level info
    levelInfo = scene->addText(""); // Adds a text item for displaying level information
    levelInfo->setDefaultTextColor(QColor(200, 150, 255)); // Sets the text color for level information
    levelInfo->setPos(8, hudY + 4); // Positions the level info text within the HUD
    levelInfo->setZValue(10); // Ensures the text appears above other elements

    // Player info
    playerInfo = scene->addText(""); // Adds a text item for displaying player information
    playerInfo->setDefaultTextColor(QColor(100, 200, 255)); // Sets the text color for player information
    playerInfo->setPos(8, hudY + 22); // Positions the player info text within the HUD
    playerInfo->setZValue(10); //Ensures the text appears above other elements

    // Enemy info
    enemyInfo = scene->addText(""); // Adds text for displaying enemy info
    enemyInfo->setDefaultTextColor(QColor(255, 100, 100)); //Sets the text color for enemy information
    enemyInfo->setPos(8, hudY + 42); // Positions the enemy info text within the HUD
    enemyInfo->setZValue(10); // Ensures the text appears above other elements

    // Turn info
    turnInfo = scene->addText(""); // Adds a text item for displaying turn info
    turnInfo->setDefaultTextColor(QColor(255, 220, 80)); // Sets the text color for turn information
    turnInfo->setPos(8, hudY + 62); // Positions the turn info text within the HUD
    turnInfo->setZValue(10); // Ensures the text appears above other elements

    // Message
    messageLabel = scene->addText(""); // Adds a text item for displaying messages
    messageLabel->setDefaultTextColor(QColor(180, 255, 180)); // Sets the text color for messages
    messageLabel->setPos(8, hudY + 82); // Positions the message text within the HUD
    messageLabel->setZValue(10); // Ensures the text appears above other elements
}

void HUD::update(Player* p, Enemy* primaryEnemy, bool isPlayerTurn,
                 int levelNum, const QVector<Enemy*>& allEnemies) //Defines a function that updates the HUD using player, enemy, level, and turn state
{
    // Health bar
    auto bar = [](int cur, int max, int len = 8) { // Declares a lambda function named bar that generates a visual bar based on values. Auto is used because function type can change
        int filled = (max > 0) ? (cur * len / max) : 0; // Calculates how many segments of the bar should be filled
        QString s = "["; // declares string and starts the health bar with [
        for (int i = 0; i < len; i++) // goes over the length of the bar
            s += (i < filled) ? "|" : "-"; // Adds a filled or empty section depending on health amount
        s += "]"; // Closes the health bar with ]
        return s; // returns the constructor of the bar string
    };

    // Level label
    levelInfo->setPlainText(QString("LEVEL %1 / 5").arg(levelNum)); // Shows the current level number

    // Player info
    playerInfo->setPlainText( //Updates the player info text
        QString("ELEVEN  HP: %1/%2 %3   AP: %4/%5") // Creates a formatted string with placeholders
            .arg(p->getHealth()) // gets player's current health it replaces %1 using .agr()
            .arg(p->getMaxHealth()) // gets players max health replaces %2 using .arg()
            .arg(bar(p->getHealth(), p->getMaxHealth())) // gets both health and max health generates the whole healh bar replaces %3 using .agr()
            .arg(p->getAP()) // Replaces %4 with current action points
            .arg(p->getMaxAP()) // Replaces %5 with maximum action points
        );

    // Enemy summary
    QString eText; // Stores all enemy information text
    int aliveCount = 0; // Counter for alive enemies
    for (Enemy* e : allEnemies) if (e->isAlive()) aliveCount++; // Loops through all enemies and counts alive ones

    if (aliveCount == 0) { // Checks if all enemies are defeated
        eText = "ENEMIES: [ALL DEFEATED]";
    } else if (primaryEnemy && primaryEnemy->isAlive()) { // Checks if main enemy exists and is alive
        eText = QString("%1  HP: %2/%3 %4") // Creates enemy health display text
                    .arg(primaryEnemy->getName()) // Replaces %1 with enemy name
                    .arg(primaryEnemy->getHealth()) // Replaces %2 with enemy current health
                    .arg(primaryEnemy->getMaxHealth()) // Replaces %3 with enemy maximum health
                    .arg(bar(primaryEnemy->getHealth(), primaryEnemy->getMaxHealth())); // Replaces %4 with enemy health bar

        if (allEnemies.size() > 1) { // Checks if there are multiple enemies in the level
            eText += QString("   (+%1 more alive)").arg(aliveCount - 1); // Shows how many extra enemies are still alive
        }
    } else { // Executes if no main enemy is alive
        eText = QString("ENEMIES: %1 remaining").arg(aliveCount); // Displays remaining enemy count
    }

    enemyInfo->setPlainText(eText); // Updates enemy information text on screen

    // Turn info
    turnInfo->setPlainText( // Updates turn information text
        isPlayerTurn
            ? "> YOUR TURN  [WASD/Arrows=move | Q=psychic | Enter=end turn | F5=Save | F9=Load]"
            : "  ENEMY TURN..."
        ); // Sets text for player turn and text for enemy turn.
}

void HUD::showMessage(const QString& text) // Defines a function to display a message
{
    messageLabel->setPlainText(text); // Sets the message text
}

void HUD::clearMessage() // Defines a function to clear the message
{
    messageLabel->setPlainText(""); //Clears the message text
}