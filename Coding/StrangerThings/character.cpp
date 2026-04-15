#include "character.h"

Character::Character(QString name, int hp, int atk, int def, Position pos)
    : name(name), health(hp), maxHealth(hp),
    attackPower(atk), defense(def), position(pos), alive(true) {}

Position Character::getPosition() const { return position; }
void     Character::setPosition(Position p) { position = p; }
int      Character::getHealth()    const { return health; }
int      Character::getMaxHealth() const { return maxHealth; }
int      Character::getAttack()    const { return attackPower; }
bool     Character::isAlive()      const { return alive; }

void Character::takeDamage(int amount) {
    int dmg = qMax(0, amount - defense);
    health -= dmg;
    if (health <= 0) { health = 0; alive = false; }
}

void Character::move(Position newPos) { position = newPos; }