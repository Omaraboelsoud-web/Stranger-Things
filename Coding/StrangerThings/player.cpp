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
    // Must be in same row or same column
    if (position.x != targetPos.x && position.y != targetPos.y) return false;

    // Max range 3
    if (position.distanceTo(targetPos) > 3) return false;

    // Line-of-sight check: no walls or locked doors between
    int dx = 0, dy = 0;
    if (targetPos.x > position.x)      dx = 1;
    else if (targetPos.x < position.x) dx = -1;
    else if (targetPos.y > position.y) dy = 1;
    else                               dy = -1;

    Position check(position.x + dx, position.y + dy);
    while (!check.equals(targetPos)) {
        Tile t = map->getTile(check);
        if (!t.isWalkable()) return false;
        check.x += dx;
        check.y += dy;
    }
    return true;
}