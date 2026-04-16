#include "hud.h" //Includes my custom hud classes which I'll use so makes it easier for me rather than creating to many functions
#include <QBrush> // Includes the QBrush class used for filling shapes with color
#include <QPen> // Includes the QPen class used for drawing outlines

HUD::HUD(QGraphicsScene* s, int mapHeightPx)
    : scene(s), hudY(mapHeightPx) // Defines the constructor and initializes scene and hudY using an initializer list
{

    hudBg = scene->addRect( //Adds a rectangle to the scene and assigns it to hudBg
        0, hudY, 480, 100, // Sets the position and size of the rectangle
        QPen(Qt::NoPen), // Specifies that the rectangle has no border
        QBrush(QColor(20, 20, 30)) // Fills the rectangle with a dark color
        );
    hudBg->setZValue(10); // Sets the drawing order so the HUD appears above other elements

    playerInfo = scene->addText(""); // Adds a text item for displaying player information
    playerInfo->setDefaultTextColor(QColor(100, 200, 255)); // Sets the text color for player information
    playerInfo->setPos(8, hudY + 8); // Positions the player info text within the HUD 8 rows
    playerInfo->setZValue(10); //Ensures the text appears above other elements

    enemyInfo = scene->addText(""); // Adds text for displaying enemy info
    enemyInfo->setDefaultTextColor(QColor(255, 100, 100)); //Sets the text color for enemy information
    enemyInfo->setPos(8, hudY + 30); // Positions the enemy info text within the HUD
    enemyInfo->setZValue(10); // Ensures the text appears above other elements

    turnInfo = scene->addText(""); // Adds a text item for displaying turn info
    turnInfo->setDefaultTextColor(QColor(255, 220, 80)); // Sets the text color for turn information
    turnInfo->setPos(8, hudY + 52); // Positions the turn info text within the HUD
    turnInfo->setZValue(10); // Ensures the text appears above other elements

    messageLabel = scene->addText(""); // Adds a text item for displaying messages
    messageLabel->setDefaultTextColor(QColor(180, 255, 180)); // Sets the text color for messages
    messageLabel->setPos(8, hudY + 72); // Positions the message text within the HUD
    messageLabel->setZValue(10); // Ensures the text appears above other elements
}

void HUD::update(Player* p, Enemy* e, bool isPlayerTurn) //Defines a function that updates the HUD using player, enemy, and turn state
{
    auto bar = [](int cur, int max, int len = 10) { // Declares a lambda function named bar that generates a visual bar based on values. Auto is used because function type can change
        int filled = (max > 0) ? (cur * len / max) : 0; // Calculates how many segments of the bar should be filled
        QString s; // declares strint to store the bar
        for (int i = 0; i < len; i++) // goes over the length of the bar
            s += (i < filled) ? "█" : "░"; // Adds a filled or empty block character depending on the index
        return s; // returns the constructor of the bar string
    };

    playerInfo->setPlainText( //Updates the player info text
        QString("ELEVEN  HP: %1/%2 [%3]   AP: %4/%5") // Creates a formatted string with placeholders
            .arg(p->getHealth()) // gets player's current health it replaces %1 using .agr()
            .arg(p->getMaxHealth()) // gets players max health rep[laces %2 using .arg()
            .arg(bar(p->getHealth(), p->getMaxHealth())) // gets both health and max health generates the whole healh bar replaces %3 using .agr()
            .arg(p->getAP()) // Replaces %4 with current action points
            .arg(p->getMaxAP()) // Replaces %5 with maximum action points
        );

    if (e->isAlive()) { // Checks if the enemy is alive
        enemyInfo->setPlainText( // Updates enemy info text
            QString("PAPA    HP: %1/%2 [%3]") // Creates formatted string for enemy health
                .arg(e->getHealth()) // Replaces %1 with enemy health
                .arg(e->getMaxHealth()) // Replaces %2 with enemy max health
                .arg(bar(e->getHealth(), e->getMaxHealth())) // Replaces %3 with enemy health bar
            );
    } else { // This will execute if enemy is not alive
        enemyInfo->setPlainText("PAPA    [DEFEATED]"); // message displayed if enemy dead
    }

    turnInfo->setPlainText( // Updates turn information text
        isPlayerTurn ? "▶ YOUR TURN  [WASD = move | Q = attack | Enter = end turn]" : "  PAPA's TURN...");
} // Sets text for player turn and text for enemy turn.

void HUD::showMessage(const QString& text) // Defines a function to display a message
{
    messageLabel->setPlainText(text); // Sets the message text
}

void HUD::clearMessage() // Defines a function to clear the message
{
    messageLabel->setPlainText(""); //Clears the message text
}