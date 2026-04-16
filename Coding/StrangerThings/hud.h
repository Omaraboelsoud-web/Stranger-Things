#ifndef HUD_H
#define HUD_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include "player.h"
#include "enemy.h"

class HUD {
private:
    QGraphicsScene* scene;

    QGraphicsRectItem* hudBg;
    QGraphicsTextItem* playerInfo;
    QGraphicsTextItem* enemyInfo;
    QGraphicsTextItem* turnInfo;
    QGraphicsTextItem* messageLabel;

    int hudY;

public:
    HUD(QGraphicsScene* scene, int mapHeightPx);

    void update(Player* p, Enemy* e, bool isPlayerTurn);
    void showMessage(const QString& text);
    void clearMessage();
};

#endif