#include "hud.h"
#include <QBrush>
#include <QPen>

HUD::HUD(QGraphicsScene* s, int mapHeightPx)
    : scene(s), hudY(mapHeightPx)
{
    // Dark background bar
    hudBg = scene->addRect(
        0, hudY, 480, 100,
        QPen(Qt::NoPen),
        QBrush(QColor(20, 20, 30))
        );
    hudBg->setZValue(10);

    playerInfo = scene->addText("");
    playerInfo->setDefaultTextColor(QColor(100, 200, 255));
    playerInfo->setPos(8, hudY + 8);
    playerInfo->setZValue(10);

    enemyInfo = scene->addText("");
    enemyInfo->setDefaultTextColor(QColor(255, 100, 100));
    enemyInfo->setPos(8, hudY + 30);
    enemyInfo->setZValue(10);

    turnInfo = scene->addText("");
    turnInfo->setDefaultTextColor(QColor(255, 220, 80));
    turnInfo->setPos(8, hudY + 52);
    turnInfo->setZValue(10);

    messageLabel = scene->addText("");
    messageLabel->setDefaultTextColor(QColor(180, 255, 180));
    messageLabel->setPos(8, hudY + 72);
    messageLabel->setZValue(10);
}

void HUD::update(Player* p, Enemy* e, bool isPlayerTurn)
{
    auto bar = [](int cur, int max, int len = 10) {
        int filled = (max > 0) ? (cur * len / max) : 0;
        QString s;
        for (int i = 0; i < len; i++)
            s += (i < filled) ? "█" : "░";
        return s;
    };

    playerInfo->setPlainText(
        QString("ELEVEN  HP: %1/%2 [%3]   AP: %4/%5")
            .arg(p->getHealth())
            .arg(p->getMaxHealth())
            .arg(bar(p->getHealth(), p->getMaxHealth()))
            .arg(p->getAP())
            .arg(p->getMaxAP())
        );

    if (e->isAlive()) {
        enemyInfo->setPlainText(
            QString("PAPA    HP: %1/%2 [%3]")
                .arg(e->getHealth())
                .arg(e->getMaxHealth())
                .arg(bar(e->getHealth(), e->getMaxHealth()))
            );
    } else {
        enemyInfo->setPlainText("PAPA    [DEFEATED]");
    }

    turnInfo->setPlainText(
        isPlayerTurn
            ? "▶ YOUR TURN  [WASD = move | Q = attack | Enter = end turn]"
            : "  PAPA's TURN..."
        );
}

void HUD::showMessage(const QString& text)
{
    messageLabel->setPlainText(text);
}

void HUD::clearMessage()
{
    messageLabel->setPlainText("");
}