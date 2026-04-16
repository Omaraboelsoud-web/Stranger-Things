#ifndef PLAYER_H
#define PLAYER_H
#include <QString>
#include "character.h"
#include "gridmap.h"

class Player : public Character {
private:
    int actionPoints;
    int maxActionPoints;
public:
    Player(QString name, int hp, int atk, int def, Position pos, int maxAP = 2);

    int  getAP()     const;
    int  getMaxAP()  const;
    bool canAct()    const;
    void resetAP();
    bool useAP(int cost);


    bool canUsePsychicAttack(Position targetPos, GridMap* map) const;
};

#endif