#ifndef CHARACTER_H
#define CHARACTER_H

#include <QString> //Including QString as I'll use it in the code
#include "position.h" //Includes my custom position classes which I'll use so makes it easier for me rather than creating to many functions

class Character { // I'll declare all functions and attributes here
protected: //Better than private so I can access the variables without issues inside and inside each inheritence
    QString  name; // Stores the name as text
    int      health; //Stores health
    int      maxHealth; //Stores the maximum health the character can have
    int      attackPower; //Stores the attack power the character deals
    int      defense; // Stores the defense which reduces the damage on character
    Position position; //Stores the characters location on the map
    bool     alive; // Checks if character is alive or not true alive false dead
public: //public because I wabt it to be accessible anywhere in the program
    Character(QString name, int hp, int atk, int def, Position pos); // Constructer initializing new character with name, hp, attack power, defense value, and starting position
    virtual ~Character() = default; // Makes sure the program is clean when derrived objects are deleted. I used virtual because it is a base class so not using virtual can cause memory leaks
    Position getPosition() const; // Returns the characters current position
    void     setPosition(Position p); // Updates the characters position
    int  getHealth()    const; // returns the characters current health
    int  getMaxHealth() const; //returns the characters maximum health
    int  getAttack()    const; // returns the characters attack power
    bool isAlive()      const; // returns whether the character is alive or not
    void takeDamage(int amount); //This will reduce the health when the character gets hit
    void move(Position newPos); // This will move the character to the newer place on the gridmap
};

#endif