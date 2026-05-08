#include "enemy.h" //Includes my custom enemy classes which I'll use so makes it easier for me rather than creating to many functions
#include "player.h" //Includes my custom player classes which I'll use so makes it easier for me rather than creating to many functions
#include "gridmap.h" //Includes my custom gridmap classes which I'll use so makes it easier for me rather than creating to many functions
#include "tile.h" //Includes tile system so enemy can detect walls, traps, etc.

Enemy::Enemy(QString name, int hp, int atk, int def, Position pos) : Character(name, hp, atk, def, pos) {}
// This initializes the Enemy constructors objects
// Takes the objects from the parameters and intialized them to the objects declared in the enemy.h

bool Enemy::isAdjacentTo(Position p) const { return position.distanceTo(p) == 1; }
// Checks if enemy is next to a target position

Position Enemy::getNextMoveToward(Position target, GridMap* map,
                                  const QVector<Enemy*>& allEnemies) const {

    Position dirs[4] = { // 4 possible directions to move in
        Position(position.x + 1, position.y), //Moves right
        Position(position.x - 1, position.y), // Moves left
        Position(position.x,     position.y + 1), // Moves down
        Position(position.x,     position.y - 1)  // Moves up
    };

    Position best = position; // Changes the position to the best possible position
    int bestDist = position.distanceTo(target); // Makes best distance move closer to target

    for (Position& d : dirs) { // This means that the enemy loops through all possible moves

        // Must be walkable on the map
        if (!map->isWalkable(d)) continue; // skips tiles that are blocked (walls, invalid tiles, etc.)

        // Enemies avoid trap tiles
        Tile t = map->getTile(d); // gets tile info at that position
        if (t.getType() == TrapTile) continue; // avoids traps

        // Enemies avoid tiles occupied by another living enemy
        bool occupiedByEnemy = false; // checks if another enemy is already there
        for (Enemy* other : allEnemies) {
            if (other != this && other->isAlive() && other->getPosition().equals(d)) {
                occupiedByEnemy = true; // collision detected with another enemy
                break;
            }
        }

        if (occupiedByEnemy) continue; // skip move if another enemy is there

        // Accept only if it brings us closer to the target
        if (d.distanceTo(target) < bestDist) { // checks if move reduces distance to player
            bestDist = d.distanceTo(target);
            best = d; // saves best move found
        } // Both of these save the best move found
    }

    return best; // Returns the best position picked
}

void Enemy::takeTurn(Player* player, GridMap* map,
                     const QVector<Enemy*>& allEnemies) { // This defines what the enemy will do during its turn

    if (!player->isAlive() || !alive) return; // if statements that stop if player is dead or enemy is dead

    Position pPos = player->getPosition(); // This stores where the player is so the enemy always knows

    if (isAdjacentTo(pPos)) {
        player->takeDamage(attackPower); // enemy attacks if in range
    } else {
        Position next = getNextMoveToward(pPos, map, allEnemies); // calculates best move toward player

        if (!next.equals(position)) {
            position = next; // moves enemy to new position
        }
    } // This checks the enemy attack range if true then player takes damage otherwise enemy moves normally
}