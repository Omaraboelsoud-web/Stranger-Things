#include "player.h" //Includes my custom player classes which I'll use so makes it easier for me rather than creating to many functions

Player::Player(QString name, int hp, int atk, int def, Position pos, int maxAP) : Character(name, hp, atk, def, pos), actionPoints(maxAP), maxActionPoints(maxAP) {} // Defines the constructor of the Player class with input parameters for initialization

int  Player::getAP()    const { return actionPoints; } // returns action points
int  Player::getMaxAP() const { return maxActionPoints; } // returns max AP
bool Player::canAct()   const { return actionPoints > 0; } // checks if player has AP

void Player::resetAP() { actionPoints = maxActionPoints; } //Resets Ap by doing AP=maxAP

bool Player::useAP(int cost) { // Defines a function that tries to use action points for an action
    if (actionPoints < cost) return false; //Checks if there are enough action points and returns false if not enough
    actionPoints -= cost; // Deducts the action point cost
    return true; // Confirms the action was successful
}

bool Player::canUsePsychicAttack(Position targetPos, GridMap* map) const { //Defines a function that checks whether a psychic attack is valid
    if (position.x != targetPos.x && position.y != targetPos.y) return false; //Ensures the target is in the same row or column
    if (position.distanceTo(targetPos) > 3) return false; //Ensures the target is within attack range
    int stepx = 0, stepy = 0; // Declares variables to store movement direction
    if (targetPos.x > position.x)      stepx = 1;
    else if (targetPos.x < position.x) stepx = -1;
    else if (targetPos.y > position.y) stepy = 1;
    else                               stepy = -1;
    // Determines horizontal and vertical direction toward the target
    Position check(position.x + stepx, position.y + stepy); //Sets the starting position one step toward the target
    while (!check.equals(targetPos)) { // Loops until the target position is reached
        Tile t = map->getTile(check); // Retrieves the tile at the current checked position
        if (!t.isWalkable()) return false; // Stops if there is something blocking the path
        check.x += stepx;
        check.y += stepy;
    } // Moves the check position one step closer to the target
    return true; // Returns true if the path is clear and attack is valid
}