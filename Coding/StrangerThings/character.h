#ifndef CHARACTER_H
#define CHARACTER_H

#include <QString>
#include "position.h"

class Character {
protected:
    QString  name;
    int      health;
    int      maxHealth;
    int      attackPower;
    int      defense;
    Position position;
    bool     alive;
public:
    Character(QString name, int hp, int atk, int def, Position pos);


    Position getPosition() const;
    void     setPosition(Position p);

    int  getHealth()    const;
    int  getMaxHealth() const;
    int  getAttack()    const;
    bool isAlive()      const;

    void takeDamage(int amount);
    void move(Position newPos);
};

#endif