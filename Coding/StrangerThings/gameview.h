#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView> //provides a widget to display graphics scene
#include <QGraphicsScene> // it stores and manages the graphical items
#include <QGraphicsPixmapItem> //it displays images of items like the player or enemy
#include <QMap> // Qt container for storing key value pairs
#include "level.h" //includes the level class
#include "hud.h" //includes the HUD class

static const int TILE = 40; // to keep sizes consistent throughout the game

class GameView : public QGraphicsView {// gameview inherits from qgraphicsview so it uses its features plus a few more features
    Q_OBJECT // allows for the use of features like signals slots

private:
    QGraphicsScene* scene; // scene stores the graphics with the tiles, objects and characters and the gameview then shows this scene on the screen
    Level*          level; //stores game logic and map dara
    HUD*            hud; // manages the user interface showing the player heath score messages, etc.


    QGraphicsPixmapItem* playerSprite;//image for player
    QGraphicsPixmapItem* enemySprite;//image for the enemy


    QPixmap pxFloor, pxWall, pxTrap, pxDoorLocked, pxDoorOpen, pxTreasure;
    QPixmap pxEleven, pxPapa;// image for floor, wall, traos, locked door, open door, treasure, players image and enemys image

    bool spritesLoaded;// checks if images were loaded or not

    void loadPixmaps();// loads all the required images
    void drawMap();//draws the map tiles
    void drawObjects();//draws map objects
    void drawCharacters();// draws or updates the player and enemy sprites

public:
    explicit GameView(QWidget* parent = nullptr);// prevents unintended automatic type conversions and the class can optionally have a parent widget
    ~GameView() override;//runs when gameview is destroyed and overrides the destructor from base class

    void initLevel(Level* lvl);// stores that pointer in level and prepares the screen for drawing the map and characters
    void redraw(bool isPlayerTurn);// tells the function whether it is currently the player’s turn and is called whenever the game state changes
    void showMessage(const QString& msg);//shows a message to the player

signals:
    void moveRequested(int dx, int dy);//used when a movement action is requested
    void psychicAttackRequested();//indicates that the player wants to perform a psychic attack
    void endTurnRequested();//indicates that the player wants to end their turn

protected:
    void keyPressEvent(QKeyEvent* event) override;//contains information about which key was pressed and override means this function replaces the default behavior
};

#endif