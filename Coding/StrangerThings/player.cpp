#include "player.h"

Player::Player(QString name, int hp, int atk, int def, Position pos, int maxAP)
    : Character(name, hp, atk, def, pos),
    actionPoints(maxAP), maxActionPoints(maxAP) {}

int  Player::getAP()    const { return actionPoints; }
int  Player::getMaxAP() const { return maxActionPoints; }
bool Player::canAct()   const { return actionPoints > 0; }

void Player::resetAP() { actionPoints = maxActionPoints; }

bool Player::useAP(int cost) {
    if (actionPoints < cost) return false;
    actionPoints -= cost;
    return true;
}

bool Player::canUsePsychicAttack(Position targetPos, GridMap* map) const {

    if (position.x != targetPos.x && position.y != targetPos.y) return false;

    if (position.distanceTo(targetPos) > 3) return false;

    int stepx = 0, stepy = 0;
    if (targetPos.x > position.x)      stepx = 1;
    else if (targetPos.x < position.x) stepx = -1;
    else if (targetPos.y > position.y) stepy = 1;
    else                               stepy = -1;

    Position check(position.x + stepx, position.y + stepy);
    while (!check.equals(targetPos)) {
        Tile t = map->getTile(check);
        if (!t.isWalkable()) return false;
        check.x += stepx;
        check.y += stepy;
    }
    return true;
}