#ifndef PLAYER_H
#define PLAYER_H
#include <QString>
#include "character.h"//Includes my custom character classes which I'll use so makes it easier for me rather than creating to many functions
#include "gridmap.h" //Includes my custom gridmap classes which I'll use so makes it easier for me rather than creating to many functions

class Player : public Character { //class player derrived from character
private:
    int actionPoints; // declares current AP
    int maxActionPoints; // declares max AP
public:
    Player(QString name, int hp, int atk, int def, Position pos, int maxAP = 2); //Constructor that declares name, hp, attack, defense, position, and maximum AP
    int  getAP()     const; // Function returns AP
    int  getMaxAP()  const; // Function returns max AP
    bool canAct()    const; // Function checks if player can still perform actions
    void resetAP(); // Restores AP
    bool useAP(int cost); // checks if theres enough AP then uses it if false then not enough AP
    bool canUsePsychicAttack(Position targetPos, GridMap* map) const; // checks if player can perform psychic attacks
};

#endif