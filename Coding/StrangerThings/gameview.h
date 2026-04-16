#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMap>
#include "level.h"
#include "hud.h"

static const int TILE = 40;

class GameView : public QGraphicsView {
    Q_OBJECT

private:
    QGraphicsScene* scene;
    Level*          level;
    HUD*            hud;


    QGraphicsPixmapItem* playerSprite;
    QGraphicsPixmapItem* enemySprite;


    QPixmap pxFloor, pxWall, pxTrap, pxDoorLocked, pxDoorOpen, pxTreasure;
    QPixmap pxEleven, pxPapa;

    bool spritesLoaded;

    void loadPixmaps();
    void drawMap();
    void drawObjects();
    void drawCharacters();

public:
    explicit GameView(QWidget* parent = nullptr);
    ~GameView() override;

    void initLevel(Level* lvl);
    void redraw(bool isPlayerTurn);
    void showMessage(const QString& msg);

signals:
    void moveRequested(int dx, int dy);
    void psychicAttackRequested();
    void endTurnRequested();

protected:
    void keyPressEvent(QKeyEvent* event) override;
};

#endif