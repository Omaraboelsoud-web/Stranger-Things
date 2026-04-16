#include "enemy.h"
#include "player.h"

Enemy::Enemy(QString name, int hp, int atk, int def, Position pos)
    : Character(name, hp, atk, def, pos) {}

bool Enemy::isAdjacentTo(Position p) const {
    return position.distanceTo(p) == 1;
}

Position Enemy::getNextMoveToward(Position target, GridMap* map) const {

    Position dirs[4] = {
        Position(position.x+1, position.y),
        Position(position.x-1, position.y),
        Position(position.x,   position.y+1),
        Position(position.x,   position.y-1)
    };

    Position best = position;
    int bestDist = position.distanceTo(target);

    for (auto& d : dirs) {
        if (map->isWalkable(d) && d.distanceTo(target) < bestDist) {
            bestDist = d.distanceTo(target);
            best = d;
        }
    }
    return best;
}

void Enemy::takeTurn(Player* player, GridMap* map) {
    if (!player->isAlive() || !alive) return;

    Position pPos = player->getPosition();

    if (isAdjacentTo(pPos)) {

        player->takeDamage(attackPower);
    } else {

        Position next = getNextMoveToward(pPos, map);
        if (!next.equals(position)) {
            position = next;
        }
    }
}