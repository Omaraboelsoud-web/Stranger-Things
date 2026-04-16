#ifndef HUD_H
#define HUD_H

#include <QGraphicsScene> // This represents the 2D diagram where everthing will be drawn
#include <QGraphicsTextItem> //This is used to display texts on the screen
#include <QGraphicsRectItem> // This is used to draw rectanges which will be used in the HUD
#include "player.h" //Includes my custom player classes which I'll use so makes it easier for me rather than creating to many functions
#include "enemy.h" //Includes my custom enemy classes which I'll use so makes it easier for me rather than creating to many functions

class HUD { // I'll declare all functions and attributes here
private: // All functions will need to be pointers because they are owned somehwere else and HUD class only uses them doesnt own them
    QGraphicsScene* scene; // This is the main drawing surface where the HUD elements are added
    QGraphicsRectItem* hudBg; //This is a background panel tfor the HUD which will be a rectangle
    QGraphicsTextItem* playerInfo; //This displays player stats on screen
    QGraphicsTextItem* enemyInfo; // This displays enemy stats on screen
    QGraphicsTextItem* turnInfo; // This shows who's turn it is player or enemy
    QGraphicsTextItem* messageLabel; // This is used for temporary messages that pop up then disppear
    int hudY; // This stores vertical position of the HUD on screen
public:
    HUD(QGraphicsScene* scene, int mapHeightPx); // This is a constructor that takes the scene positions the HUD below the game
    void update(Player* p, Enemy* e, bool isPlayerTurn); // This updates all HUD information based on current game status.
    void showMessage(const QString& text); //This displays a message on the HUD
    void clearMessage(); //This removes the message displayed on the HUD
};

#endif