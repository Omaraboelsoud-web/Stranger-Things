#include "enemy.h" //Includes my custom enemy classes which I'll use so makes it easier for me rather than creating to many functions
#include "player.h" //Includes my custom player classes which I'll use so makes it easier for me rather than creating to many functions

Enemy::Enemy(QString name, int hp, int atk, int def, Position pos) : Character(name, hp, atk, def, pos) {}
// This initializes the Enemy constructors objects
// Takes the objects from the parameters and intialized them to the objects declared in the enemy.h
bool Enemy::isAdjacentTo(Position p) const { return position.distanceTo(p) == 1;} // Checks if enemy is next to a target position
Position Enemy::getNextMoveToward(Position target, GridMap* map) const {
    Position dirs[4] = { // 4 possible directions to move in
        Position(position.x+1, position.y), //Moves right
        Position(position.x-1, position.y), // Moves left
        Position(position.x,   position.y+1), // Moves up
        Position(position.x,   position.y-1) // Moves down
    };
    Position best = position; // Changes the position to the best possible position
    int bestDist = position.distanceTo(target); // Makes bet distance move closer to target
    for (Position& d : dirs) { // This means that the enemy loops through all possible moves
        if (map->isWalkable(d) && d.distanceTo(target) < bestDist) { // this gives an if statement that a move is accepted only if it isWalkable() nothing stopping it and d.distanceTo(target) < bestDist to reduce the distance to the character
            bestDist = d.distanceTo(target);
            best = d;
        }// Both of these save the best move found
    }
    return best; // Returns the best position picked
}
void Enemy::takeTurn(Player* player, GridMap* map) { // This defines what the enemy will do during its turn
    if (!player->isAlive() || !alive) return; // if statements that stops if player is dead or enemy is dead
    Position pPos = player->getPosition(); //This stores where the player is so the enemy always knows
    if (isAdjacentTo(pPos)) {
        player->takeDamage(attackPower);
    } else {
        Position next = getNextMoveToward(pPos, map);
        if (!next.equals(position)) {
            position = next;
        }
    } // This checks the enemy attack rnge if its true then he player will take damage as enegmy will hit him and if not then he will continue normally
}