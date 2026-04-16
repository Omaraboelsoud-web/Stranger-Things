#include "character.h" //Includes my custom character classes which I'll use so makes it easier for me rather than creating to many functions

Character::Character(QString name, int hp, int atk, int def, Position pos) : name(name), health(hp), maxHealth(hp), attackPower(atk), defense(def), position(pos), alive(true) {}
// This initializes the Character constructors objects
// Takes the objects from the parameters and intialized them to the objects declared in the character.h

Position Character::getPosition() const { return position; } //Returns the current position of the character
void     Character::setPosition(Position p) { position = p; } // Updates the characters position
int      Character::getHealth()    const { return health; } // Returns current Hp
int      Character::getMaxHealth() const { return maxHealth; } // Returns maximum Hp
int      Character::getAttack()    const { return attackPower; } //Returns attack strength
bool     Character::isAlive()      const { return alive; } // Returns whether character is still alive
void Character::takeDamage(int amount) { // This is called when the character takes damage
    int dmg = qMax(0, amount - defense); // qMax is used to ensure damage is never negative, amount shows the incoming attack, the defense reduces damage, int dmg declares the damage calculation
    health -= dmg; // Subtract the health with damage taken
    if (health <= 0) { health = 0; alive = false; } // If statement if the health is less or equal to 0 then the character died
}
void Character::move(Position newPos) { position = newPos; } // This changes the character's position by declaring the position to new position